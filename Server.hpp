#include <string>
#include <array>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace server
{
    namespace boost_ip =  boost::asio::ip;

    template <std::size_t MAX_MESSAGE_SIZE>
    class UDPServer
    {
    public:
      UDPServer(boost::asio::io_context& io_context, unsigned short port)
        : _socket(io_context, boost_ip::udp::endpoint(boost_ip::udp::v4(), port))
      {
          start_receive();
      }
      virtual ~UDPServer() {}

      template <class HandlerFunc>
      void async_send_to(std::string const& msg, boost_ip::udp::endpoint const& to_endpoint, HandlerFunc&& handler)
      {
          _socket.async_send_to(msg, to_endpoint, handler);
      }

    private:

      virtual void handle_receive(boost::system::error_code const&, std::size_t) = 0;

      virtual void handle_send(boost::system::error_code const&, std::size_t) = 0;

    protected:
      void start_receive()
      {
          _socket.async_receive_from(
                  boost::asio::buffer(recv_buffer, MAX_MESSAGE_SIZE),
                  sender_endpoint,
                  [this](boost::system::error_code const& err, std::size_t bytes_recd){ this->handle_receive(err, bytes_recd); });
      }
      std::array<char, MAX_MESSAGE_SIZE> recv_buffer;
      boost_ip::udp::endpoint sender_endpoint;
      boost_ip::udp::socket _socket;
    };
}
