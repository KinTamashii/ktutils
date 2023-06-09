#pragma once
#include <ktu/bit.hpp>
namespace ktu {

    namespace impl {

        template <auto fn, typename T>
        struct array_fn {
            
            template <typename ...Args>
            using array_type = typename std::array<T, sizeof...(Args)>;
            
            template <typename ...Args>
            static inline constexpr array_type<Args...> make(Args&&... args) {
                return array_type<Args...>{fn((T)args)...};
            }
        };
    };

    

    template <auto fn, typename T>
    requires (!std::is_same<T, void>::value)
    struct array_base : impl::array_fn<fn, T> {};


    template <typename T = void>
    struct array_t {
        template <typename ...Args>
        using value_type = typename std::conditional<std::is_same<T, void>::value, typename std::common_type<Args...>::type, T>::type;
        template <typename ...Args>
        using array_type = typename std::array<value_type<Args...>, sizeof...(Args)>;
        
        template <typename ...Args>
        static inline constexpr array_type<Args...> make(Args&&... args) {
            return array_type<Args...>{(value_type<Args...>)args...};
        }
    };
    
    struct array {
        template <typename T = void>
        using type = array_t<T>;
        
        template <typename ...Args>
        static inline constexpr auto make(Args&&... args) {
            return std::array{args...};
        }

        template <typename T, size_t N, size_t M>
        static constexpr auto concat(const std::array<T, N> &a, const std::array<T, M> &b) {
            std::array<T, N+M> result{};
            std::copy(a.begin(), a.end(), result.begin()+N);
            std::copy(b.begin(), b.end(), result.begin()+N);
            return result;
        }

        template <typename ...Args>
        static constexpr auto concat(Args&&... args) {
            constexpr auto size = ([&]()constexpr{return typename std::remove_reference<Args>::type().size();}() + ...);
            using array_type = typename std::remove_reference<typename ktu::variadic::first<Args...>::type>::type;
            std::array<typename array_type::value_type, size> result{};
            auto dest = result.begin();
            ([&](){std::copy(args.begin(), args.end(), dest); dest += args.size();}(), ...);
            return result;
        }

    };



    template <bool little_endian, typename T>
    struct little_endian_array : array_base<ktu::little_endian<little_endian, T>, T> {};

    template <bool big_endian, typename T>
    struct big_endian_array : array_base<ktu::big_endian<big_endian, T>, T> {};


    template <typename T, size_t N>
    inline consteval size_t array_size(T(&array)[N]) {
        return N / sizeof(T);
    }

    template <typename T, size_t N>
    inline consteval size_t array_null_size(T(&array)[N]) {
        return (N - 1) / sizeof(T);
    }

    namespace impl {
        template <typename T>
        struct array_range_t {
            T *first, *last;
            inline constexpr T *begin() {return first;}
            inline constexpr const T *begin() const {return first;}
            inline constexpr T *end() {return last;}
            inline constexpr const T *end() const {return last;}

            inline constexpr T *rbegin() {return first - 1;}
            inline constexpr const T *rbegin() const {return first - 1;}
            inline constexpr T *rend() {return last - 1;}
            inline constexpr const T *rend() const {return last - 1;}
        };
    };

    template <typename T, size_t N>
    inline consteval impl::array_range_t<T> array_range(T(&array)[N]) {
        return impl::array_range_t<T> {.first = &array[0], .last = &array[N]};
    }
    template <typename T, size_t N>
    inline consteval impl::array_range_t<T> array_null_range(T(&array)[N]) {
        return impl::array_range_t<T> {.first = &array[0], .last = &array[N-1]};
    }
    
};