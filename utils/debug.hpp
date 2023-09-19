#ifndef UTILS_DEBUG_HPP_
#define UTILS_DEBUG_HPP_

#include <iomanip>
#include <iostream>

template <typename Iterator>
void
print_bytes(Iterator begin, Iterator end) {
#ifdef __DEBUG__
  Iterator it = begin - 1;

  while (++it < end) {
    std::cout << std::hex << std::setfill('0') << std::setw(2)
              << static_cast<u_short>(*it) << " ";
  }
  std::cout << std::endl;
#endif
}

#endif // UTILS_DEBUG_HPP_
