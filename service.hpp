#ifndef SMILE_SERVICE_HPP
#define SMILE_SERVICE_HPP

#include "service_port_map.hpp"
#include "access_request.hpp"
#include "access_reply.hpp"
#include "access_input_packet.hpp"

#include <boost/asio.hpp>
#include <boost/signals2/signal.hpp>
#include <chucho/loggable.hpp>

#include <string>
#include <map>
#include <chrono>
#include <condition_variable>
#include <future>

namespace smile
{

/**
 * Service will be created as a weak pointer by as400. Users of
 * service will have to periodically check that their weak pointer
 * is still valid.
 *
 * All I/O done through boost::asio must be async from this class
 * and its subclasses.
 */
class service : public std::enable_shared_from_this<service>, public chucho::loggable<service>
{
public:
    service(boost::asio::io_service& io,
            const std::string& system_name,
            service_port_map& port_map);
    virtual ~service() { }

    void connect(std::chrono::milliseconds max_wait);

    boost::signals2::connection connect_to_connect_sig(std::function<void(const std::string& system_name,
                                                                          const char* const service_name,
                                                                          std::uint16_t port)> func);
    virtual std::uint16_t id() const = 0;
    virtual const char* name() const = 0;
    std::unique_ptr<access_reply> receive(std::uint32_t correlation_id,
                                          std::chrono::milliseconds max_wait);
    std::uint32_t send(access_request& req);
    void send_no_reply(access_request& req, std::chrono::milliseconds max_wait);

protected:
    virtual std::unique_ptr<access_reply> packet_to_reply(const access_input_packet& packet) const = 0;

private:
    struct raw_reply
    {
        std::shared_ptr<std::vector<std::uint8_t>> bytes;
        std::exception_ptr except;
    };

    void connect_handler(std::shared_ptr<service> myself,
                         std::shared_ptr<std::promise<void>> prom,
                         const boost::system::error_code& ec);
    void resolve_handler(std::shared_ptr<service> myself,
                         std::shared_ptr<std::promise<void>> prom,
                         const boost::system::error_code& ec,
                         boost::asio::ip::tcp::resolver::iterator itor);
    void read_data_handler(std::shared_ptr<service> myself,
                           std::shared_ptr<std::vector<std::uint8_t>> buf,
                           std::uint32_t correlation_id,
                           const boost::system::error_code& ec,
                           std::size_t count);
    void read_length_handler(std::shared_ptr<service> myself,
                             std::shared_ptr<std::array<std::uint8_t, sizeof(std::uint32_t)>> buf,
                             std::uint32_t correlation_id,
                             const boost::system::error_code& ec,
                             std::size_t count);
    void send_handler(std::shared_ptr<service> myself,
                      std::shared_ptr<std::vector<std::uint8_t>> data,
                      std::uint32_t correlation_id,
                      const boost::system::error_code& ec,
                      std::size_t bytes_transferred);
    void send_no_reply_handler(std::shared_ptr<service> myself,
                               std::shared_ptr<std::vector<std::uint8_t>> data,
                               std::uint32_t correlation_id,
                               std::shared_ptr<std::promise<void>> prom,
                               const boost::system::error_code& ec,
                               std::size_t bytes_transferred);

    boost::asio::io_service& io_;
    std::uint16_t port_;
    service_port_map& port_map_;
    std::string system_name_;
    boost::asio::ip::tcp::socket sock_;
    std::atomic<std::uint32_t> correlation_;
    boost::signals2::signal<void(const std::string& system_name,
                                 const char* const service_name,
                                 std::uint16_t port)> connect_sig_;
    std::map<std::uint32_t, raw_reply> replies_;
    std::mutex reply_guard_;
    std::condition_variable reply_cond_;
};

inline boost::signals2::connection service::connect_to_connect_sig(std::function<void(const std::string&,
                                                                                      const char* const,
                                                                                      std::uint16_t)> func)
{
    return connect_sig_.connect(func);
}

}

#endif
