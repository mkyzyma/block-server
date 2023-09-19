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
          return handle_request(socket, key);
        });
  }

private:
  boost::asio::awaitable<void> handle_request(auto &socket, auto key) {

    auto block = blocks_[key];
    auto length = block.size() + key.size();

    print_bytes(block.begin(), block.end());

    buffers response = {boost::asio::buffer(&length, sizeof(int32_t)),
                        boost::asio::buffer(key), boost::asio::buffer(block)};

    co_await boost::asio::async_write(socket, response,
                                      boost::asio::use_awaitable);
  }

private:
  KeyServer key_server_;
  Blocks blocks_;
};

#endif // LIB_BLOCK_SERVER_HPP_
