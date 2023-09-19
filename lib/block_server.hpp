#ifndef LIB_BLOCK_SERVER_HPP_
#define LIB_BLOCK_SERVER_HPP_

#include "tcp_server.hpp"
#include <boost/asio/awaitable.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/write.hpp>

#include <utility>
#include <vector>

#include "utils/debug.hpp"

using buffers = std::vector<boost::asio::const_buffer>;
template <class KeyServer, class Blocks> class block_server {
public:
  void start(boost::asio::io_context &ioc, std::string_view host, ushort port) {
    key_server_.start(
        ioc, host, port,
        [this](auto &socket, auto key) -> boost::asio::awaitable<void> {
          auto block = blocks_[key];
          auto length = block.size() + key.size();
          // for (auto b : block) {
          //   std::cout << std::hex << std::setfill('0') << std::setw(2)
          //             << static_cast<u_short>(b) << " ";
          // }
          // std::cout << "|" << std::endl;

          print_bytes(block.begin(), block.end());

          buffers response = {boost::asio::buffer(&length, 4),
                              boost::asio::buffer(key),
                              boost::asio::buffer(block)};

          co_await boost::asio::async_write(socket, response,
                                            boost::asio::use_awaitable);

          co_return;
        });
  }

private:
  KeyServer key_server_;
  Blocks blocks_;
};

#endif // LIB_BLOCK_SERVER_HPP_
