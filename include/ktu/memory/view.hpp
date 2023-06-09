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
namespace ktu {
    class buffer;
    class reader;


    class view {
        public:
            using value_type = uint8_t;
            using size_type = size_t;
            using pointer = const value_type*;
            using const_pointer = pointer const;

            view() : first(nullptr), last(nullptr) {}

            template <typename T>
            view(const T *first, const T *last) : first((const value_type*)first), last((const value_type*)last) {}
            template <typename T>
            view(const T *first, size_type size) : first((const value_type*)first), last((const value_type*)(first + size)) {}

            operator buffer() const;
            operator reader() const;


            template <auto match, ktu::arithmetic T = value_type>
            inline const T *find(const T *ptr) {
                while (!match(*ptr) && ptr < end<T>())
                    ++ptr;
                return ptr;
            }
            template <auto match, ktu::arithmetic T = value_type>
            inline const T *find() {return find<match, T>(begin<T>());}

            template <ktu::arithmetic T = value_type>
            inline const T *find(T match, const T *ptr) {
                while ((match != *ptr) && ptr < end<T>())
                    ++ptr;
                return ptr;
            }
            template <ktu::arithmetic T = value_type, ktu::arithmetic U>
            requires (!std::is_same<T, U>::value)
            inline const T *find(T match, const U *ptr) {
                return find(match, (const T*)ptr);
            }
            template <ktu::arithmetic T = value_type>
            inline const T *find(T match) {return find<T>(match, begin<T>());}
            
            template <class _Searcher>
            requires (!ktu::arithmetic<_Searcher>)
            inline const auto *find(_Searcher Searcher, const auto *ptr) {
                using T = typename std::remove_reference<decltype(*(Searcher.__first_))>::type;
                return std::search((T*)ptr, end<T>(), Searcher);
            }
            template <class _Searcher>
            requires (!ktu::arithmetic<_Searcher>)
            inline const auto *find(_Searcher Searcher) {
                using T = typename std::remove_reference<decltype(*(Searcher.__first_))>::type;
                return find<_Searcher, T>(Searcher, begin<T>());
            }

            template <typename T = value_type>
            inline const T* const begin() const {
                return (const T* const)first;
            }
            template <typename T = value_type>
            inline const T* const end() const {
                if constexpr (sizeof(T) == 1) {
                    return (const T* const)(last);
                } else {
                    return (const T* const)(last - ((size()) % sizeof(T)));
                }
                
            }

            template <typename T = value_type>
            inline auto range() const {
                return ktu::range(begin<T>(), end<T>());
            }
            

            template <typename T = value_type>
            inline size_type size() const {
                return ((const T* const)last - (const T* const)first);
            }

            inline view slice(size_type size) {
                return view(begin(), size);
            }
            inline view slice(size_type first, size_type last) {
                return view(begin() + first, begin() + last);
            }

            template <typename T>
            inline view slice(const T* position, size_type size) {
                return view(position, size);
            }

            template <typename T>
            inline view slice(const T* first, const T*last) {
                return view(first, last);
            }

            inline void writef(const std::filesystem::path &path) {
                ktu::writef(path, begin(), end());
            }

            inline void writef(const std::filesystem::path &path, size_type size) {
                ktu::writef(path, begin(), begin()+size);
            }
            template <typename T>
            inline void writef(const std::filesystem::path &path, const T* position, size_type size) {
                ktu::writef(path, (const char*)position, (const char*)(position+size));
            }
            template <typename T>
            inline void writef(const std::filesystem::path &path, const T* first, const T* last) {
                ktu::writef(path, (const char*)first, (const char*)last);
            }

            inline const view &operator=(const view &r) {
                first = r.first;
                last = r.last;
                return *this;
            }

            

            friend inline std::ostream &operator<<(std::ostream &os, const view &r) {
                return os << buf_io::output((const void*)r.begin(), r.size());
            }

        private:
            pointer first;
            pointer last;
            friend reader;
    };

    
};