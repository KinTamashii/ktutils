#pragma once
#include <ktu/ios.hpp>
#include <ktu/memory/writef.hpp>
#include <ktu/bit.hpp>
#include <ktu/iterator.hpp>
#include <ktu/template.hpp>
#include <string>
#include <algorithm>
#include <functional>
#include <experimental/algorithm>
#include <experimental/functional>
#include <ktu/array.hpp>
#include <ktu/memory/view.hpp>
namespace ktu {
    class buffer;



    template <typename T = void, typename U>
    inline auto peek(U *ptr) {
        using return_type = typename std::conditional<std::is_same<T, void>::value, U, T>::type;
        return *(return_type*)ptr;
    }
    template <typename T = void>
    inline auto peek_little_endian(auto *ptr) {
        return little_endian(peek<T>(ptr));
    }
    template <bool condition = true, typename T = void>
    inline auto peek_little_endian(auto *ptr) {
        return little_endian<condition, T>(peek<T>(ptr));
    }
    template <typename T = void>
    inline auto peek_little_endian(auto *ptr, bool condition) {
        return (condition) ? little_endian(peek<T>(ptr)) : peek<T>(ptr);
    }
    template <typename T = void>
    inline auto peek_big_endian(auto *ptr) {
        return big_endian(peek<T>(ptr));
    }
    template <bool condition = true, typename T = void>
    inline auto peek_big_endian(auto *ptr) {
        return big_endian<condition, T>(peek<T>(ptr));
    }
    template <typename T = void>
    inline auto peek_big_endian(auto *ptr, bool condition) {
        return (condition) ? big_endian(peek<T>(ptr)) : peek<T>(ptr);
    }


    namespace impl {

            template <bool forward, typename T, typename U>
        inline void advance(U *&ptr) {
            if constexpr (forward)
                ptr = (U*)((uint8_t*)(ptr) + sizeof(T));
            else
                ptr = (U*)((uint8_t*)(ptr) - sizeof(T));
        }


        template <bool forward, typename T = void, typename U>
        inline auto read(U *&ptr) {
            using return_type = typename std::conditional<std::is_same<T, void>::value, U, T>::type;
            return_type value = peek<return_type>(ptr);
            advance<forward, return_type>(ptr);
            // if constexpr (forward)
            //     ptr = (U*)((uint8_t*)(ptr) + sizeof(return_type));
            // else
            //     ptr = (U*)((uint8_t*)(ptr) - sizeof(return_type));
            
            return value;
        }

        template <bool forward, typename T>
        inline void write(auto *&ptr, T value) {
            *(T*)ptr = value;
            advance<forward, T>(ptr);
        }
    };

    template <typename T = void>
    inline auto read(auto *&ptr) {
        return impl::read<true, T>(ptr);
    }
    template <typename T = void>
    inline auto read_little_endian(auto *&ptr) {
        return little_endian(read<T>(ptr));
    }
    template <bool condition = true, typename T = void>
    inline auto read_little_endian(auto *&ptr) {
        return little_endian<condition>(read<T>(ptr));
    }
    template <typename T = void>
    inline auto read_little_endian(auto *&ptr, bool condition) {
        return (condition) ? read_little_endian<T>(ptr) : read<T>(ptr);
    }
    template <typename T = void>
    inline auto read_big_endian(auto *&ptr) {
        return big_endian(read<T>(ptr));
    }
    template <bool condition = true, typename T = void>
    inline auto read_big_endian(auto *&ptr) {
        return big_endian<condition>(read<T>(ptr));
    }
    template <typename T = void>
    inline auto read_big_endian(auto *&ptr, bool condition) {
        return (condition) ? read_big_endian<T>(ptr) : read<T>(ptr);
    }
    template <typename T = void>
    inline auto rread(auto *&ptr) {
        return impl::read<false, T>(ptr);
    }
        template <typename T = void>
    inline auto rread_little_endian(auto *&ptr) {
        return little_endian(rread<T>(ptr));
    }
    template <bool condition = true, typename T = void>
    inline auto rread_little_endian(auto *&ptr) {
        return little_endian<condition>(rread<T>(ptr));
    }
    template <typename T = void>
    inline auto rread_little_endian(auto *&ptr, bool condition) {
        return (condition) ? rread_little_endian<T>(ptr) : rread<T>(ptr);
    }
    template <typename T = void>
    inline auto rread_big_endian(auto *&ptr) {
        return big_endian(rread<T>(ptr));
    }
    template <bool condition = true, typename T = void>
    inline auto rread_big_endian(auto *&ptr) {
        return big_endian<condition>(rread<T>(ptr));
    }
    template <typename T = void>
    inline auto rread_big_endian(auto *&ptr, bool condition) {
        return (condition) ? rread_big_endian<T>(ptr) : rread<T>(ptr);
    }
    template <typename T>
    inline void write(auto *&ptr, T item) {
        impl::write<true, T>(ptr, item);
    }
    template <typename T>
    inline void write_little_endian(auto *&ptr, T item) {
        write<T>(ptr, ktu::little_endian(item));
    }
    template <bool little_endian, typename T>
    inline void write_little_endian(auto *&ptr, T item) {
        write<T>(ptr, ktu::little_endian<little_endian, T>(item));
    }
    template <typename T>
    inline void write_big_endian(auto *&ptr, T item) {
        write<T>(ptr, ktu::big_endian(item));
    }
    template <bool big_endian, typename T>
    inline void write_big_endian(auto *&ptr, T item) {
        write<T>(ptr, ktu::big_endian<big_endian, T>(item));
    }

