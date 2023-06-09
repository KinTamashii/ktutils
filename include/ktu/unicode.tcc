#pragma once
template <typename ...Args>
constexpr uint32_t ktu::u8::impl::read(Args... args) {
    const auto &back = variadic::get<1>(args...);
    const auto *&cur = dereference<-1>(variadic::get<0>(args...));
    uint8_t c1, c2, c3, c4;
    auto inc = [&cur, &back]() {
        if constexpr (std::is_same<decltype(back), const empty&>::value) {
            return *cur++;
        } else {
            return (cur == back) ? *cur : *cur++;
        } 
    };
    c1 = inc();
    if (c1 < 0x80) // ASCII
        return c1;
    if (c1 < 0xC0) // Invalid.
        return 0xFFFD;

    c2 = inc();
    if (!c2) // Invalid.
        return 0xFFFD;
    if (c1 < 0xE0) // 2 Byte
        return (uint32_t)
            ((c1 & 0x1F) << 6) |
            (c2 & 0x3F);

    c3 = inc();
    if (!c3) // Invalid.
        return 0xFFFD;
    if (c1 < 0xF0) // 3 Byte
        return (uint32_t)
        ((c1 & 0xF) << 12) |
        ((c2 & 0x3F) << 6) |
        (c3 & 0x3F);
    
    c4 = inc();
    if (!c4) // Invalid.
        return 0xFFFD;
    if (c1 < 0xF8) // 4 Byte
        return (uint32_t)
            ((c1 & 0x7) << 18) |
            ((c2 & 0x3F) << 12) |
            ((c3 & 0x3F) << 6) |
            (c4 & 0x3F);
    
    return 0xFFFD; // Invalid.
}

template <typename ...Args>
constexpr uint32_t ktu::u8::impl::rread(Args... args) {
    const auto &front = variadic::get<1>(args...);
    const auto *&cur = dereference<-1>(variadic::get<0>(args...));
    uint8_t c1, c2, c3, c4;
    auto dec = [&cur, &front]() {
        if constexpr (std::is_same<decltype(front), const empty&>::value) {
            return *cur--;
        } else {
            return (cur == front) ? *cur : *cur--;
        } 
    };

    c1 = dec();

    if (c1 < 0x80) { // ASCII
        return c1;
    }

    if (c1 < 0xC0) { // Must be at least 2 bytes.
        c2 = dec();
        if (c2 < 0x80) // Last byte was invalid, but this is an ASCII character.
            return c2;
        if (c2 < 0xC0) { // Must be at least 3 bytes.
            c3 = dec();
            if (c3 < 0x80) // Last 2 bytes were invalid, but this is an ASCII character.
                return c3;
            if (c3 < 0xC0) { // Must be at least 4 bytes.
                c4 = dec();
                if (c4 < 0x80) // Last 3 bytes were invalid, but this is an ASCII character.
                    return c4;
                if (0xEF < c4 && c4 < 0xF8) { // Valid 4 byte.
                    return (uint32_t)
                        ((c4 & 0x7) << 18) |
                        ((c3 & 0x3F) << 12) |
                        ((c2 & 0x3F) << 6) |
                        (c1 & 0x3F);
                }
                return 0xFFFD; // Invalid.
            }
            if (0xDF < c3 && c3 < 0xF0) { // Valid 3 byte.
                return (uint32_t)
                    ((c3 & 0xF) << 12) |
                    ((c2 & 0x3F) << 6) |
                    (c1 & 0x3F);
            }
            return 0xFFFD; // Invalid.
        }
        if (c2 < 0xE0) { // Valid 2 Byte.
            return (uint32_t)
                    ((c2 & 0x1F) << 6) |
                    (c1 & 0x3F);
        }
        return 0xFFFD; // Invalid.
    }
    return 0xFFFD; // Invalid.
};

template <ktu::arithmetic<1> T>
constexpr size_t ktu::u8::impl::write(uint32_t codepoint, T *buf) {
    if (codepoint < 0x80) {
        buf[0] = codepoint;
        return 1;
    }
        
    if (codepoint < 0x800) {
        buf[0] = ((codepoint >> 6) & 0x1F) | 0xC0;
        buf[1] = (codepoint & 0x3F) | 0x80;
        return 2;
    }
    if (codepoint < 0x10000) {
        buf[0] = ((codepoint >> 12) & 0x0F) | 0xE0;
        buf[1] = ((codepoint >>  6) & 0x3F) | 0x80;
        buf[2] = (codepoint & 0x3F) | 0x80;
        return 3;
    }
    if (codepoint < 0x110000) {
        buf[0] = ((codepoint >> 18) & 0x07) | 0xF0;
        buf[1] = ((codepoint >> 12) & 0x3F) | 0x80;
        buf[2] = ((codepoint >>  6) & 0x3F) | 0x80;
        buf[3] = (codepoint & 0x3F) | 0x80;
        return 4;
    }
    buf[0] = 0xEF;
    buf[1] = 0xBF;
    buf[2] = 0xBD;
    return 3;
}

