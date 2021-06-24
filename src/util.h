//
// Created by huhua on 2021/6/22.
//

#ifndef NES_UTIL_H
#define NES_UTIL_H
#include <string>
#include <cstdint>

std::string hex(uint32_t n, uint8_t d);

// you are not work??
template <typename T>
bool in(T val, T min, T max) {
    return val >= min && val <= max;
}
#endif //NES_UTIL_H
