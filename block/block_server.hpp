#ifndef BLOCK_BLOCK_SERVER_HPP_
#define BLOCK_BLOCK_SERVER_HPP_

#include <boost/asio/awaitable.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <iostream>

template <class BaseServer> class block_server {
private:
  using tcp = boost::asio::ip::tcp;

public:
  template <typename Handler>
  void start(boost::asio::io_context &ioc, std::string_view host, ushort port,
             Handler &&handler) {
    base_.start(ioc, host, port,
                [](auto socket) -> boost::asio::awaitable<void> {
                  std::cout << "Hello, I`m block_server" << std::endl;
                  co_return;
                });
  }

private:
  BaseServer base_;
};

#endif // BLOCK_BLOCK_SERVER_HPP_
