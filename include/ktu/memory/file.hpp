#pragma once
#include <filesystem>
#include <ktu/template.hpp>
#include <fstream>
#include <iostream>



namespace ktu {
    // void writef(const std::filesystem::path &path, char *first, char *last);
    
    // template <typename T, typename U>
    // requires (!std::is_same<T, char>::value && !std::is_same<U, char>::value)
    // inline void writef(const std::filesystem::path &path, T *first, U *last) {
    //     writef(path, (char*)first, (char*)last);
    // }

    // template <typename T>
    // inline void writef(const std::filesystem::path &path, T *pos, size_t size) {
    //     writef(path, pos, pos+size);
    // }


    // template <class CharT> class basic_multi_string;
    
    struct file {

        template <typename T>
        struct result_type {
            T result;
            bool success;
        };

        static void write(const std::filesystem::path &path, char *first, char *last);
    
        template <typename T, typename U>
        requires (!std::is_same<T, char>::value && !std::is_same<U, char>::value)
        inline static void write(const std::filesystem::path &path, T *first, U *last) {
            write(path, (char*)first, (char*)last);
        }

        template <typename T>
        inline static void write(const std::filesystem::path &path, T *pos, size_t size) {
            writef(path, pos, pos+size);
        }


        static void read(const std::filesystem::path &path, void *dst, size_t filesize);
        
        class info {
            public:
                template <size_t N>
                info(const char (&path)[N])             : priv(path) {}
                info(const char *path)                  : priv(path) {}
                info(const std::filesystem::path &path) : priv(path) {}

                inline void read(void *dst) {
                    file::read(priv.path, dst, priv.size);
                };
                inline size_t size() {return priv.size;}
                inline size_t exists() {return priv.exists;}
                const std::filesystem::path &path() {return priv.path;}
            private:
                struct priv_t {
                    template <size_t N>
                    priv_t (const char (&path)[N])              : path(path) {assign();}
                    priv_t (const char *path)                   : path(path) {assign();}
                    priv_t (const std::filesystem::path &path)  : path(path) {assign();}
                    void assign() {
                        if (!(exists = std::filesystem::exists(path))) {
                            size = 0;
                            return;
                        }
                        size = std::filesystem::file_size(path);
                    }
                    const std::filesystem::path path;
                    size_t size;
                    bool exists;
                } priv;
        };
        

        template <class inputClass>
        inline static bool read(inputClass &inputObject, size_t index, info info) {
            if (!info.exists())
                return false;
            size_t count = inputObject.size()-index;
            inputObject.resize(inputObject.size()+info.size());
            
            void *ptr = (void*)(inputObject.data()+index);
            
            memcpy(inputObject.data()+index+info.size(), inputObject.data()+index, count);
            info.read(ptr);
            return true;
        }
        // template <class CharT>
        // inline static bool read(basic_multi_string<CharT> &inputObject, size_t index, info info) {
        //     if (!info.exists())
        //         return false;
        //     size_t count = inputObject.building_size()-index;
        //     inputObject.building_resize(inputObject.building_size()+info.size());

        //     void *ptr = (void*)(inputObject.building_data()+index);

        //     memcpy(inputObject.building_data()+index+info.size(), inputObject.building_data()+index, count);
        //     info.read(ptr);
        //     inputObject.push_building();
        //     return true;
        // }
    
        template <class inputClass>
        inline static bool read(inputClass &inputObject, info info) {
            return read(inputObject, inputObject.size(), info);
        }

        // template <class CharT>
        // inline static bool read(basic_multi_string<CharT> &inputObject, info info) {
        //     return read(inputObject, inputObject.building_size(), info);
        // }
        
        


    };

};