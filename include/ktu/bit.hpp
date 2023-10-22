#pragma once
#include "template.hpp"
#include <bit>
namespace ktu {
    namespace impl {
        template <arithmetic<2> T>
        inline constexpr T byteswap(T value) {
            return 
                ((value&0xFF00) >> 8) |
                ((value&0xFF) << 8);
        }
        template <arithmetic<4> T>
        inline constexpr T byteswap(T value) {
            return (
                    (((value) & 0xFF000000) >> 24)|
                    (((value) & 0x00FF0000) >>  8) |
                    (((value) & 0x0000FF00) <<  8) |
                    (((value) & 0x000000FF) << 24)
                );
        }
        template <arithmetic<8> T>
        inline constexpr T byteswap(T value) {
            return (
                    (((value) & 0xFF00000000000000) >> 56)|
                    (((value) & 0x00FF000000000000) >> 40) |
                    (((value) & 0x0000FF0000000000) >> 24) |
                    (((value) & 0x000000FF00000000) >> 8) |
                    (((value) & 0x00000000FF000000) << 8)|
                    (((value) & 0x0000000000FF0000) << 24) |
                    (((value) & 0x000000000000FF00) << 40) |
                    (((value) & 0x00000000000000FF) << 56)
                );
        }
        template <typename T>
        inline constexpr auto byteswap(T value) {
            if (std::is_constant_evaluated()) {
                throw "Invalid type for constexpr byteswap.";
            } else {
                return T();
            }
            
        }
        
    };

    /* Reverses the byte order of any type. If used in constexpr, only works on numeric types.
        The condition is handled at compile-time.
    */
    template <bool condition = true, typename T>
    inline constexpr T byteswap(T value) {
        if constexpr (sizeof(T) == 1 || !condition)
            return value;

        if (std::is_constant_evaluated()) {
            if constexpr (std::is_integral<T>::value) {
                return impl::byteswap(value);
            } else {
                return T();
            }
            
        } else {
            if (sizeof(T) == 2) {
                uint16_t temp = impl::byteswap(*(uint16_t*)&value);
                return *(T*)&temp;
            } else if (sizeof(T) == 4) {
                uint32_t temp = impl::byteswap(*(uint32_t*)&value);
                return *(T*)&temp;
            } else if (sizeof(T) == 8) {
                uint64_t temp = impl::byteswap(*(uint64_t*)&value);
                return *(T*)&temp;
            } else {
                uint8_t *b = (uint8_t*)&value;
                ::std::reverse(b, b+sizeof(value));
                return value;
            };
        }
    }
    
    /* Reverses the byte order of any type. If used in constexpr, only works on numeric types.
        The condition is handled at runtime.
    */
    template <typename T>
    inline constexpr T byteswap(T value, bool condition) {
        return (condition) ? byteswap(value) : value;
    }

    /* Set the endianness of a type to little-endian. */
    template <typename T>
    inline constexpr T little_endian(T value) {
        return byteswap<std::endian::native == std::endian::big>(value);
    }

    /* Set the endianness of a type to little-endian. 
        The condition is handled at compile-time.
    */
    template <bool condition, typename T>
    inline constexpr T little_endian(T value) {
        return byteswap<condition && std::endian::native == std::endian::big>(value);
    }

    /* Set the endianness of a type to little-endian. 
        The condition is handled at runtime.
    */
    template <typename T>
    inline constexpr T little_endian(T value, bool condition) {
        return (condition) ? little_endian<T>(value) : value;
    }

    /* Set the endianness of a type to big-endian. */
    template <typename T>
    inline constexpr T big_endian(T value) {
        return byteswap<std::endian::native == std::endian::little>(value);
    }

    /* Set the endianness of a type to big-endian. 
        The condition is handled at compile-time.
    */
    template <bool condition, typename T>
    inline constexpr T big_endian(T value) {
        return byteswap<condition && std::endian::native == std::endian::little>(value);
    }

    /* Set the endianness of a type to big-endian.
        The condition is handled at runtime.
    */
    template <typename T>
    inline constexpr T big_endian(T value, bool condition) {
        return (condition) ? big_endian(value) : value;
    }
    

    /* An unsigned integer type that can store N bytes. */
    template <size_t N>
    using integer_size_type = typename std::conditional<(N > 4),
        uint64_t,
        typename std::conditional<(N > 2),
            uint32_t,
            typename std::conditional<(N > 1),
                uint16_t,
                uint8_t
            >::type
        >::type
    >::type;

    /* Returns an integer value that masks N bytes. */
    constexpr uint64_t byte_mask(uint8_t N) {
        uint64_t value = (1ULL << ((N)*8ULL)) - 1ULL;
        if constexpr (std::endian::native == std::endian::big) {
            uint8_t typesize = N;
            typesize--; typesize |= typesize >> 1; typesize |= typesize >> 2; typesize++;
            value <<= (((typesize)-(N))*8);
        }
        return value;
    }

    /* Mask N bytes at a location in memory, returning an appropiately sized type. */
    template <uint8_t N, typename T = ktu::integer_size_type<N>>
    requires (std::is_integral<T>::value)
    constexpr T byte_mask(const void *ptr) {
        return *(const T*)ptr & byte_mask(N);
    }


    /* Returns a numeric value which produces the identical set of bytes in the provided string on the target architecture. */
    template <typename T = void, size_t N>
    requires (std::is_integral<T>::value || std::is_floating_point<T>::value || std::is_same<T, void>::value)
    constexpr auto numeric_literal(const char (&str)[N]) {
        using value_type = typename std::conditional<std::is_same<T,void>::value,
            integer_size_type<N-1>,
            T
        >::type;

        value_type value = 0;
        if constexpr (sizeof(value_type) == 1) {
            if constexpr (N > 0) {
                value = str[0] & 0xFF;
            }
        } else if constexpr (std::endian::native == std::endian::big) {
            const char *ptr = &str[0];
            for (int i = 0; i < N; i++) {
                value <<= 8;
                value |= *ptr & 0xFF;
                if (i < N - 1) ptr++;
            }
        } else if constexpr (std::endian::native == std::endian::little) {
            const char *ptr = &str[N-1];
            for (int i = 0; i < N; i++) {
                value <<= 8;
                value |= *ptr & 0xFF;
                if (i < N - 1) ptr--;
            }
        }
        return value;
    }
    
    template <std::integral T>
    inline constexpr T align2(T value, unsigned power) {
        return ((value - 1ULL) | ((1ULL<<power)-1ULL)) + 1ULL;
    }


    #include <concepts>

    template <std::integral T>
    inline constexpr int highest_bit (T value) {
        return value ? (int)(sizeof(value)*8 - 1 - (std::countl_zero(static_cast<typename std::make_unsigned<T>::type>(value)))) : -1;
    }
    

    template <std::integral T = unsigned long long>
    inline constexpr T field(unsigned bit, unsigned count) {
        return (((1 << count) - 1) << bit);
    }
    template <std::integral T>
    inline constexpr T field_value(auto field, T value) {
        using U = typename std::make_unsigned<T>::type;
        return field ? value << std::countr_zero(static_cast<U>(field)) : 0;
    }
    template <typename T>
    requires (!std::is_integral<T>::value)
    inline constexpr unsigned long long field_value(auto field, T value) {
        return field_value(field, (unsigned long long)(value));
    }

    
};