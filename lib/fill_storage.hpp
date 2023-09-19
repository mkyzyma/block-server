#ifndef LIB_FILL_STORAGE_HPP_
#define LIB_FILL_STORAGE_HPP_

#include "utils/random.hpp"
#include <iterator>
#include <ranges>
#include <string>

template <size_t MaxBlockSize, char Fill> class fill_storage {
public:
  size_t get_block_number(const std::string &hash) {
    return random<size_t>(0, SIZE_MAX);
  }
  size_t get_block_size(const std::string &hash) {
    return random<size_t>(0x01, MaxBlockSize);

    // return MaxBlockSize;
  }
  size_t get_block_data(size_t block_num, char *buffer, size_t buffer_size) {
    std::fill_n(buffer, buffer_size, Fill);

    return buffer_size;
  }
};

#endif // LIB_FILL_STORAGE_HPP_
