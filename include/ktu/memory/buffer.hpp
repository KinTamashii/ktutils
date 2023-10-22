#pragma once
#include <ktu/memory/file.hpp>
#include <ktu/iterator.hpp>
#include <ktu/ios.hpp>
#include <ktu/bit.hpp>




namespace ktu {

    class reader;
    class view;

    constexpr unsigned long readBin(const char *&ptr, long size = -1);
    constexpr bool isBin(const char c);

    constexpr unsigned long readOct(const char *&ptr, long size = -1);
    constexpr bool isOct(const char c);


    constexpr unsigned long readDec(const char *&ptr, long size = -1);
    constexpr bool isDec(const char c);

    constexpr unsigned long readHex(const char *&ptr, long size = -1);
    constexpr bool isHex(const char c);



    const char *readVariable(const char *ptr);
    constexpr bool isVariableStart(const char c);

    class buffer {
        public:
            using value_type = uint8_t;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const pointer;

            template <typename T = value_type>
            using iterator = contiguous_iterator<T, true>;
            template <typename T = value_type>
            using const_iterator = contiguous_iterator<const T, true>;

            template <typename T = value_type>
            using reverse_iterator = contiguous_iterator<T, false>;
            template <typename T = value_type>
            using const_reverse_iterator = contiguous_iterator<const T, false>;

            

            buffer();
            template <typename T = value_type>
            buffer(size_type count, const T &value) {
                assign<T>(count, value);
            }
            template <typename T = value_type>
            buffer (size_type count) {
                resize<T, false>(count);
            }
            template <typename InputIt>
            buffer(InputIt first, InputIt last) {
                assign<InputIt>(first, last);
            }
            buffer(const buffer &other);
            buffer(buffer &&other) noexcept;
            template <typename T = value_type>
            buffer(std::initializer_list<T> init) {
                assign<T>(init);
            }
            buffer(const std::filesystem::path &path) {
                assign(path);
            }
            ~buffer();

            operator reader() const;
            operator view() const;

            buffer &operator=(const buffer &other);
            buffer &operator=(buffer &&other) noexcept;
            template <typename T = value_type>
            buffer &operator=(std::initializer_list<T> ilist) {
                assign<T>(ilist);
                return *this;
            }

    
            template <typename T = value_type>
            void assign(size_type count, const T &value = T()) {
                if constexpr (std::is_arithmetic<T>::value && sizeof(T) == 1) {
                    resize<T, false>(count, value);
                } else {
                    resize<T, false>(count);
                    for (T* ptr = (T*)priv.data, *endptr = ptr + count; ptr != endptr; ptr++) {
                        *ptr = value;
                    }
                }
            }
            template <typename InputIt>
            void assign(InputIt first, InputIt last) {
                using baseType = typename std::remove_const<typename std::remove_reference<decltype(*first)>::type>::type;
                size_type size = std::distance(first, last);
                if constexpr (is_random_access<InputIt>::value && std::is_trivially_copyable<baseType>::value) {
                    assign((void*)first, size * sizeof(baseType));
                } else {
                    resize<value_type, false>(size);
                    auto ptr = (baseType*)priv.data;
                    for (auto it = first; it != last; it++, ptr++)
                        *ptr = *it;
                }
            }

            void assign(void *ptr, size_type count);

            template <typename T = value_type>
            inline void assign(T *ptr, size_type count) {
                if constexpr (std::is_trivially_copyable<T>::value) {
                    assign(ptr, count * sizeof(T));
                } else {
                    size_type size = count * sizeof(T);
                    resize<value_type, false>(size);
                    for (T* dataptr = (T*)priv.data, endptr = ptr + size; ptr != endptr; ptr++, dataptr++)
                        *dataptr = *ptr;
                        
                }
            }

            template <typename T = value_type>
            inline void assign(std::initializer_list<T> ilist) {
                assign(ilist.begin(), ilist.end());
            }
            
            /* Reads the file at the provided path into the buffer.
                Returns false if the directory does not exist. */
            bool assign(const std::filesystem::path &path);


            // Element Access
            template <typename T = value_type>
            inline T &at(size_type index) {
                if ((index * sizeof(T)) > priv.size) throw std::out_of_range("Index out of range!");
                return ((T*)priv.data)[index];
            }
            template <typename T = value_type>
            inline const T &at(size_type index) const {
                if ((index * sizeof(T)) > priv.size) throw std::out_of_range("Index out of range!");
                return ((const T*)priv.data)[index];
            }


            template <typename T = value_type>
            inline T &operator[](size_type index) {
                return ((T*)priv.data)[index];
            }
            template <typename T = value_type>
            inline const T &operator[](size_type index) const {
                return ((const T*)priv.data)[index];
            }

