#ifndef LIB_DEEP_THOUGHT_HPP_
#define LIB_DEEP_THOUGHT_HPP_

#include "blocks.hpp"
#include "fill_storage.hpp"

// Предоставляет блоки переменной длинны заполненные байтами со
// значением "42"
template <size_t MaxBlockSize>
class deep_thought : public blocks<fill_storage<MaxBlockSize, 42>> {};

#endif // LIB_DEEP_THOUGHT_HPP_
