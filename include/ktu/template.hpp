#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <algorithm>


namespace ktu {

    template <auto Fn, auto ...Args>
    static consteval auto static_function() {
        return Fn(Args...);
    }
    
    template <typename T> struct indirection {
        using value_type = typename std::remove_reference<T>::type;
        static constexpr unsigned value =
            std::conditional<
                std::is_pointer<value_type>::value,
                    indirection<typename std::remove_pointer<value_type>::type>,
                    std::integral_constant<unsigned, (unsigned)-1>
            >::type::value + 1;
    };

        
    template <typename T, int N>
    struct add_pointers;

    template <typename T>
    struct container_type {
        container_type() {}
        using type = T;
    };




    template <typename T, int N>
    struct add_pointers {
        private:
            template <typename T2, int N2>
            static constexpr auto add_ptr() {
                if constexpr (N2 > 0) {
                    return add_ptr<typename std::add_pointer<T2>::type, N2-1>();
                } else {
                    return container_type<T2>();
                }
            }
        public:
            using type = typename decltype(add_ptr<T, N>())::type;
    };


    template <typename T> struct remove_all_pointers {
        using type = T;
    };
    template <typename T> struct remove_all_pointers<T*> {
        using type = typename remove_all_pointers<T>::type;
    };
    template <typename T> struct remove_all_pointers<T&> {
        using type = typename remove_all_pointers<T>::type;
    };

    

    template <typename T, typename U>
    using is_same_size = typename std::conditional<
        sizeof(T) == sizeof(U),
        std::true_type,
        std::false_type
    >::type;

    template <typename T, typename U>
    using base_is_same = std::is_same<typename remove_all_pointers<T>::type, typename remove_all_pointers<U>::type>;
    template <typename T>
    using base_is_arithmetic = std::is_arithmetic<typename remove_all_pointers<T>::type>;

    template <typename T>
    using base_size = std::integral_constant<size_t, sizeof(typename remove_all_pointers<T>::type)>;

    template <typename T, int size = 0>
    concept arithmetic = std::is_arithmetic<T>::value && (sizeof(T) == size || !size);

    template <typename T, typename U>
    concept indirect = base_is_same<T, U>::value && ((indirection<T>::value - indirection<U>::value == 1) || (indirection<T>::value - indirection<U>::value == 0));
   
    template <typename T, int size = 0>
    concept indirect_arithmetic = arithmetic<typename remove_all_pointers<T>::type, size> &&
                (indirection<T>::value == 0 ||
                indirection<T>::value == 1);


    template <typename U, typename T>
    inline constexpr auto indirect_cast(T ptr) {
        constexpr auto dif = indirection<T>::value - indirection<U>::value;
        if constexpr (dif == 0) {
            return (U)ptr;
        } else if (dif == 1) {
            return (U*)ptr;
        }
    }
    
    /*
    Dereference a pointer of arbitrary indirection a specified number of times.
    If the specified indirection is negative, the pointer with be dereferenced
    until the indirection is the absolute value of the initial specification.
    */
    template <int depth, typename T>
    inline constexpr auto &dereference(T &ptr) {
        if constexpr (depth < 0) {
            return dereference<std::max((int)(indirection<T>::value + depth), 0)>(ptr);
        } else if constexpr (depth > 0 && std::is_pointer<T>::value) {
            return dereference<depth - 1>(*ptr);
        } else {
            return ptr;
        }
    }
    


    template<typename Test, template<typename...> class Ref>
    struct is_specialization : std::false_type {};

    template<template<typename...> class Ref, typename... Args>
    struct is_specialization<Ref<Args...>, Ref>: std::true_type {};

    template <typename T>
    inline constexpr T identity_fn(T item) {return item;}

