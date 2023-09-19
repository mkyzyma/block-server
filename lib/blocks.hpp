#ifndef LIB_BLOCKS_HPP_
#define LIB_BLOCKS_HPP_

#include <string>
#include <vector>

using bytes = std::vector<char>;

template <class BlockStorage> class blocks {
public:
  bytes operator[](std::string_view key) {
    size_t block_number = storage_.get_block_number(key.data());
    size_t block_size = storage_.get_block_size(key.data());

    bytes block(block_size);

    storage_.get_block_data(block_number, block.data(), block_size);

    return block;
  }

private:
  BlockStorage storage_;
};

#endif // LIB_BLOCKS_HPP_
