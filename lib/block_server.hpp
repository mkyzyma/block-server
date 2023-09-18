#ifndef LIB_BLOCK_SERVER_HPP_
#define LIB_BLOCK_SERVER_HPP_

#include "tcp_server.hpp"
#include <boost/asio/awaitable.hpp>
#include <iomanip>
#include <iostream>
#include <utility>

template <class KeyServer, class Blocks> class block_server {
public:
  void start(boost::asio::io_context &ioc, std::string_view host, ushort port) {
    key_server_.start(
        ioc, host, port,
        [this](auto &socket, auto key) -> boost::asio::awaitable<void> {
          auto block = blocks_[std::move(key)];

          for (auto b : block) {
            std::cout << std::hex << std::setfill('0') << std::setw(2)
                      << static_cast<u_short>(b) << " ";
          }
          std::cout << "|" << std::endl;

          co_return;
        });
  }

private:
  KeyServer key_server_;
  Blocks blocks_;
};

#endif // LIB_BLOCK_SERVER_HPP_