            template <typename T = value_type>
            inline T &access(size_type index) {
                return ((T*)priv.data)[index];
            }
            template <typename T = value_type>
            inline const T &access(size_type index) const {
                return ((const T*)priv.data)[index];
            }


            template <typename T = value_type>
            inline T &front() {
                return *(T*)priv.data;
            }
            template <typename T = value_type>
            inline const T &front() const {
                return *(const T*)priv.data;
            }

            template <typename T = value_type>
            inline T &back() {
                return *(T*)(priv.data + priv.size - sizeof(T));
            }
            template <typename T = value_type>
            inline const T &back() const {
                return *(const T*)(priv.data + priv.size - sizeof(T));
            }

            template <typename T = value_type>
            inline T* data() noexcept {
                return (T*)priv.data;
            }
            template <typename T = value_type>
            inline const T* data() const noexcept {
                return (const T*)priv.data;
            }

            // Iterators
            template <typename T = value_type>
            inline iterator<T> begin() noexcept {return iterator<T>((T*)priv.data);}
            template <typename T = value_type>
            inline const_iterator<T> begin() const noexcept {return const_iterator<T>((const T*)priv.data);}
            template <typename T = value_type>
            inline const_iterator<T> cbegin() const noexcept {return const_iterator<T>((const T*)priv.data);}
            
            template <typename T = value_type>
            inline iterator<T> end() noexcept {return iterator<T>((T*)(priv.data + priv.size - priv.size % sizeof(T)));}
            template <typename T = value_type>
            inline const_iterator<T> end() const noexcept {return const_iterator<T>((const T*)(priv.data + priv.size - priv.size % sizeof(T)));}
            template <typename T = value_type>
            inline const_iterator<T> cend() const noexcept {return const_iterator<T>((const T*)(priv.data + priv.size - priv.size % sizeof(T)));}

            template <typename T = value_type>
            inline reverse_iterator<T> rbegin() noexcept {return reverse_iterator<T>(((T*)(priv.data + priv.size)) - 1);}
            template <typename T = value_type>
            inline const_reverse_iterator<T> rbegin() const noexcept {
                return const_reverse_iterator<T>(((const T*)(priv.data + priv.size)) - 1);
            }
            template <typename T = value_type>
            inline const_reverse_iterator<T> crbegin() const noexcept {
                return const_reverse_iterator<T>(((const T*)(priv.data + priv.size)) - 1);
            }
            
            template <typename T = value_type>
            inline reverse_iterator<T> rend() noexcept {
                
                return reverse_iterator<T>(((T*)(priv.data + priv.size % sizeof(T))) - 1);
            }
            template <typename T = value_type>
            inline const_reverse_iterator<T> rend() const noexcept {
                return const_reverse_iterator<T>(((const T*)(priv.data + priv.size % sizeof(T))) - 1);
            }
            template <typename T = value_type>
            inline const_reverse_iterator<T> crend() const noexcept {
                return const_reverse_iterator<T>(((const T*)(priv.data + priv.size % sizeof(T))) - 1);
            }

            template <typename T = value_type>
            inline auto range() noexcept {
                return ktu::range(begin<T>(), end<T>());
            }
            template <typename T = value_type>
            inline auto crange() const noexcept {
                return ktu::range(cbegin<T>(), cend<T>());
            }
            template <typename T = value_type>
            inline auto rrange() noexcept {
                return ktu::range(rbegin<T>(), rend<T>());
            }
            template <typename T = value_type>
            inline auto crrange() const noexcept {
                return ktu::range(crbegin<T>(), crend<T>());
            }

            // Capacity
            inline bool empty() const {return !priv.size;}
            template <typename T = value_type>
            inline size_type size() const noexcept {return priv.size / sizeof(T);}
            template <typename T = value_type>
            inline size_type max_size() const noexcept {return std::numeric_limits<size_type>::max() / sizeof(T);}

            template <bool copyContents = true>
            void reserve(size_type capacity) {
                if (capacity <= priv.capacity)
                    return;

                
                
                

                if constexpr (copyContents) {
                    // allocate(priv.data, priv.capacity, capacity);
                    priv.data = (pointer)realloc(priv.data, capacity);
                    
                    
                    // pointer newData = (pointer)malloc(capacity);
                    // debugf("%p [%lu %lu] %p\n", newData, priv.capacity, capacity, priv.data);
                    // memcpy(newData, priv.data, priv.capacity);
                    // std::swap(newData, priv.data);
                    // if (newData) free(newData);
                } else {
                    pointer newData = (pointer)malloc(capacity);
                    std::swap(newData, priv.data);
                    if (newData) free(newData);
                }
                if (!priv.data)
                    throw std::bad_alloc();
                priv.capacity = capacity;
            }
            template <typename T = value_type>
            inline size_type capacity() {return priv.capacity / sizeof(value_type);}
            void shrink_to_fit();
            

