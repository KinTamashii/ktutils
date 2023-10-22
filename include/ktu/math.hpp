#pragma once
#include <cstdint>
#include <concepts>

namespace ktu {

    template <std::integral T>
    constexpr T pow(T base, uint8_t exponent) {
        T result = 1;
        while (true) {
            if (exponent & 1)
                result *= base;
            exponent >>= 1;
            if (!exponent)
                break;
            base *= base;
        }
        return result;
    }
};