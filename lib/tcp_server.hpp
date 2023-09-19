#ifndef LIB_TCP_SERVER_HPP_
#define LIB_TCP_SERVER_HPP_

#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <iostream>
#include <string_view>
#include <utility>

class tcp_server {
private:
  using tcp = boost::asio::ip::tcp;

public:
  template <typename Handler>
  void start(boost::asio::io_context &ioc, std::string_view host, ushort port,
             Handler &&handler) {
    auto address = boost::asio::ip::make_address(host);
    tcp::endpoint endpoint{address, port};
    boost::asio::co_spawn(ioc, listen(endpoint, handler),
                          boost::asio::detached);
  }

private:
  template <typename Handler>
  boost::asio::awaitable<void> listen(tcp::endpoint endpoint,
                                      Handler &&handler) {
    auto executor = co_await boost::asio::this_coro::executor;
    try {
      tcp::acceptor acceptor(executor, endpoint);
      std::cout << "Server listening on port " << endpoint.port() << std::endl;

      while (true) {
        try {
          auto client_socket =
              co_await acceptor.async_accept(boost::asio::use_awaitable);

          co_await handler(std::move(client_socket));
        } catch (boost::system::system_error se) {
          if (se.code() != boost::asio::error::connection_reset &&
              se.code() != boost::asio::error::broken_pipe) {
            throw;
          }
        }
      }
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }
};

#endif // LIB_TCP_SERVER_HPP_
