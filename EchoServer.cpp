#include <iostream>

#include "Server.hpp"

class EchoServer : public server::UDPServer
{
public:
    EchoServer(boost::asio::io_context& io_context, unsigned short port, size_t const MAX_MESSAGE_SIZE)
        : server::UDPServer(io_context, port, MAX_MESSAGE_SIZE)
    {
    }
private:
    void handle_receive(
            std::shared_ptr<std::vector<char>> msg,
            std::shared_ptr<boost::asio::ip::udp::endpoint> sender,
            boost::system::error_code const& error, 
            std::size_t bytes_recd) override final
    {
        if(!error && bytes_recd > 0)
        {
            this->_socket.async_send_to(
                    boost::asio::buffer(*msg, bytes_recd), 
                    *sender,
                    [](boost::system::error_code const&, std::size_t){});
        }
    }
};

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: async_udp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_context io_context;

    EchoServer s(io_context, std::atoi(argv[1]), 1024u);

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
    return 0;
}
