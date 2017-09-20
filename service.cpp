#include "service.hpp"

#include <chucho/log.hpp>

#include <boost/endian/conversion.hpp>

namespace smile
{

service::service(boost::asio::io_service& io,
                 const std::string& system_name,
                 service_port_map& port_map)
    : io_(io),
      port_map_(port_map),
      system_name_(system_name),
      sock_(io)
{
}

void service::connect(std::chrono::milliseconds max_wait)
{
    port_ = port_map_.port(name());
    boost::asio::ip::tcp::resolver resolver(io_);
    boost::asio::ip::tcp::resolver::query query(system_name_,
                                                std::to_string(port_));
    auto prom = std::make_shared<std::promise<void>>();
    auto fut = prom->get_future();
    CHUCHO_DEBUG_LGBL("Starting connection " << system_name_ << ':' << port_ << ' ' << name());
    resolver.async_resolve(query,
                           std::bind(&service::resolve_handler,
                                     this,
                                     shared_from_this(),
                                     prom,
                                     std::placeholders::_1,
                                     std::placeholders::_2));
    if(fut.wait_for(max_wait) == std::future_status::timeout)
        CHUCHO_ERROR_LGBL("Timed out waiting for the connection to finish");
}

void service::connect_handler(std::shared_ptr<service> myself,
                              std::shared_ptr<std::promise<void>> prom,
                              const boost::system::error_code& ec)
{
    if (ec)
    {
        CHUCHO_ERROR_LGBL("Error connecting to " << system_name_ <<
            ':' << port_ << ": " << ec);
    }
    else
    {
        CHUCHO_DEBUG_LGBL("Connected successfully to " << system_name_ <<
            ':' << port_ << ' ' << name());
        connect_sig_(system_name_, name(), port_);
    }
    prom->set_value();
}

void service::read_data_handler(std::shared_ptr<service> myself,
                                std::shared_ptr<std::vector<std::uint8_t>> buf,
                                std::uint32_t correlation_id,
                                const boost::system::error_code& ec,
                                std::size_t count)
{
    raw_reply rp;
    if (ec)
    {
        rp.except = std::make_exception_ptr(std::runtime_error("Error reading packet data: " + ec.message()));
    }
    else
    {
        buf->insert(buf->begin(), sizeof(std::uint32_t));
        *reinterpret_cast<std::uint32_t*>(&buf->at(0)) = boost::endian::native_to_big(buf->size());
        rp.bytes = buf;
    }
    std::lock_guard<std::mutex> lock(reply_guard_);
    replies_[correlation_id] = rp;
    reply_cond_.notify_one();
}

void service::read_length_handler(std::shared_ptr<service> myself,
                                  std::shared_ptr<std::array<std::uint8_t, sizeof(std::uint32_t)>> buf,
                                  std::uint32_t correlation_id,
                                  const boost::system::error_code& ec,
                                  std::size_t count)
{
    if (ec)
    {
        raw_reply rp;
        rp.except = std::make_exception_ptr(std::runtime_error("Error reading packet length: " + ec.message()));
        std::lock_guard<std::mutex> lock(reply_guard_);
        replies_[correlation_id] = rp;
        reply_cond_.notify_one();
    }
    else
    {
        auto buf_len = boost::endian::big_to_native(*reinterpret_cast<std::uint32_t*>(buf->begin())) -
            sizeof(std::uint32_t);
        auto dbuf = std::make_shared<std::vector<std::uint8_t>>(buf_len);
        boost::asio::async_read(sock_,
                                boost::asio::buffer(*buf),
                                std::bind(&service::read_data_handler,
                                          this,
                                          myself,
                                          dbuf,
                                          correlation_id,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
}

std::unique_ptr<access_reply> service::receive(std::uint32_t correlation_id,
                                               std::chrono::milliseconds max_wait)
{
    raw_reply rp;
    std::unique_lock<std::mutex> lock(reply_guard_);
    if(reply_cond_.wait_for(lock,
                             max_wait,
                             [&]() { return replies_.find(correlation_id) != replies_.end(); }))
    {
        auto found = replies_.find(correlation_id);
        assert(found != replies_.end());
        rp = found->second;
        replies_.erase(found);
    }
    else
    {
        rp.except = std::make_exception_ptr(std::runtime_error("Timeout waiting for reply " +
                                                               std::to_string(correlation_id)));
    }
    lock.unlock();
    if(rp.except)
        std::rethrow_exception(rp.except);
    access_input_packet packet(*rp.bytes);
    return std::move(packet_to_reply(packet));
}

void service::resolve_handler(std::shared_ptr<service> myself,
                              std::shared_ptr<std::promise<void>> prom,
                              const boost::system::error_code& ec,
                              boost::asio::ip::tcp::resolver::iterator itor)
{
    if (ec)
    {
        CHUCHO_ERROR_LGBL("Error resolving host name " << system_name_ <<
            ':' << port_ << ": " << ec);
    }
    else
    {
        boost::asio::async_connect(sock_,
                                   itor,
                                   std::bind(&service::connect_handler,
                                             this,
                                             myself,
                                             prom,
                                             std::placeholders::_1));
    }
}

std::uint32_t service::send(access_request& req)
{
    auto corr = correlation_++;
    req.set_correlation_id(corr);
    req.finish();
    auto buf = std::make_shared<std::vector<std::uint8_t>>(req.bytes());
    boost::asio::async_write(sock_,
                             boost::asio::buffer(*buf),
                             std::bind(&service::send_handler,
                                       this,
                                       shared_from_this(),
                                       buf,
                                       corr,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
    return corr;
}

void service::send_handler(std::shared_ptr<service> myself,
                           std::shared_ptr<std::vector<std::uint8_t>> data,
                           std::uint32_t correlation_id,
                           const boost::system::error_code& ec,
                           std::size_t bytes_transferred)
{
    if (ec)
    {
        raw_reply rp;
        rp.except = std::make_exception_ptr(std::runtime_error("Error sending: " + ec.message()));
        std::lock_guard<std::mutex> lock(reply_guard_);
        replies_[correlation_id] = rp;
        reply_cond_.notify_one();
    }
    else
    {
        auto buf = std::make_shared<std::array<std::uint8_t, sizeof(std::uint32_t)>>();
        boost::asio::async_read(sock_,
                                boost::asio::buffer(*buf),
                                std::bind(&service::read_length_handler,
                                          this,
                                          myself,
                                          buf,
                                          correlation_id,
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
}

void service::send_no_reply(access_request& req, std::chrono::milliseconds max_wait)
{
    auto corr = correlation_++;
    req.set_correlation_id(corr);
    req.finish();
    auto buf = std::make_shared<std::vector<std::uint8_t>>(req.bytes());
    auto prom = std::make_shared<std::promise<void>>();
    auto fut = prom->get_future();
    boost::asio::async_write(sock_,
                             boost::asio::buffer(*buf),
                             std::bind(&service::send_no_reply_handler,
                                       this,
                                       shared_from_this(),
                                       buf,
                                       corr,
                                       prom,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
    if(fut.wait_for(max_wait) == std::future_status::timeout)
        CHUCHO_ERROR_LGBL("Timed out waiting for the no-reply send to finish");
}

void service::send_no_reply_handler(std::shared_ptr<service> myself,
                                    std::shared_ptr<std::vector<std::uint8_t>> data,
                                    std::uint32_t correlation_id,
                                    std::shared_ptr<std::promise<void>> prom,
                                    const boost::system::error_code& ec,
                                    std::size_t bytes_transferred)
{
    if (ec)
        CHUCHO_ERROR_LGBL("Error sending no-reply data: " << ec.message());
    prom->set_value();
}

}