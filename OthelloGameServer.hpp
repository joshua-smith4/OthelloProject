#include "UDPServer.hpp"
#include "EventSystem.hpp"

class OthelloGameServer : public server::UDPServer
{
public:
private:
    void handle_receive(
          std::shared_ptr<std::vector<char>> /*message*/, 
          std::shared_ptr<boost_ip::udp::endpoint> /*sender_endpoint*/, 
          boost::system::error_code const& /*error code*/, 
          std::size_t /*bytes_received*/) override final;
};
