#include <boost/asio.hpp>
#include <iostream>
#include <ranges>
#include <vector>

#include "utils/random.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

using namespace std::chrono_literals; // NOLINT

static const std::size_t KEY_LENGTH = 8;

using bytes = std::vector<u_char>;

auto rnd_byte = []() -> u_char { return rnd(u_char{0x00}, u_char{0xFF}); };

bytes
generate_request(size_t keys_num) {
  size_t payload_length = keys_num * KEY_LENGTH;

  bytes request(payload_length);

  std::ranges::generate_n(request.begin(), payload_length, rnd_byte);
  return request;
}

asio::awaitable<void>
request(std::string_view host, u_short port, bytes &&request) {
  try {
    tcp::socket socket{co_await asio::this_coro::executor};

    co_await socket.async_connect(
        tcp::endpoint{asio::ip::make_address(host), port},
        boost::asio::use_awaitable);

    co_await asio::async_write(socket, asio::buffer(request),
                               asio::use_awaitable);
  } catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl;
  }
}

asio::awaitable<void>
run(std::string_view host, u_short port) {
  auto exec = co_await asio::this_coro::executor;
  auto t = asio::steady_timer{exec};
  while (true) {
    t.expires_after(2s);
    co_await t.async_wait(asio::use_awaitable);

    auto payload = generate_request(4);
    co_await request(host, port, std::move(payload));
  }
}

int
main() {
  asio::io_context ioc;
  asio::co_spawn(ioc, run("127.0.0.1", 1234), asio::detached);
  ioc.run();
}