template <bool dest_big_endian>
inline std::u16string ktu::u8::to_16(std::string &str) {
    std::u16string result;
    for (auto ptr = str.data(); ptr != str.data()+str.size();) {
        u16::insert<dest_big_endian>(result, result.size(), read(&ptr));
    }
    return result;
}
template <bool dest_big_endian>
inline std::u16string ktu::u8::to_16(std::u8string &str) {
    std::u16string result;
    for (auto ptr = str.data(); ptr != str.data()+str.size();) {
        u16::insert<dest_big_endian>(result, result.size(), read(&ptr));
    }
    return result;
}

template <bool dest_big_endian>
inline std::u32string ktu::u8::to_32(std::string &str) {
    std::u32string result;
    for (auto ptr = str.data(); ptr != str.data()+str.size();) {
        u32::write<dest_big_endian>(result, read(&ptr));
    }
    return result;
}
template <bool dest_big_endian>
inline std::u32string ktu::u8::to_32(std::u8string &str) {
    std::u32string result;
    for (auto ptr = str.data(); ptr != str.data()+str.size();) {
        u32::write<dest_big_endian>(result, read(&ptr));
    }
    return result;
}


template <bool big_endian, typename ...Args>
constexpr uint32_t ktu::u16::impl::read(Args... args) {
    const auto &back = variadic::get<1>(args...);
    const auto *&cur = dereference<-1>(variadic::get<0>(args...));
    uint16_t c1, c2;
    auto inc = [&cur, &back]() {
        if constexpr (std::is_same<decltype(back), const empty&>::value) {
            return ktu::big_endian<big_endian>(*cur++);
        } else {
            return ktu::big_endian<big_endian>((cur == back) ? *cur : *cur++);
        } 
    };
    c1 = inc();
    if (0xD800 <= c1 && c1 < 0xDC00) {
        uint16_t c2 = inc();
        return (uint32_t)
            ((c1 & 0x3FF) << 10) +
            (c2 & 0x3FF) + 0x10000;
    }
    if (0xDC00 <= c1 && c1 < 0xE000) {
        return 0xFFFD;
    }
    return c1;
}

template <bool big_endian, typename ...Args>
constexpr uint32_t ktu::u16::impl::rread(Args... args) {
    const auto &front = variadic::get<1>(args...);
    const auto *&cur = dereference<-1>(variadic::get<0>(args...));
    uint16_t c1, c2;

    auto dec = [&cur, &front]() {
        if constexpr (std::is_same<decltype(front), const empty&>::value) {
            return ktu::big_endian<big_endian>(*cur--);
        } else {
            return ktu::big_endian<big_endian>((cur == front) ? *cur : *cur--);
        }
    };
    
    c1 = dec();

    if (0xDC00 <= c1 && c1 < 0xE000) {
        c2 = dec();
        return (uint32_t)
            ((c2 & 0x3FF) << 10) +
            (c1 & 0x3FF) + 0x10000;
    }

    if (0xD800 <= c1 && c1 < 0xDC00)
        return 0xFFFD;

    return c1;
}


template <bool big_endian, ktu::arithmetic<2> T>
constexpr size_t ktu::u16::impl::write(uint32_t codepoint, T *buf) {
    if (codepoint < 0xD800 || (0xDFFF < codepoint && codepoint < 0x10000)) {
        buf[0] = ktu::big_endian<big_endian, uint16_t>(codepoint);
        return 1;
    }
    codepoint -= 0x10000;

    buf[0] = ktu::big_endian<big_endian, uint16_t>(((0b1111'1111'1100'0000'0000 & codepoint) >> 10) + 0xD800);
    buf[1] = ktu::big_endian<big_endian, uint16_t>(((0b0000'0000'0011'1111'1111 & codepoint)) + 0xDC00);
    return 2;
}


template <bool src_big_endian, bool dest_big_endian>
inline std::u32string ktu::u16::to_32(std::u16string &str) {
    std::u32string result;
    for (auto ptr = str.data(); ptr != str.data()+str.size();) {
        u32::write<dest_big_endian>(result, read<src_big_endian>(&ptr));
    }
    return result;
}