// #define __DEBUG__
#include "lib/block_server.hpp"
#include "lib/deep_thought.hpp"
#include "lib/key_server.hpp"
#include "lib/tcp_server.hpp"
#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

struct config {
  std::string host = "0.0.0.0";
  u_short port = 1234;
  size_t threads_count = 4;
  bool exit = false;
};

config
get_config(int argc, char **argv) {
  config cfg;
  // ! Что-то не то у меня с boost::program_options (неопределённая ссылка)
  // Не успел разобраться. Хотел параметры командной строки использовать

  // po::options_description desc("Allowed options");

  // desc.add_options()("help", "produce help message")(
  //     "h", po::value<std::string>(), "set host name")(
  //     "p", po::value<u_short>(), "set port")("c", po::value<u_short>(),
  //                                            "set threads count");

  // po::variables_map vm;
  // po::store(po::parse_command_line(argc, argv, desc), vm);
  // po::notify(vm);

  // if (vm.count("help")) {
  //   std::cout << desc << std::endl;
  //   return {.exit = true};
  // }

  return cfg;
}

int
main(int argc, char **argv) {
  auto cfg = get_config(argc, argv);

  asio::io_context ioc;

  block_server<key_server<tcp_server, 128>, deep_thought<0x100000>> server;
  server.start(ioc, cfg.host, cfg.port);

  for (size_t i = 0; i < cfg.threads_count; i++) {
    std::thread([&ioc]() { ioc.run(); }).join();
  }
}
