#include "block/block_server.hpp"
#include "tcp/tcp_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int
main() {
  asio::io_context ioc;

  block_server<tcp_server> server;
  server.start(ioc, "0.0.0.0", 1234, [](auto socket) -> asio::awaitable<void> {
    std::cout << "Hello, I`m handler" << std::endl;
    co_return;
  });
#if 0 // tcp_server
  tcp_server server;
  server.start(ioc, "0.0.0.0", 1234,
               [](auto socket) -> asio::awaitable<void> {
                 std::cout << "Hello, I`m handler" << std::endl;
                 co_return;
               });
#endif
  ioc.run();
}
