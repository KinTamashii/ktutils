#pragma once
#include <algorithm>

namespace ktu {
    template <typename T>
    struct is_random_access : std::is_same<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category> {};
    
    template <typename T, bool forward>
    class contiguous_iterator {
        
        public:
            using value_type = T;
            using element_type = T;
            using pointer = T*;
            using reference = T&;
            using iterator_category = std::contiguous_iterator_tag;
            using difference_type = ptrdiff_t;
            /*
            
            */
            
            constexpr contiguous_iterator(pointer ptr) : ptr(ptr) {}
            constexpr pointer to_ptr() {return ptr;}
            // template <typename U, bool forward_condition>
            // contiguous_iterator(const contiguous_iterator<U, forward_condition> &other) : ptr(&*other) {}
            template <typename U, bool forward_condition>
            constexpr operator contiguous_iterator<U, forward_condition>() {
                return contiguous_iterator<U, forward_condition>(ptr);
            }
            constexpr pointer operator->() const {return ptr;}
            constexpr reference operator*() const {return *ptr;}
            constexpr reference operator[](difference_type offset) const {return *(*this + offset);}
            constexpr contiguous_iterator& operator++() {
                if constexpr (forward) {
                    ptr++;
                } else {
                    ptr--;
                }
                return *this;
            }
            constexpr contiguous_iterator operator++(int) {
                contiguous_iterator result = *this; ++(*this); return result;
            }
            constexpr contiguous_iterator& operator--() {
                if constexpr (forward) {
                    ptr--;
                } else {
                    ptr++;
                }
                return *this;
            }
            constexpr contiguous_iterator operator--(int) {
                contiguous_iterator result = *this; --(*this); return result;
            }

            constexpr difference_type operator+(const contiguous_iterator& other) const {
                if constexpr (forward) {
                    return ptr + other.ptr;
                } else {
                    return ptr - other.ptr;
                }
            }
            constexpr difference_type operator-(const contiguous_iterator& other) const {
                if constexpr (forward) {
                    return ptr - other.ptr;
                } else {
                    return ptr + other.ptr;
                }
            }

            constexpr contiguous_iterator operator+(const size_t& diff) const {
                if constexpr (forward) {
                    return contiguous_iterator(ptr + diff);
                } else {
                    return contiguous_iterator(ptr - diff);
                }
            }
            constexpr contiguous_iterator operator-(const size_t& diff) const {
                if constexpr (forward) {
                    return contiguous_iterator(ptr - diff);
                } else {
                    return contiguous_iterator(ptr + diff);
                }
            }
            constexpr contiguous_iterator& operator+=(const size_t& diff) {
                if constexpr (forward) {
                    ptr += diff;
                } else {
                    ptr -= diff;
                }
                return *this;
            }
            constexpr contiguous_iterator& operator-=(const size_t& diff) {
                if constexpr (forward) {
                    ptr -= diff;
                } else {
                    ptr += diff;
                }
                return *this;
            }
            constexpr auto operator<=>(const contiguous_iterator&) const = default;
            constexpr friend contiguous_iterator operator+(size_t value, const contiguous_iterator& other) {
                if constexpr (forward) {
                    return other + value;
                } else {
                    return other - value;
                }
            }
        private:
            pointer ptr;
    };

    template <typename InputIt>
    class range {
        private:
            InputIt first, last;
        public:
            constexpr range(const InputIt &first, const InputIt &last) : first(first), last(last) {}
            constexpr inline InputIt begin() {return first;}
            constexpr inline InputIt end() {return last;}
    };
};