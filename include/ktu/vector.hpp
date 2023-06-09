#pragma once

#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <iterator>
namespace ktu {
     template <auto fn, typename ...Args>
    static constexpr size_t vector_function_size(Args&&... args) {
        auto V = fn(args...);
        return V.size();
    }
    template <auto vector_function, auto ...Args>
    static constexpr auto vector_create_array() {
        std::array<typename decltype(vector_function(Args...))::value_type, vector_function_size<vector_function>(Args...)> A{};
        auto V = vector_function(Args...);
        std::copy(V.begin(), V.end(), A.begin());
        return A;
    }
    template <typename T>
    class vector {
        public:

            
            using value_type = T;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const pointer;

            
            using iterator = contiguous_iterator<value_type, true>;
            using const_iterator = const contiguous_iterator<value_type, true>;

            using reverse_iterator = contiguous_iterator<value_type, false>;
            using const_reverse_iterator = const contiguous_iterator<value_type, false>;

            /* Constructor */
            constexpr vector() noexcept {}
            constexpr vector(size_type count, const value_type& value) noexcept {
                assign(count, value);
            }
            constexpr vector(size_type count) {
                resize(count);
            }
            template<class InputIt >
            constexpr vector(InputIt first, InputIt last) {
                assign(first, last);
            }

            constexpr vector(const vector& other) {
                assign(other.begin(), other.end());
            }

            constexpr vector(vector&& other) noexcept {
                priv = other.priv;
                other.priv.data = nullptr;
                other.priv.size = 0;
                other.priv.capacity = 0;
            }

            constexpr vector(std::initializer_list<value_type> init) {
                assign(init);
            }

            /* Destructor */
            constexpr ~vector() {
                if (priv.data)
                    delete[] priv.data;
            }

            /* Operator = */
            constexpr vector &operator=(const vector& other) {
                assign(other.begin(), other.end());
                return *this;
            }
            constexpr vector &operator=(vector&& other ) noexcept {
                priv = other.priv;
                other.priv.data = nullptr;
                other.priv.size = 0;
                other.priv.capacity = 0;
                return *this;
            }
            constexpr vector &operator=(std::initializer_list<value_type> ilist ) {
                assign(ilist);
                return *this;
            }


            constexpr void assign( size_type count, const T& value ) {
                resize(count, value);
            }

            template< class InputIt >
            constexpr void assign(InputIt first, InputIt last) {
                
                resize(std::distance(first, last));
                pointer d_it = priv.data;
                for (auto it = first; it != last; it++, d_it++) {
                    *d_it = *it;
                }
            }

            constexpr void assign(std::initializer_list<T> ilist) {
                assign(ilist.begin(), ilist.end());
            }

            /* Element Access */
            constexpr reference at(size_type pos) {
                if (pos >= priv.size)
                    throw std::out_of_range("Out of range!");
                return priv.data[pos];
            }
            constexpr const_reference at(size_type pos) const {
                if (pos >= priv.size)
                    throw std::out_of_range("Out of range!");
                return priv.data[pos];
            }

            constexpr reference operator[](size_type pos) {
                return priv.data[pos];
            }
            constexpr const_reference operator[](size_type pos) const {
                return priv.data[pos];
            }

            constexpr reference front() {
                return *priv.data;
            }
            constexpr const_reference front() const {
                return *priv.data;
            }

            constexpr reference back() {
                return priv.data[priv.size-1];
            }
            constexpr const_reference back() const {
                return priv.data[priv.size-1];
            }

            constexpr pointer data() noexcept {
                return priv.data;
            }
            constexpr const_pointer data() const noexcept {
                return priv.data;
            }


            /* Iterators */
            constexpr iterator begin() {
                return iterator(priv.data);
            }
            constexpr const_iterator begin() const {
                return const_iterator(priv.data);
            }

            constexpr iterator end() {
                return iterator(priv.data+priv.size);
            }
            constexpr const_iterator end() const {
                return const_iterator(priv.data+priv.size);
            }


            constexpr reverse_iterator rbegin() {
                return reverse_iterator(priv.data+priv.size-1);
            }
            constexpr const_reverse_iterator rbegin() const {
                return const_reverse_iterator(priv.data+priv.size-1);
            }

            constexpr reverse_iterator rend() {
                return reverse_iterator(priv.data-1);
            }
            constexpr const_reverse_iterator rend() const {
                return const_reverse_iterator(priv.data-1);
            }


