#ifndef UTILS_RANDOM_HPP_
#define UTILS_RANDOM_HPP_
#include <random>

template <typename IntType = int>
inline IntType
rnd(IntType start, IntType end) {
  std::random_device rd;
  auto gen = std::default_random_engine(rd());
  auto dst = std::uniform_int_distribution<IntType>(start, end);
  return dst(gen);
}

#endif // UTILS_RANDOM_HPP_
