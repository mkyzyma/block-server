#ifndef LIB_KEY_SERVER_HPP_
#define LIB_KEY_SERVER_HPP_

#include <boost/asio/awaitable.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <utility>

#include <vector>

using bytes = std::vector<u_char>;
template <class BaseServer, size_t KeyLength = 128> class key_server {
private:
  using tcp = boost::asio::ip::tcp;

public:
  template <typename Handler>
  void start(boost::asio::io_context &ioc, std::string_view host, ushort port,
             Handler &&handler) {
    base_.start(
        ioc, host, port,
        [this, handler](tcp::socket socket) -> boost::asio::awaitable<void> {
          return handle_request(std::move(socket), handler);
        });
  }

private:
  template <typename Handler>
  boost::asio::awaitable<void> handle_request(tcp::socket socket,
                                              Handler &&handler) {
    boost::asio::streambuf b;

    try {
      while (true) {
        boost::asio::streambuf::mutable_buffers_type bufs =
            b.prepare(KeyLength);

        size_t length = 0;
        while (length < KeyLength) {
          length +=
              co_await socket.async_read_some(bufs, boost::asio::use_awaitable);
        }

        auto key_ptr = boost::asio::buffer_cast<u_char *>(bufs);

        bytes key{key_ptr, key_ptr + KeyLength};

        co_await handler(socket, std::move(key));

        b.consume(KeyLength);
      }
    } catch (boost::system::system_error se) {
      if (se.code() != boost::asio::error::eof) {
        throw;
      }
    }
  }

private:
  BaseServer base_;
};

#endif // LIB_KEY_SERVER_HPP_
