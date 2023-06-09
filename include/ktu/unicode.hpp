#pragma once
#include <ktu/template.hpp>
#include <ktu/variadic.hpp>
#include <ktu/bit.hpp>

#include <string>

#include <iostream>
namespace ktu {

    inline bool validUnicode(uint32_t codepoint) {
        return codepoint && ((codepoint < 0xD800) || (0xDFFF < codepoint && codepoint < 0x10FFFF));
    }

    class buffer;
    

    class u8 {
            
        private:
            struct impl {
                template <typename ...Args>
                static constexpr uint32_t read(Args... args);

                template <typename ...Args>
                static constexpr uint32_t rread(Args... args);

                #pragma GCC diagnostic push
                #pragma GCC diagnostic ignored "-Wundefined-inline"
                template <arithmetic<1> T>
                static constexpr size_t write(uint32_t codepoint, T *buf);
                #pragma GCC diagnostic pop
            };
        public:
            struct ptr {
                static uint32_t (*const read)(const void*);
                static uint32_t (*const read_ref)(const void**);
                static uint32_t (*const rread)(const void*);
                static uint32_t (*const rread_ref)(const void**);

                static size_t (*const write)(uint32_t, void*);
            };

            
            /* Returns a utf-codepoint decoded from utf-8, iterating the pointer forwards. */
            template <indirect_arithmetic<1> T>
            static inline uint32_t read(T* ptr) {
                return impl::read(indirect_cast<const uint8_t*>(ptr));
            }

            /* Returns a utf-codepoint decoded from utf-8, iterating the pointer forwards. */
            template <indirect_arithmetic<1> T, arithmetic<1> U>
            static inline consteval uint32_t read(T *ptr, U *front) {
                return impl::read(ptr, front);
            }

            
            /* Returns a utf-codepoint decoded from utf-8, iterating the pointer backwards. */
            template <indirect_arithmetic<1> T>
            static inline uint32_t rread(T* ptr) {
                return impl::rread(indirect_cast<const uint8_t*>(ptr));
            }

            /* Returns a utf-codepoint decoded from utf-8, iterating the pointer forwards. */
            template <indirect_arithmetic<1> T, arithmetic<1> U>
            static inline consteval uint32_t rread(T *ptr, U *front) {
                return impl::rread(ptr, front);
            }

            /* Get utf-8 units from a codepoint. */
            template <arithmetic<1> T>
            static inline constexpr size_t write(uint32_t codepoint, T *buf) {
                if (std::is_constant_evaluated()) {
                    return impl::write(codepoint, buf);
                } else {
                    return impl::write<uint8_t>(codepoint, (uint8_t*)buf);
                }
            }
            
            /* Insert utf-8 code units in a container. */
            template <typename T>
            static inline void insert(T &container, size_t pos, uint32_t codepoint) {
                typename std::conditional<ktu::arithmetic<typename T::value_type, 1>, typename T::value_type, uint8_t>::type buf[4];
                size_t size = ktu::u8::write(codepoint, &buf[0]);
                container.insert(pos, &buf[0], size);
            }
            

            /* Add utf-8 code units to the end of a container. */
            template <typename T>
            static inline void push_back(T &container, uint32_t codepoint) {
               insert<T>(container, container.size(), codepoint);
            }

            /* Convert an std::string to a u16string. */
            template <bool dest_big_endian>
            static inline std::u16string to_16(std::string &str);
            /* Convert an std::string to a u16string. */
            static inline std::u16string to_16(std::string &str, bool destBigEndian) {
                return (destBigEndian) ? to_16<true>(str) : to_16<false>(str);
            }
            /* Convert a u8string to a u16string. */
            template <bool dest_big_endian>
            static inline std::u16string to_16(std::u8string &str);
            /* Convert a u8string to a u16string. */
            static inline std::u16string to_16(std::u8string &str, bool destBigEndian) {
                return (destBigEndian) ? to_16<true>(str) : to_16<false>(str);
            }

            /* Convert an std::string to a u32string. */
            template <bool dest_big_endian>
            static inline std::u32string to_32(std::string &str);
            /* Convert an std::string to a u32string. */
            static inline std::u32string to_32(std::string &str, bool destBigEndian) {
                return (destBigEndian) ? to_32<true>(str) : to_32<false>(str);
            }
            /* Convert a u8string to a u32string. */
            template <bool dest_big_endian>
            static inline std::u32string to_32(std::u8string &str);
            /* Convert a u8string to a u32string. */
            static inline std::u32string to_32(std::u8string &str, bool destBigEndian) {
                return (destBigEndian) ? to_32<true>(str) : to_32<false>(str);
            }
    };