            /* Capacity */
            constexpr bool empty() const noexcept {
                return !priv.size;
            }
            constexpr size_type size() const noexcept {
                return priv.size;
            }
            constexpr size_type max_size() const noexcept {
                return std::numeric_limits<size_type>::max() / sizeof(value_type);
            }
            constexpr inline void reallocate(size_type capacity) {
                
                priv.capacity = capacity;
                auto new_data = new value_type[capacity];
                std::swap(priv.data, new_data);
                if (new_data) {
                    std::copy(new_data, new_data+priv.size, priv.data);
                    delete[] new_data;
                }
                
            }
            constexpr inline void increase_space(size_type count) {
                size_type new_size = priv.size + count;
                if (new_size > priv.capacity) {
                    reallocate(std::max((priv.size) * 2, new_size));
                }
                priv.size = new_size;
            }
            constexpr void reserve(size_type capacity) {
                if (capacity <= priv.capacity)
                    return;

                reallocate(capacity);
            }

            constexpr size_type capacity() const noexcept {
                return priv.capacity;
            }

            

            constexpr void shrink_to_fit() {
                if (priv.capacity <= priv.size)
                    return;
                reallocate(priv.size);
            }

            /* Modifiers */
            constexpr void clear() noexcept {
                priv.size = 0;
            }

            constexpr void shift(const_iterator pos, size_type count) {
                size_type index = std::distance(begin(), pos);
                increase_space(count);
                std::copy(priv.data+index, priv.data + priv.size - count, priv.data+index + count);
            }
            constexpr iterator insert(const_iterator pos, const value_type &value) {
                shift(pos, 1);
                priv.data[pos] = value;
                return iterator(priv.data + std::distance(begin(), pos));
            }
            constexpr iterator insert(const_iterator pos, value_type &&value) {
                size_type index = std::distance(begin(), pos);
                shift(pos, 1);
                priv.data[index] = std::move(value);
                return iterator(priv.data + index);
            }
            constexpr iterator insert(const_iterator pos, size_type count, const value_type &value) {
                shift(pos, count);
                for (pointer it = priv.data + pos, end = priv.data + pos + count; it != end; it++) {
                    *it = value;
                }
                return iterator(priv.data + std::distance(begin(), pos));
            }
            template <class InputIt>
            constexpr iterator insert(const_iterator pos, InputIt first, InputIt last) {
                size_type
                    count = std::distance(first, last),
                    index = std::distance(begin(), pos);
                shift(pos, count);

                InputIt it = first;
                pointer d_it = priv.data + index;
                while (it != last) {
                    *d_it++ = *it++;
                }
                return iterator(priv.data + index);
            }
            constexpr iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
                return insert(pos, ilist.begin(), ilist.end());
            }

            template< class... Args >
            constexpr iterator emplace( const_iterator pos, Args&&... args ) {
                size_type index = std::distance(priv.data, pos);
                shift(pos, sizeof...(Args));
                pointer d_it = priv.data + index;
                for (auto &&arg : {args...}) {
                    *d_it++ = std::move(arg);
                }
                return iterator(priv.data + index);
            }

            constexpr iterator erase(const_iterator pos, size_type count = 1) {
                priv.size -= count;
                std::copy(priv.data + pos + count, priv.data + priv.size - pos, priv.data + pos);
                return iterator (priv.data + std::distance(priv.data, pos));
            }
            constexpr iterator erase(const_iterator first, const_iterator last) {
                return erase(first, std::distance(first, last));
            }
            constexpr void push_back(const value_type &value) {
                size_type index = priv.size;
                increase_space(1);
                priv.data[index] = value;
            }
            constexpr void push_back(value_type &&value) {
                size_type index = priv.size;
                increase_space(1);
                priv.data[index] = std::move(value);
            }
            template< class... Args >
            constexpr reference emplace_back( Args&&... args ) {
                size_type index = priv.size;
                increase_space(sizeof...(args));
                pointer d_it = priv.data + index;
                for (auto &&arg : {args...}) {
                    *d_it++ = std::move(arg);
                }
                return priv.data[priv.size+sizeof...(args)-1];
            }
            constexpr inline void pop_back() {
                priv.size--;
            }

            constexpr inline void resize(size_type size) {
                reserve(priv.size = size);
            }
            constexpr inline void resize(size_type size, const value_type &value) {
                resize(size);
                for (pointer it = priv.data+priv.size, end = priv.data+priv.capacity; it != end; it++) {
                    *it = value;
                }
            }
            constexpr void swap(vector& other) noexcept {
                std::swap(priv, other.priv);
            }
        private:
            struct {
                pointer data = nullptr;
                size_type size = 0;
                size_type capacity = 0;
            } priv;
    };
};
