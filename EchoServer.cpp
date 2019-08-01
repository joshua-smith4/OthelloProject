#include <iostream>

#include "Server.hpp"

class EchoServer : public server::UDPServer
{
public:
    EchoServer(boost::asio::io_context& io_context)
        : server::UDPServer(io_context)
    {
    }
private:
    void handler_receive(boost::system::error_code const& error, std::size_t bytes_recd) override final
    {
    }

    void handle_send(boost::system::error_code const & error, std::size_t bytes_sent) override final
    {
    }
};
