#pragma once
#include <cstdint>
#include <concepts>
namespace ktu {

    constexpr uint32_t ston32(const char *str) {
        uint32_t value = 0;
        bool negative;
        if ((negative = (*str == '-'))) {
            str++;
        }
        if (*str == '0') {
            ++str;
            if ('0' <= *str && *str <= '7') {
                do {
                    value <<= 3;
                    value += (*str++) - '0';
                } while ('0' <= *str && *str <= '7');
            } else if (*str == 'b') {
                ++str;
                while (true) {
                    
                    switch (*str++) {
                        case '0':
                            value <<= 1;
                            break;
                        case '1':
                            value <<= 1;
                            value++;
                            break;
                        default:
                            return (negative) ? -value : value;
                    }
                    
                }
            } else if (*str == 'x') {
                ++str;
                while (true) {
                    
                    if ('0' <= *str && *str <= '9') {
                        value <<= 4;
                        value += (*str) - '0';
                    } else if ('A' <= *str && *str <= 'F') {
                        value <<= 4;
                        value += (*str) - 0x37;
                    } else if ('a' <= *str && *str <= 'f') {
                        value <<= 4;
                        value += (*str) - 0x57;
                    } else return (negative) ? -value : value;
                    ++str;
                }
            }
        } else if ('1' <= *str && *str <= '9') {
            do {
                value *= 10;
                value += (*str++) - '0';
            } while ('0' <= *str && *str <= '9');
        }
        return (negative) ? -value : value;
    }

    constexpr uint64_t ston64(const char *str) {
        uint64_t value = 0;
        bool negative;
        if ((negative = (*str == '-'))) {
            str++;
        }
        if (*str == '0') {
            ++str;
            if ('0' <= *str && *str <= '7') {
                do {
                    value <<= 3;
                    value += (*str++) - '0';
                } while ('0' <= *str && *str <= '7');
            } else if (*str == 'b') {
                ++str;
                while (true) {
                    
                    switch (*str++) {
                        case '0':
                            value <<= 1;
                            break;
                        case '1':
                            value <<= 1;
                            value++;
                            break;
                        default:
                            return (negative) ? -value : value;
                    }
                    
                }
            } else if (*str == 'x') {
                ++str;
                while (true) {
                    
                    if ('0' <= *str && *str <= '9') {
                        value <<= 4;
                        value += (*str) - '0';
                    } else if ('A' <= *str && *str <= 'F') {
                        value <<= 4;
                        value += (*str) - 0x37;
                    } else if ('a' <= *str && *str <= 'f') {
                        value <<= 4;
                        value += (*str) - 0x57;
                    } else return (negative) ? -value : value;
                    ++str;
                }
            }
        } else if ('1' <= *str && *str <= '9') {
            do {
                value *= 10;
                value += (*str++) - '0';
            } while ('0' <= *str && *str <= '9');
        }
        return (negative) ? -value : value;
    }

    template <std::integral T = size_t>
    inline constexpr T ston(const char *str) {
        if constexpr (sizeof(T) <= 4) {
            return ston32(str);
        } else {
            return ston64(str);
        }
    }
};

