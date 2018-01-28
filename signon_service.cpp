#include "signon_service.hpp"
#include "signon_info_reply.hpp"
#include "end_signon_service_request.hpp"

#include <chucho/log.hpp>

namespace smile
{

signon_service::signon_service(boost::asio::io_service& io,
                               const std::string& system_name,
                               service_port_map& port_map)
    : service(io, system_name, port_map)
{
    rename_logger(typeid(*this));
}

signon_service::~signon_service()
{
    try
    {
        end_signon_service_request req;
        send_no_reply(req, std::chrono::milliseconds(1000));
    }
    catch (const std::exception& e)
    {
        CHUCHO_ERROR_LGBL("Error ending signon service: " << e.what());
    }
}

std::uint16_t signon_service::id() const
{
    return IDENTIFIER;
}

const char* signon_service::name() const
{
    return "as-signon";
}

std::unique_ptr<access_reply> signon_service::packet_to_reply(const access_input_packet& packet) const
{
    std::unique_ptr<access_reply> result;
    switch (packet.request_reply_id())
    {
        case 0xf001:
            //result = new ExchangeRandomSeedsReply(packet);
            assert(false);
            break;
        case 0xf002:
            //result = new StartServerReply(packet);
            assert(false);
            break;
        case 0xf003:
            //result = new SignonExchangeAttributesReply(packet);
            assert(false);
            break;
        case 0xf004:
            result = std::make_unique<signon_info_reply>(packet);
            break;
        case 0xf005:
            // Change password
            assert(false);
            break;
        case 0xf007:
            // Generate authentication token
            assert(false);
            break;
        case 0xf008:
            // Signon get authentication token
            assert(false);
            break;
        default:
            throw std::runtime_error(std::string(name()) + ": Unrecognized request-reply ID: " +
                std::to_string(packet.request_reply_id()));
    }
    return std::move(result);
}

}