    template <typename T>
    inline void rwrite(auto *&ptr, T item) {
        impl::write<false, T>(ptr, item);
    }
    template <typename T>
    inline void rwrite_little_endian(auto *&ptr, T item) {
        rwrite<T>(ptr, ktu::little_endian(item));
    }
    template <bool little_endian, typename T>
    inline void rwrite_little_endian(auto *&ptr, T item) {
        rwrite<T>(ptr, ktu::little_endian<little_endian, T>(item));
    }
    template <typename T>
    inline void rwrite_big_endian(auto *&ptr, T item) {
        rwrite<T>(ptr, ktu::big_endian(item));
    }
    template <bool big_endian, typename T>
    inline void rwrite_big_endian(auto *&ptr, T item) {
        rwrite<T>(ptr, ktu::big_endian<big_endian, T>(item));
    }


    

template <auto func>
inline std::string readString(const char *&ptr) {
    const char * tmp = ptr;
    ptr = func(ptr);
    return std::string(tmp, ptr);
}
    
    class reader {
        public:
            using value_type = uint8_t;
            using size_type = size_t;
            using pointer = const value_type*;
            using const_pointer = pointer const;

            reader() : view() {}

            template <typename T>
            reader(const T *first, const T *last) : view(first, last), ptr((const value_type*)first) {}
            template <typename T>
            reader(const T *first, size_type size) : view(first, first+size), ptr((const value_type*)first) {}

            operator buffer() const;
            operator view() const;

            template <typename T = value_type>
            inline T read() {
                return ktu::read<T>(ptr);
            }
            template <auto match, ktu::arithmetic T = value_type>
            inline bool is() {
                return match(ktu::peek<T>(ptr));
            }
            template <ktu::arithmetic T = value_type>
            inline bool is(T match) {
                return match == ktu::peek<T>(ptr);
            }

            template <ktu::indirect_arithmetic T = pointer>
            inline bool is(T first, T last) {
                return !memcmp(cur<typename std::remove_pointer<T>::type>(), first, last-first);
            }
            template <ktu::arithmetic T = value_type, size_t N>
            inline bool is(T(&array)[N]) {
                return is(
                    &array[0],
                    &array[N-(
                        std::is_same<T, const char>::value ||
                        std::is_same<T, const char8_t>::value ||
                        std::is_same<T, const char16_t>::value ||
                        std::is_same<T, const char32_t>::value
                    )]
                );
            }
            template <auto match, ktu::arithmetic T = value_type>
            inline view get() {
                T* first = cur<T>();
                while (valid<T>() && is<match, T>())
                    ptr += sizeof(T);
                return view(first, ptr);
            }
            template <auto match, ktu::arithmetic T = value_type>
            inline view nget() {
                T* first = cur<T>();
                while (valid<T>() && !is<match, T>())
                    ptr += sizeof(T);
                return view(first, ptr);
            }



            template <auto match, ktu::arithmetic T = value_type>
            inline bool find() {
                seek<T>(view.find<match, T>(cur<T>()));
                return valid<T>();
            }
            template <ktu::arithmetic T = value_type>
            inline bool find(T match) {
                seek<T>(view.find<T>(match, cur<T>()));
                return valid<T>();
            }
            template <class _Searcher>
            inline bool find(_Searcher Searcher) {
                using T = typename std::remove_reference<decltype(*(Searcher.__first_))>::type;
                seek<T>(view.find<_Searcher>(Searcher, cur<T>()));
                return valid<T>();
            }

            template <typename T>
            inline T read_little_endian() {
                return ktu::read_little_endian<T>(ptr);
            }
            template <bool condition = true, typename T = value_type>
            inline T read_little_endian() {
                return ktu::read_little_endian<condition, T>(ptr);
            }
            template <typename T = value_type>
            inline T read_little_endian(bool condition) {
                return ktu::read_little_endian<T>(ptr, condition);
            }
            template <typename T>
            inline T read_big_endian() {
                return ktu::read_big_endian<T>(ptr);
            }
            template <bool condition = true, typename T = value_type>
            inline T read_big_endian() {
                return ktu::read_big_endian<condition, T>(ptr);
            }
            template <typename T = value_type>
            inline T read_big_endian(bool condition) {
                return ktu::read_big_endian<T>(ptr, condition);
            }

