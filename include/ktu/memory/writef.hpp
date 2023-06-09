#pragma once
#include <filesystem>
#include <ktu/template.hpp>
namespace ktu {
    void writef(const std::filesystem::path &path, char *first, char *last);
    
    template <typename T, typename U>
    requires (!std::is_same<T, char>::value && !std::is_same<U, char>::value)
    inline void writef(const std::filesystem::path &path, T *first, U *last) {
        writef(path, (char*)first, (char*)last);
    }

    template <typename T>
    inline void writef(const std::filesystem::path &path, T *pos, size_t size) {
        writef(path, pos, pos+size);
    }
};