    class u16 {
        private:
            struct impl {
                template <bool big_endian, typename ...Args>
                static constexpr uint32_t read(Args... args);

                template <bool big_endian, typename ...Args>
                static constexpr uint32_t rread(Args... args);

                template <bool big_endian, arithmetic<2> T>
                static constexpr size_t write(uint32_t codepoint, T *buf);
            };
        public:
            struct ptr {
                static uint32_t (*const read_le)(const void*);
                static uint32_t (*const read_be)(const void*);
                static uint32_t (*const read_ref_le)(const void**);
                static uint32_t (*const read_ref_be)(const void**);
                static uint32_t (*const rread_le)(const void*);
                static uint32_t (*const rread_be)(const void*);
                static uint32_t (*const rread_ref_le)(const void**);
                static uint32_t (*const rread_ref_be)(const void**);

                static size_t (*const write_le)(uint32_t, void*);
                static size_t (*const write_be)(uint32_t, void*);
            };

            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer forwards. */
            template <bool big_endian, indirect_arithmetic T>
            static inline uint32_t read(T* ptr) {
                return impl::read<big_endian>(indirect_cast<const uint16_t*>(ptr));
            }

            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer forwards. */
            template <indirect_arithmetic T>
            static inline uint32_t read(T* ptr, bool bigEndian) {
                return (bigEndian) ? read<true>(ptr) : read<false>(ptr);
            }
            
            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer forwards. */
            template <bool big_endian, indirect_arithmetic<2> T, arithmetic<2> U>
            static inline consteval uint32_t read(T *ptr, U *front) {
                return impl::read<big_endian>(ptr, front);
            }

            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer forwards. */
            template <indirect_arithmetic<2> T, arithmetic<2> U>
            static inline consteval uint32_t read(T *ptr, U *front, bool bigEndian) {
                return (bigEndian) ? impl::read<true>(ptr, front) : impl::read<false>(ptr, front);
            }


            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer backwards. */
            template <bool big_endian, indirect_arithmetic T>
            static inline uint32_t rread(T* ptr) {
                return impl::rread<big_endian>(indirect_cast<const uint16_t*>(ptr));
            }

            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer backwards. */
            template <indirect_arithmetic T>
            static inline uint32_t rread(T* ptr, bool bigEndian) {
                return (bigEndian) ? rread<true>(ptr) : rread<false>(ptr);
            }

            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer forwards. */
            template <bool big_endian, indirect_arithmetic<2> T, arithmetic<2> U>
            static inline consteval uint32_t rread(T *ptr, U *front) {
                return impl::rread<big_endian>(ptr, front);
            }

            /* Returns a utf-codepoint decoded from utf-16, iterating the pointer forwards. */
            template <indirect_arithmetic<2> T, arithmetic<2> U>
            static inline consteval uint32_t rread(T *ptr, U *front, bool bigEndian) {
                return (bigEndian) ? impl::rread<true>(ptr, front) : impl::rread<false>(ptr, front);
            }

            /* Get utf-16 units from a codepoint. */
            template <bool big_endian, arithmetic<2> T>
            static inline constexpr size_t write(uint32_t codepoint, T *buf) {
                if (std::is_constant_evaluated()) {
                    return impl::write<big_endian>(codepoint, buf);
                } else {
                    return impl::write<big_endian, uint16_t>(codepoint, (uint16_t*)buf);
                }
            }
            /* Get utf-16 units from a codepoint. */
            template <arithmetic<2> T>
            static inline constexpr size_t write(uint32_t codepoint, T *buf, bool bigEndian) {
                return (bigEndian) ? write<true>(codepoint, buf) : write<false>(codepoint, buf);
            }
           

            /* Insert utf-16 code units in a container. */
            template <bool big_endian, typename T>
            static inline void insert(T &container, size_t pos, uint32_t codepoint) {
                typename std::conditional<ktu::arithmetic<typename T::value_type, 2>, typename T::value_type, uint16_t>::type buf[2];
                size_t size = write<big_endian>(codepoint, &buf[0]);
                container.insert(pos, &buf[0], size * (1 + std::is_same<T, ktu::buffer>::value));
            }

            /* Insert utf-16 code units in a container. */
            template <typename T>
            static inline void insert(T &container, size_t pos, uint32_t codepoint, bool bigEndian) {
                (bigEndian) ? insert<true>(container, pos, codepoint) : insert<false>(container, pos, codepoint);
            }


