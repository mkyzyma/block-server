// #define __DEBUG__
#include "lib/block_server.hpp"
#include "lib/deep_thought.hpp"
#include "lib/key_server.hpp"
#include "lib/tcp_server.hpp"
#include <boost/asio.hpp>
#include <iostream>
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int
main() {
  asio::io_context ioc;

  block_server<key_server<tcp_server, 8>, deep_thought<0x100000>> server;
  server.start(ioc, "0.0.0.0", 1234);

  ioc.run();
}
