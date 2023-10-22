#pragma once
#include <string_view>
#include <ktu/template.hpp>
#include <cstddef>
#include <cctype>
namespace ktu {
    
    namespace impl {
        template <bool case_insensitive = false, typename T>
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
                char c = *cptr;
                if constexpr (case_insensitive) {
                    if (std::is_constant_evaluated()) {
                        result ^= (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
                    } else {
                        result ^= std::tolower(c);
                    }
                    
                } else {
                    result ^= c;
                }
                
                result *= FNVprime;
            }
            return result;
        }

    };

    template <typename T>
    inline constexpr size_t hash(const T *ptr, size_t size) {
        if (std::is_constant_evaluated()) {
            return impl::hash<false>(ptr, size);
        } else {
            return impl::hash<false, void>(ptr, size);
        }
    }

    

    template <size_t N>
    inline constexpr size_t hash(const char (&str)[N]) {
        return hash(str, N-1);
    }

    inline size_t hash(std::string_view str) {
        return hash(str.data(), str.size());
    }



    /* Case insensitive hash. */
    template <typename T>
    inline constexpr size_t hashcase(const T *ptr, size_t size) {
        if (std::is_constant_evaluated()) {
            return impl::hash<true>(ptr, size);
        } else {
            return impl::hash<true, void>(ptr, size);
        }
    }

    
    /* Case insensitive hash. */
    template <size_t N>
    inline constexpr size_t hashcase(const char (&str)[N]) {
        return hashcase(str, N-1);
    }

    /* Case insensitive hash. */
    inline size_t hashcase(std::string_view str) {
        return hashcase(str.data(), str.size());
    }
};