            /* Add utf-16 code units to the end of a container. */
            template <bool big_endian, typename T>
            static inline void push_back(T &container, uint32_t codepoint) {
               insert<big_endian, T>(container, container.size(), codepoint);
            }

            /* Add utf-16 code units to the end of a container. */
            template <typename T>
            static inline void push_back(T &container, size_t pos, uint32_t codepoint, bool bigEndian) {
                (bigEndian) ? push_back<true>(container, codepoint) : push_back<false>(container, codepoint);
            }

           
            /* Convert a u16string to an std::string. */
            template <bool src_big_endian>
            static inline std::string to_string(std::u16string &str) {
                std::string result;
                for (auto ptr = str.data(); ptr != str.data()+str.size();) {
                    u8::push_back(result, read<src_big_endian>(&ptr));
                }
                return result;
            }
            /* Convert a u16string to an std::string. */
            static inline std::string to_string(std::u16string &str, bool srcBigEndian) {
                return (srcBigEndian) ? to_string<true>(str) : to_string<false>(str);
            }
            /* Convert a u16string to a u8string. */
            template <bool src_big_endian>
            static inline std::u8string to_8(std::u16string &str) {
                std::u8string result;
                for (auto ptr = str.data(); ptr != str.data()+str.size();) {
                    u8::push_back(result, read<src_big_endian>(&ptr));
                }
                return result;
            }
            /* Convert a u16string to a u8string. */
            static inline std::u8string to_8(std::u16string &str, bool srcBigEndian) {
                return (srcBigEndian) ? to_8<true>(str) : to_8<false>(str);
            }
            /* Convert a u16string to a u32string. */
            template <bool src_big_endian, bool dest_big_endian>
            static inline std::u32string to_32(std::u16string &str);
            /* Convert a u16string to a u8string. */
            static inline std::u32string to_32(std::u16string &str, bool srcBigEndian, bool destBigEndian) {
                return
                    (srcBigEndian) ?
                        (destBigEndian) ?
                            to_32<true, true>(str) :
                            to_32<false, true>(str)
                        :
                        (destBigEndian) ?
                            to_32<true, false>(str) :
                            to_32<false, false>(str);
            }

    };

    class u32 {
        public:
            struct ptr {
                static uint32_t (*const read_le)(const void*);
                static uint32_t (*const read_be)(const void*);
                static uint32_t (*const read_ref_le)(const void**);
                static uint32_t (*const read_ref_be)(const void**);
                static uint32_t (*const rread_le)(const void*);
                static uint32_t (*const rread_be)(const void*);
                static uint32_t (*const rread_ref_le)(const void**);
                static uint32_t (*const rread_ref_be)(const void**);

                static size_t (*const write_le)(uint32_t,void*);
                static size_t (*const write_be)(uint32_t,void*);
            };
            /* Returns a utf-codepoint decoded from utf-32, iterating the pointer forwards. */
            template <bool big_endian, indirect_arithmetic<4> T>
            static inline constexpr uint32_t read(T *ptr) {
                return ktu::big_endian<big_endian>(*dereference<-1>(ptr)++);
            }
            /* Returns a utf-codepoint decoded from utf-32, iterating the pointer forwards. */
            template <bool big_endian, indirect_arithmetic T>
            requires (!indirect_arithmetic<T, 4>)
            static inline constexpr uint32_t read(T *ptr) {
                return read<big_endian>(indirect_cast<uint32_t*>(ptr));
            }

            /* Returns a utf-codepoint decoded from utf-32, iterating the pointer forwards. */
            template <indirect_arithmetic T>
            static inline constexpr uint32_t read(T *ptr, bool bigEndian) {
                return (bigEndian) ? read<true>(ptr) : read<false>(ptr);
            }

            /* Returns a utf-codepoint decoded from utf-32, iterating the pointer backwards. */
            template <bool big_endian, indirect_arithmetic<4> T>
            static inline constexpr uint32_t rread(T *ptr) {
                return ktu::big_endian<big_endian>(*dereference<-1>(ptr)--);
            }
            /* Returns a utf-codepoint decoded from utf-32, iterating the pointer backwards. */
            template <bool big_endian, indirect_arithmetic T>
            requires (!indirect_arithmetic<T, 4>)
            static inline constexpr uint32_t rread(T *ptr) {
                return rread<big_endian>(indirect_cast<uint32_t*>(ptr));
            }
            /* Returns a utf-codepoint decoded from utf-32, iterating the pointer backwards. */
            template <indirect_arithmetic<4> T>
            static inline constexpr uint32_t rread(T *ptr, bool bigEndian) {
                return (bigEndian) ? rread<true>(ptr) : rread<false>(ptr);
            }