            template <typename T = value_type>
            inline T sread() {
                return (valid<T>()) ? read<T>() : T();
            }
            template <typename T = value_type>
            inline T sread_little_endian() {
                return (valid<T>()) ? ktu::read_little_endian<T>(ptr) : T();
            }
            template <bool condition = true, typename T = value_type>
            inline T sread_little_endian() {
                return (valid<T>()) ? ktu::read_little_endian<condition, T>(ptr) : T();
            }
            template <typename T = value_type>
            inline T sread_little_endian(bool condition) {
                return (valid<T>()) ? ktu::read_little_endian<T>(ptr, condition) : T();
            }
            template <typename T = value_type>
            inline T sread_big_endian() {
                return (valid<T>()) ? ktu::read_big_endian<T>(ptr) : T();
            }
            template <bool condition = true, typename T = value_type>
            inline T sread_big_endian() {
                return (valid<T>()) ? ktu::read_big_endian<condition, T>(ptr) : T();
            }
            template <typename T = value_type>
            inline T sread_big_endian(bool condition) {
                return (valid<T>()) ? ktu::read_big_endian<T>(ptr, condition) : T();
            }

            template <typename T = value_type>
            inline T peek() const {
                return ktu::peek<T>(ptr);
            }
            template <typename T = value_type>
            inline T speek() const {
                return (valid<T>()) ? peek<T>() : T();
            }

            template <typename T = value_type>
            inline T peek_little_endian() const {
                return ktu::peek_little_endian<T>(ptr);
            }
            template <bool little_endian, typename T = value_type>
            inline T peek_little_endian() const {
                return ktu::peek_little_endian<little_endian, T>(ptr);
            }
            template <typename T = value_type>
            inline T peek_little_endian(bool littleEndian) const {
                return ktu::peek_little_endian<T>(ptr, littleEndian);
            }
            template <typename T = value_type>
            inline T peek_big_endian() const {
                return ktu::peek_big_endian<T>(ptr);
            }
            template <bool big_endian, typename T = value_type>
            inline T peek_big_endian() const {
                return ktu::peek_big_endian<big_endian, T>(ptr);
            }
            template <typename T = value_type>
            inline T peek_big_endian(bool bigEndian) const {
                return ktu::peek_big_endian<T>(ptr, bigEndian);
            }

            template <typename T = value_type>
            inline T speek_little_endian() const {
                return (valid<T>()) ? peek_little_endian<T>() : T();
            }
            template <bool little_endian, typename T = value_type>
            inline T speek_little_endian() const {
                return (valid<T>()) ? peek_little_endian<little_endian, T>() : T();
            }
            template <typename T = value_type>
            inline T speek_little_endian(bool littleEndian) const {
                return (valid<T>()) ? peek_little_endian<T>(littleEndian) : T();
            }
            template <typename T = value_type>
            inline T speek_big_endian() const {
                return (valid<T>()) ? peek_big_endian<T>() : T();
            }
            template <bool big_endian, typename T = value_type>
            inline T speek_big_endian() const {
                return (valid<T>()) ? peek_big_endian<big_endian, T>() : T();
            }
            template <typename T = value_type>
            inline T speek_big_endian(bool bigEndian) const {
                return (valid<T>()) ? peek_big_endian<T>(bigEndian) : T();
            }
            template <typename T = value_type>
            inline void seek(size_type pos = 1) {
                ptr = (pointer)(begin<T>() + pos);
            }

            inline void seek(const auto *ptr) {
                this->ptr = (const value_type*)ptr;
            }

            template <typename T = value_type>
            inline void skip(size_type count = 1) {
                ptr = (pointer)(cur<T>() + count);
            }

            template <typename T = value_type>
            inline const T*cur() {
                return (const T*)ptr;
            }

            template <typename T = value_type>
            inline const T* const begin() const {
                return view.begin<T>();
            }
            template <typename T = value_type>
            inline const T* const end() const {
                return view.end<T>();
            }

            template <typename T = value_type>
            inline auto range() const {
                return view.range<T>();
            }
            
        
            template <typename T = value_type>
            inline bool valid() const noexcept {
                return (T*)ptr < end<T>();
            };

            template <typename T = value_type>
            inline size_type size() const {
                return view.size<T>();
            }

            inline reader slice(size_type size) {
                return reader(begin(), size);
            }
            inline reader slice(size_type first, size_type last) {
                return reader(begin() + first, begin() + last);
            }

            template <typename T>
            inline reader slice(const T* position, size_type size) {
                return reader(position, size);
            }

            template <typename T>
            inline reader slice(const T* first, const T*last) {
                return reader(first, last);
            }

            inline void writef(const std::filesystem::path &path) {
                view.writef(path);
            }

            inline void writef(const std::filesystem::path &path, size_type size) {
                view.writef(path, size);
            }
            template <typename T>
            inline void writef(const std::filesystem::path &path, const T* position, size_type size) {
                view.writef(path, position, size);
            }
            template <typename T>
            inline void writef(const std::filesystem::path &path, const T* first, const T* last) {
                view.writef(path, first, last);
            }

            inline const reader &operator=(const reader &r) {
                view = r.view;
                ptr = r.ptr;
                return *this;
            }

            

            friend inline std::ostream &operator<<(std::ostream &os, const reader &r) {
                return os << buf_io::output((const void*)r.begin(), r.size());
            }

        private:
            view view;
            pointer ptr;
            
    };

    
};