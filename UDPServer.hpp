#include <string>
#include <array>
#include <memory>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace server
{
    namespace boost_ip =  boost::asio::ip;

    class UDPServer
    {
    private:

      virtual void handle_receive(
              std::shared_ptr<std::vector<char>> /*message*/, 
              std::shared_ptr<boost_ip::udp::endpoint> /*sender_endpoint*/, 
              boost::system::error_code const& /*error code*/, 
              std::size_t /*bytes_received*/) = 0;

    public:
      UDPServer(boost::asio::io_context& io_context, unsigned short port, size_t const MAX_MESSAGE_SIZE)
        : _socket(io_context, boost_ip::udp::endpoint(boost_ip::udp::v4(), port)),
          MAX_MESSAGE_SIZE(MAX_MESSAGE_SIZE)
      { start_receive(); }

      virtual ~UDPServer() {}

      template <class HandlerFunc>
      void async_send_to(std::string const& msg, boost_ip::udp::endpoint const& to_endpoint, HandlerFunc&& handler)
      {
          _socket.async_send_to(msg, to_endpoint, handler);
      }

    protected:
      void start_receive()
      {
          auto sender_endpoint = std::make_shared<boost_ip::udp::endpoint>();
          auto buffer = std::make_shared<std::vector<char>>();
          buffer->resize(MAX_MESSAGE_SIZE);
          _socket.async_receive_from(
                  boost::asio::buffer(*buffer, MAX_MESSAGE_SIZE),
                  *sender_endpoint,
                  [this, sender_endpoint, buffer](boost::system::error_code const& err, std::size_t bytes_recd)
                  { 
                    this->handle_receive(buffer, sender_endpoint, err, bytes_recd); 
                    this->start_receive();
                  });
      }

      boost_ip::udp::socket _socket;
      const size_t MAX_MESSAGE_SIZE;
    };
}

