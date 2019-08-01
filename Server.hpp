#include <string>
#include <array>
#include <boost/asio.hpp>

namespace server
{
    namespace boost_udp =  boost::asio::ip::udp;

    template <std::size_t MAX_MESSAGE_SIZE>
    class UDPServer
    {
    public:
      UDPServer(boost::asio::io_context& io_context, unsigned short port)
        : _socket(io_context, boost_udp::endpoint(boost_udp::v4(), port))
      {
          start_receive();
      }

    private:
      void start_receive()
      {
          _socket.async_receive_from(
                  boost::asio::buffer(recv_buffer, MAX_MESSAGE_SIZE),
                  sender_endpoint,
                  std::bind(&UDPServer::handle_receive, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred));
      }

      virtual void handle_receive(boost::system::error_code const&, std::size_t) = 0;

      virtual void handle_send(boost::system::error_code const&, std::size_t) = 0;

      boost_udp::socket _socket;

    protected:
      std::array<char, MAX_MESSAGE_SIZE> recv_buffer;
      boost_udp::endpoint sender_endpoint;
    };
}
