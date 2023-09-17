#include "lib/key_server.hpp"
#include "lib/tcp_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int
main() {
  asio::io_context ioc;

  key_server<tcp_server, 8> server;
  server.start(ioc, "0.0.0.0", 1234,
               [](auto &socket, auto key) -> asio::awaitable<void> {
                 for (auto b : key) {
                   std::cout << std::hex << std::setfill('0') << std::setw(2)
                             << static_cast<u_short>(b) << " ";
                 }
                 std::cout << "|" << std::endl;
                 co_return;
               });

  ioc.run();
}
