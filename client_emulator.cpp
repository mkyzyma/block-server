#include <boost/asio.hpp>
#include <iostream>
#include <ranges>
#include <vector>

#define __DEBUG__
#include "utils/debug.hpp"
#include "utils/random.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

using namespace std::chrono_literals; // NOLINT

static const size_t KEY_LENGTH = 128;
static const size_t MAX_KEYS_NUM = 10000;
static const size_t SIZE_LENGTH = 6;

using bytes = std::vector<u_char>;

auto random_char = []() -> char { return random(0x00, 0x7F); };

bytes
generate_request(size_t keys_num) {
  size_t payload_length = keys_num * KEY_LENGTH;

  bytes request(payload_length);

  std::ranges::generate_n(request.begin(), payload_length, random_char);
  return request;
}

asio::awaitable<bytes>
consume(tcp::socket &socket, asio::streambuf &buffer, size_t desired_length) {
  asio::streambuf::mutable_buffers_type bufs = buffer.prepare(desired_length);

  size_t length = 0;

  while (length < desired_length) {
    length += co_await asio::async_read(socket, bufs, asio::use_awaitable);
  }

  auto ptr = asio::buffer_cast<const char *>(buffer.data());
  buffer.consume(desired_length);

  co_return bytes{ptr, ptr + desired_length};
}

asio::awaitable<void>
request(std::string_view host, u_short port, size_t keys_num) {
  try {
    auto payload = generate_request(keys_num);
    tcp::socket socket{co_await asio::this_coro::executor};

    co_await socket.async_connect(
        tcp::endpoint{asio::ip::make_address(host), port}, asio::use_awaitable);

    co_await asio::async_write(socket, asio::buffer(payload),
                               asio::use_awaitable);

    asio::streambuf buffer;

    for (size_t block_index = 0; block_index < keys_num; ++block_index) {
      bytes size_bytes = co_await consume(socket, buffer, sizeof(int32_t));
      size_t length = *reinterpret_cast<int32_t *>(size_bytes.data());

      std::cout << "Length bytes: ";
      print_bytes(size_bytes.begin(), size_bytes.end());
      std::cout << "Length: " << length << std::endl;

      bytes payload = co_await consume(socket, buffer, length);

      bytes key{payload.begin(), payload.begin() + KEY_LENGTH};
      std::cout << "Key: ";

      print_bytes(key.begin(), key.end());
      std::cout << std::endl;
    }
  } catch (std::exception &ex) {
    std::cerr << ex.what() << std::endl;
  }
}

asio::awaitable<void>
run(std::string_view host, u_short port) {
  auto exec = co_await asio::this_coro::executor;
  auto t = asio::steady_timer{exec};
  while (true) {
    // t.expires_after(2s);
    // co_await t.async_wait(asio::use_awaitable);

    size_t keys_num = random<size_t>(0, MAX_KEYS_NUM);

    co_await request(host, port, keys_num);
  }
}

int
main() {
  asio::io_context ioc;
  asio::co_spawn(ioc, run("127.0.0.1", 1234), asio::detached);
  ioc.run();
}
