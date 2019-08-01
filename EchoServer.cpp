#include <iostream>

#include "Server.hpp"

template <std::size_t MAX_MESSAGE_SIZE>
class EchoServer : public server::UDPServer<MAX_MESSAGE_SIZE>
{
public:
    EchoServer(boost::asio::io_context& io_context, unsigned short port)
        : server::UDPServer<MAX_MESSAGE_SIZE>(io_context, port)
    {
    }
private:
    void handle_receive(boost::system::error_code const& error, std::size_t bytes_recd) override final
    {
        if(!error && bytes_recd > 0)
        {
            this->_socket.async_send_to(boost::asio::buffer(this->recv_buffer, bytes_recd), this->sender_endpoint, 
                    [this](boost::system::error_code const& err, std::size_t bs){ this->handle_send(err, bs); });
        }
        else
        {
            this->start_receive();
        }
    }

    void handle_send(boost::system::error_code const & error, std::size_t bytes_sent) override final
    {
        this->start_receive();
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

    EchoServer<1024u> s(io_context, std::atoi(argv[1]));

    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }
    return 0;
}
