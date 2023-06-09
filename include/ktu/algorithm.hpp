#pragma once
#include <string>
#include <ktu/template.hpp>
#include <cstddef>
namespace ktu {
    
    namespace impl {
        template <typename T>
        constexpr size_t hash(const T *ptr, size_t size) {

            constexpr size_t FNVoffsetBasis = std::conditional<
                sizeof(size_t) == 8, 
                std::integral_constant<size_t, 0xcbf29ce484222325>,
                std::integral_constant<size_t, 0x811c9dc5>
            >::type::value;
            constexpr size_t FNVprime = std::conditional<
                sizeof(size_t) == 8, 
                std::integral_constant<size_t, 0x00000100000001B3>,
                std::integral_constant<size_t, 0x01000193>
            >::type::value;
            
            size_t result = FNVoffsetBasis;
            using V = typename std::conditional<std::is_same<T,void>::value, uint8_t, T>::type;
            for (const V * cptr = (const V *)ptr, * const end = cptr + size; cptr != end; cptr++) {
                result ^= *cptr;
                result *= FNVprime;
            }
            return result;
        }
    };

    template <typename T>
    inline constexpr size_t hash(const T *ptr, size_t size) {
        if (std::is_constant_evaluated()) {
            return impl::hash(ptr, size);
        } else {
            return impl::hash<void>(ptr, size);
        }
    }

    

    template <size_t N>
    inline constexpr size_t hash(const char (&str)[N]) {
        return hash(str, N-1);
    }

    inline size_t hash(const std::string &str) {
        return hash(str.data(), str.size());
    }
};