#ifndef LIB_BLOCKS_HPP_
#define LIB_BLOCKS_HPP_

#include <vector>

template <class block_storage> class blocks {
public:
  std::vector<u_char> operator[](std::vector<u_char> &&key) { return key; }
};

#endif // LIB_BLOCKS_HPP_