    struct empty {
        template <typename ...T>
        constexpr empty(T&&... args) {}
        inline constexpr auto &&operator +(auto &&other) { return other; }
        inline constexpr auto &&operator -(auto &&other) { return other; }
        inline constexpr auto && operator +() { return *this; }
        inline constexpr auto && operator -() { return *this; }
        inline constexpr auto &&operator *(auto &&other) { return other; }
        inline constexpr auto &&operator /(auto &&other) { return other; }
        inline constexpr auto &&operator %(auto &&other) { return other; }
        inline constexpr auto && operator ++() { return *this; }
        inline constexpr auto &&operator ++(int) { return *this; }
        inline constexpr auto && operator --() { return *this; }
        inline constexpr auto &&operator --(int) { return *this; }
        inline constexpr bool operator ==(auto &&other) { return false; }
        inline constexpr bool operator !=(auto &&other) { return true; }
        inline constexpr bool operator >(auto &&other) { return false; }
        inline constexpr bool operator <(auto &&other) { return true; }
        inline constexpr bool operator >=(auto &&other) { return false; }
        inline constexpr bool operator <=(auto &&other) { return true; }
        inline constexpr auto && operator <=>(auto &&other) {return -1;}
        inline constexpr bool operator !() { return true; }
        inline constexpr bool operator &&(auto &&other) { return false; }
        inline constexpr bool operator ||(auto &&other) { return false; }
        inline constexpr auto && operator ~() { return *this; }
        inline constexpr auto &&operator &(auto &&other) { return other; }
        inline constexpr auto &&operator |(auto &&other) { return other; }
        inline constexpr auto &&operator ^(auto &&other) { return other; }
        inline constexpr auto &&operator <<(auto &&other) { return other; }
        friend inline constexpr auto &operator<<(auto &&other, auto &&obj) {return other;}
        inline constexpr auto &&operator >>(auto &&other) { return other; }
        friend inline constexpr auto &operator>>(auto &&other, auto &&obj) {return other;}
        inline constexpr auto &&operator =(auto &&other) { return other; }
        inline constexpr auto &&operator +=(auto &&other) { return other; }
        inline constexpr auto &&operator -=(auto &&other) { return other; }
        inline constexpr auto &&operator /=(auto &&other) { return other; }
        inline constexpr auto &&operator %=(auto &&other) { return other; }
        inline constexpr auto &&operator &=(auto &&other) { return other; }
        inline constexpr auto &&operator |=(auto &&other) { return other; }
        inline constexpr auto &&operator ^=(auto &&other) { return other; }
        inline constexpr auto &&operator <<=(auto &&other) { return other; }
        inline constexpr auto &&operator >>=(auto &&other) { return other; }
        inline constexpr auto &&operator [](auto &&other) { return other; }
        inline constexpr auto && operator *() { return *this; }
        inline constexpr auto && operator &() { return *this; }
        inline constexpr auto && operator ->() { return *this; }
        inline constexpr auto &&operator ->*(auto &&other) { return other; }
        template <typename ...T>
        inline constexpr auto operator()(T&&... args) {}
        inline constexpr auto &&operator ,(auto &&other) { return other; }
        template <typename T>
        inline constexpr operator T() {return T();}
        inline constexpr void *operator new(size_t x) noexcept {return nullptr;}
        inline constexpr void *operator new[](size_t x) noexcept {return nullptr;}
        inline constexpr void operator delete(void *a) {}
        inline constexpr void operator delete[](void *a) {}
    };

    template <bool condition, typename T>
    using discard_if = std::conditional<condition, empty, T>;






    template <int size>
    using to_unsigned_integral = typename std::conditional<
        size == 1,
            uint8_t,
            typename std::conditional<
                size == 2,
                uint16_t,
                typename std::conditional<
                    size == 4,
                    uint32_t,
                    typename std::conditional<
                        size == 8,
                        uint64_t,
                        empty
                    >::type
                >::type
            >::type
    >;

    template <int size>
    using to_signed_integral = typename std::conditional<
        size == 1,
            int8_t,
            typename std::conditional<
                size == 2,
                int16_t,
                typename std::conditional<
                    size == 4,
                    int32_t,
                    typename std::conditional<
                        size == 8,
                        int64_t,
                        empty
                    >::type
                >::type
            >::type
    >;

};

#include <ktu/variadic.hpp>

namespace ktu {


    template <typename>
    struct instantiation;
    template <template <typename ...ArgsT> class T, typename ...ArgsT>
    struct instantiation<T<ArgsT...>> {

        template <typename ...ArgsU>
        requires(variadic::types<ArgsT...>::template compare<is_same_size, ArgsU...>::value)
        static T<ArgsT...> &cast(T<ArgsU...> &item) {
            return *(T<ArgsT...>*)&item;
        }
    };
};