            // Modifiers
            inline void clear() noexcept {
                priv.size = 0;
            }
            void shift(size_type pos, size_type count);

            inline void insert(size_type pos, void *ptr, size_type count) {
                shift(pos, count);
                memcpy(priv.data + pos, ptr, count);
            }
            template <typename T = value_type>
            inline iterator<T> insert(iterator<typename std::add_const<T>::type> pos, void *ptr, size_type count) {
                size_t index = sizeof(T) * std::distance(begin<typename std::add_const<T>::type>(), pos);
                insert(index, ptr, count);
                return iterator<T>((T*)(priv.data + index + count));
            }
            template <typename T = value_type>
            inline void insert(size_type pos, const T &value) {
                if constexpr (std::is_trivially_copyable<T>::value) {
                    insert(pos, (void*)&value, sizeof(T));
                } else {
                    shift(pos, sizeof(T));
                    *(T*)(priv.data + pos) = value;
                }
            }
            template <typename T = value_type>
            inline void insert(size_type pos, T &&value) {
                insert(pos, (void*)&value, sizeof(T));
            }
            template <typename T = value_type, typename U = value_type>
            requires (!is_specialization<U, std::initializer_list>::value)
            inline iterator<T> insert(iterator<typename std::add_const<T>::type> pos, const U &value) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), pos);
                insert(index, value);
                return iterator<T>((T*)(priv.data + index + sizeof(U)));
            }
            template <typename T = value_type, typename U = value_type>
            requires (!is_specialization<U, std::initializer_list>::value)
            inline iterator<T> insert(iterator<typename std::add_const<T>::type> pos, U &&value) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), pos);
                insert(index, (void*)&value, sizeof(U));
                return iterator<T>((T*)(priv.data + index + sizeof(U)));
            }
            
            template <typename T = value_type>
            inline void insert(size_type pos, size_type count, const T &value) {
                shift(pos, count * sizeof(T));
                if constexpr (std::is_arithmetic<T>::value && sizeof(T) == 1) {
                    memset(priv.data + pos, value, count);
                } else {
                    for (pointer ptr = priv.data + pos, endptr = priv.data + pos + count * sizeof(T); ptr != endptr; ptr++) {
                        *ptr = value;
                    }
                }
            }
            template <typename T = value_type, typename U = value_type>
            inline iterator<T> insert(iterator<typename std::add_const<T>::type> pos, size_type count, const U &value) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), pos);
                insert(index, count, value);
                return iterator<T>((T*)(priv.data + index + count * sizeof(U)));
            }

            template <class InputIt>
            void insert(size_type pos, InputIt first, InputIt last) {
                using baseType = typename std::remove_reference<decltype(*first)>::type;
                size_type count = std::distance(first, last) * sizeof(baseType);
                if constexpr (is_random_access<InputIt>::value && std::is_trivially_copyable<baseType>::value) {
                    insert(pos, (void*)first, count);
                } else {
                    shift(pos, count);
                    pointer ptr = priv.data + pos;
                    for (auto &item : ktu::range(first, last)) {
                        *ptr++ = item;
                    }
                }
            }
            template <typename T = value_type, class InputIt>
            inline iterator<T> insert(iterator<typename std::add_const<T>::type> pos, InputIt first, InputIt last) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), pos);
                using baseType = typename std::remove_reference<decltype(*first)>::type;
                size_type count = std::distance(first, last) * sizeof(baseType);
                if constexpr (is_random_access<InputIt>::value && std::is_trivially_copyable<baseType>::value) {
                    insert(index, (void*)first, count);
                } else {
                    shift(index, count);
                    pointer ptr = priv.data + index;
                    for (auto &item : ktu::range(first, last)) {
                        *ptr++ = item;
                    }
                }
                return iterator<T>((T*)(priv.data + index + count * sizeof(baseType)));
            }

            template <typename T = value_type>
            inline void insert(size_type pos, std::initializer_list<T> ilist) {
                insert(pos, ilist.begin(), ilist.end());
            }
            
            template <typename T = value_type, typename U = value_type>
            inline iterator<T> insert(iterator<typename std::add_const<T>::type> pos, std::initializer_list<U> ilist) {
                return insert<T>(pos, ilist.begin(), ilist.end());
            }
            
            template <typename T = value_type, typename ...Args>
            void emplace(size_type pos, Args&&... args) {
                shift(pos, sizeof(T));
                new(priv.data + pos) T(args...);
            }
            template <typename T = value_type, typename U = value_type, typename ...Args>
            iterator<T> emplace(iterator<typename std::add_const<T>::type> pos, Args&&... args) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), pos);
                shift(index, sizeof(U));
                new(priv.data + index) T(args...);
                return iterator<T>((T*)(priv.data + index + sizeof(U)));
            }

            void erase(size_type pos, size_type count);

            template <typename T = value_type>
            inline iterator<T> erase(iterator<typename std::add_const<T>::type> pos) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), pos);
                //using baseType = typename std::remove_reference<decltype(*first)>::type;
                //size_type count = std::distance(first, last) * sizeof(baseType);
                erase(index, sizeof(T));
                return iterator<T>((T*)(priv.data + index));
            }
            template <typename T = value_type>
            inline iterator<T> erase(iterator<typename std::add_const<T>::type> first, iterator<typename std::add_const<T>::type> last) {
                size_t index = sizeof(T) * std::distance(cbegin<T>(), first);
                using baseType = typename std::remove_reference<decltype(*first)>::type;
                size_type count = std::distance(first, last) * sizeof(baseType);
                erase(index, count);
                return iterator<T>((T*)(priv.data + index));
            }



            void push_back(void *ptr, size_type count);

            template <typename T = value_type>
            inline void push_back(const T &item) {
                if constexpr (std::is_trivially_copyable<T>::value) {
                    push_back((void*)&item, sizeof(T));
                } else {
                    size_type index = priv.size, newSize = index + sizeof(T);
                    if (newSize > priv.capacity)
                        reserve(newSize * 2);
                    priv.size = newSize;
                    *(T*)(priv.data + index) = item;
                }
                
            }
            template <typename T = value_type>
            inline void push_back_little_endian(const T&item) {
                push_back<T>(ktu::little_endian<T>(item));
            }
            template <bool little_endian, typename T = value_type>
            inline void push_back_little_endian(const T&item) {
                push_back<T>(ktu::little_endian<little_endian, T>(item));
            }
            template <typename T = value_type>
            inline void push_back_little_endian(const T&item, bool littleEndian) {
                push_back<T>(ktu::little_endian<T>(item, littleEndian));
            }
            template <typename T = value_type>
            inline void push_back_big_endian(const T&item) {
                push_back<T>(ktu::big_endian<T>(item));
            }
            template <bool big_endian, typename T = value_type>
            inline void push_back_big_endian(const T&item) {
                push_back<T>(ktu::big_endian<big_endian, T>(item));
            }
            template <typename T = value_type>
            inline void push_back_big_endian(const T&item, bool bigEndian) {
                push_back<T>(ktu::big_endian<T>(item, bigEndian));
            }


            template <typename T = value_type, typename ...Args>
            void emplace_back(Args&& ...args) {
                size_type index = priv.size, newSize = index + sizeof(T);
                if (newSize > priv.capacity)
                    reserve(std::max(priv.size * 2, newSize));
                priv.size = newSize;
                new(priv.data + index) T(args...);
            }

            template <typename T = value_type>
            inline void pop_back() {
                size_type size = priv.size - sizeof(T);
                priv.size = (size < priv.size) ? size : 0;
            }

            template <typename T = value_type, bool copyContents = true>
            inline void resize(size_type size, int c = 0) {
                size_type newSize = size * sizeof(T);
                reserve<copyContents>(newSize);
                if (newSize > priv.size) memset(priv.data + priv.size, c, newSize - priv.size);
                
                priv.size = newSize;
            }


            void swap(buffer &other) noexcept;


            inline bool pushf(const std::filesystem::path &path) {
                return insertf(size(), path);
            }
            bool insertf(size_type index, const std::filesystem::path &path) {
                return ktu::file::read(*this, index, path);
            }

            template <typename T = value_type>
            inline bool insertf(iterator<typename std::add_const<T>::type> pos, const std::filesystem::path &path) {
                return insertf(std::distance(cbegin<T>(), pos), path);
            }

            inline void write(const std::filesystem::path &path) {
                ktu::file::write(path, data(), data()+size());
            };

            bool operator==(const buffer &other) const;
            
            std::weak_ordering operator<=> (const buffer &other) const;
            friend inline std::ostream &operator<<(std::ostream &os, const buffer &buf) {
                return os << buf_io::output((const void*)buf.data(), buf.size());
            }
            void input(const char *str, size_t size, long flags = 0);
            inline void input(const std::string &str, long flags = 0) {
                return input(str.data(), str.size(), flags);
            }
            inline void input(const std::u8string &str, long flags = 0) {
                return input((const char*)str.data(), str.size(), flags);
            }
            
            friend std::istream &operator>>(std::istream &is, buffer &buf);
            
            
        private:
            
            struct {
                size_type size = 0;
                size_type capacity = 0;
                pointer data = nullptr;
            } priv;
        
    };

    
};