            /* Get a utf32-codepoint. */
            template <bool big_endian>
            static inline constexpr uint32_t write(uint32_t codepoint) {
                return ktu::big_endian<big_endian>(codepoint);
            }
            /* Get a utf32-codepoint. */
            static inline constexpr uint32_t write(uint32_t codepoint, bool bigEndian) {
                return (bigEndian) ? write<true>(codepoint) : write<false>(codepoint);
            }
            /* Get a utf32-codepoint. */
            template <bool big_endian, arithmetic<4> T, bool ptr_mode = false>
            static inline constexpr auto write(uint32_t codepoint, T *buf) {
                *buf = ktu::big_endian<big_endian>(codepoint);
                if constexpr (ptr_mode) return (size_t)1;
            }
            /* Get a utf32-codepoint. */
            template <arithmetic<4> T>
            static inline constexpr void write(uint32_t codepoint, T *buf, bool bigEndian) {
                (bigEndian) ? write<true>(codepoint, buf) : write<false>(codepoint, buf);
            }
            /* Write a utf-32 codepoint to a u32string. */
            template <bool big_endian>
            static inline void write(std::u32string &str, uint32_t codepoint) {
                str.push_back(ktu::big_endian<big_endian>(codepoint));
            }
            /* Write a utf-32 codepoint to a u32string. */
            static inline void write(std::u32string &str, uint32_t codepoint, bool bigEndian) {
                return (bigEndian) ? write<true>(str, codepoint) : write<false>(str, codepoint);
            }

            /* Insert a utf-32 code unit in a container. */
            template <bool big_endian, typename T>
            static inline void insert(T &container, size_t pos, uint32_t codepoint) {
                container.insert(container.begin()+pos, ktu::big_endian<big_endian>(codepoint));
            }

            /* Insert a utf-32 code unit in a container. */
            template <typename T>
            static inline void insert(T &container, size_t pos, uint32_t codepoint, bool big_endian) {
                (big_endian) ? insert<true>(container, pos, codepoint) : insert<false>(container, pos, codepoint);
            }

            /* Insert a utf-32 code unit in a container. */
            template <bool big_endian, typename T>
            static inline void push_back(T &container, uint32_t codepoint) {
                insert<big_endian>(container, container.size(), codepoint);
            }

            /* Add a utf-32 code unit to the end of a container. */
            template <typename T>
            static inline void push_back(T &container, uint32_t codepoint, bool big_endian) {
                (big_endian) ? push_back<true>(container, codepoint) :  push_back<false>(container, codepoint);
            }


            /* Convert a u32string to an std::string. */
            template <bool src_big_endian>
            static inline std::string to_string(std::u32string &str) {
                std::string result;
                for (auto ptr = str.data(); ptr != str.data()+str.size();) {
                    u8::push_back(result, read<src_big_endian>(&ptr));
                }
                return result;
            }
            /* Convert a u32string to a std::string. */
            static inline std::string to_string(std::u32string &str, bool srcBigEndian) {
                return (srcBigEndian) ? to_string<true>(str) : to_string<false>(str);
            }
            /* Convert a u32string to a u8string. */
            template <bool src_big_endian>
            static inline std::u8string to_8(std::u32string &str) {
                std::u8string result;
                for (auto ptr = str.data(); ptr != str.data()+str.size();) {
                    u8::push_back(result, read<src_big_endian>(&ptr));
                }
                return result;
            }
            /* Convert a u32string to a u8string. */
            static inline std::u8string to_8(std::u32string &str, bool srcBigEndian) {
                return (srcBigEndian) ? to_8<true>(str) : to_8<false>(str);
            }

            /* Convert a u32string to a u16string. */
            template <bool src_big_endian, bool dest_big_endian>
            static inline std::u16string to_16(std::u32string &str) {
                std::u16string result;
                for (auto ptr = str.data(); str.data()+str.size();) {
                    u16::insert<dest_big_endian>(result, result.size(), read<src_big_endian>(&ptr));
                }
                return result;
            }
            /* Convert a u32string to a u16string. */
            static inline std::u16string to_16(std::u32string &str, bool srcBigEndian, bool destBigEndian) {
                return
                    (srcBigEndian) ?
                        (destBigEndian) ?
                            to_16<true, true>(str) :
                            to_16<false, true>(str)
                        :
                        (destBigEndian) ?
                            to_16<true, false>(str) :
                            to_16<false, false>(str);
            }
    };

};

#include "unicode.tcc"



inline std::ostream &operator<<(std::ostream &os, std::u8string &str) {
    return os << ktu::instantiation<std::string>::cast(str);
}