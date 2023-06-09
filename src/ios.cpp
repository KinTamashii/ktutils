#include <ktu/ios.hpp>

// void ktu::bin(std::ostream &os, uint8_t value) {
//     for (int j = 0; j < 8; value <<= 1, j++) {
//         os << ((int)value & 0b10000000 ? 1 : 0);
//     }
// };
// void ktu::oct(std::ostream &os, uint8_t value) {
//     os << std::setfill('0') << std::setw(3) << std::uppercase << std::oct << (int)value << std::dec;
// };
// void ktu::hex(std::ostream &os, uint8_t value) {
//     os << std::setfill('0') << std::setw(2) << std::uppercase << std::hex << (int)value << std::dec;
// };
// void ktu::ascii(std::ostream &os, uint8_t value) {
//     if (0x20 < value && value < 0x7E && value != '[') {
//         os << (char)value;
//     } else {
//         os << '[';
//         hex(os, value);
//         os << ']';
//     }
// };




std::ostream &ktu::io::bits(std::ostream &os, unsigned long value) {
    unsigned long mask = 1ULL << ((sizeof(unsigned long) << 3ULL) - 1ULL);
    int i = ((sizeof(unsigned long) << 3ULL)) - 1;
    while ((mask > 1)&& !(value & mask)) {mask >>= 1; i--;}

    os.width(std::max(os.width() - i, (std::streamsize)0));
    os.fill();


    for (;mask; mask >>= 1) {
        os << !!(value & mask);
    }
    return os;
}

std::ostream &ktu::io::octal(std::ostream &os, unsigned long value) {
    return os << std::oct << value;
}

std::ostream &ktu::io::decimal(std::ostream &os, unsigned long value) {
    return os << std::dec << value;
}

std::ostream &ktu::io::hexadecimal(std::ostream &os, unsigned long value) {
    return os << std::hex << value;
}


int const ktu::buf_io::manipGroupingId = std::ios_base::xalloc();
int const ktu::buf_io::manipFlagId = std::ios_base::xalloc();

std::ostream &ktu::buf_io::bin(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::basefield);
    os.iword(manipFlagId) |= fmtflags::bin;
    return os;
}

std::ostream &ktu::buf_io::oct(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::basefield);
    os.iword(manipFlagId) |= fmtflags::oct;
    return os;
}

std::ostream &ktu::buf_io::dec(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::basefield);
    os.iword(manipFlagId) |= fmtflags::dec;
    return os;
}

std::ostream &ktu::buf_io::hex(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::basefield);
    os.iword(manipFlagId) |= fmtflags::hex;
    return os;
}

std::ostream &ktu::buf_io::numeric(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::numeric;
    return os;
}
std::ostream &ktu::buf_io::ascii(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::ascii;
    return os;
}
std::ostream &ktu::buf_io::latin_1(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::latin_1;
    return os;
}
std::ostream &ktu::buf_io::utf_8(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::utf_8;
    return os;
}
std::ostream &ktu::buf_io::utf_16le(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::utf_16le;
    return os;
}
std::ostream &ktu::buf_io::utf_16be(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::utf_16be;
    return os;
}
std::ostream &ktu::buf_io::utf_32le(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::utf_32le;
    return os;
}
std::ostream &ktu::buf_io::utf_32be(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::encodingfield);
    os.iword(manipFlagId) |= fmtflags::utf_32be;
    return os;
}

std::ostream &ktu::buf_io::zfill(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::zfillfield);
    return os;
}
std::ostream &ktu::buf_io::nozfill(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::zfillfield);
    os.iword(manipFlagId) |= fmtflags::nozfill;
    return os;
}

std::ostream &ktu::buf_io::fmtinfo(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::fmtinfofield);
    os.iword(manipFlagId) |= fmtflags::fmtinfo;
    return os;
}
std::ostream &ktu::buf_io::nofmtinfo(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::fmtinfofield);
    return os;
}

std::ostream &ktu::buf_io::fmt(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::fmtfield);
    os.iword(manipFlagId) |= fmtflags::fmt;
    return os;
}
std::ostream &ktu::buf_io::nofmt(std::ostream &os) {
    os.iword(manipFlagId) &= ~(fmtflags::fmtfield);
    return os;
}

std::istream &ktu::buf_io::endl(std::istream &is) {
    is.iword(manipFlagId) &= ~(fmtflags::eosfield);
    return is;
}
std::istream &ktu::buf_io::endf(std::istream &is) {
    is.iword(manipFlagId) &= ~(fmtflags::eosfield);
    is.iword(manipFlagId) |= fmtflags::endf;
    return is;
}

std::istream &ktu::buf_io::fmt(std::istream &is) {
    is.iword(manipFlagId) &= ~(fmtflags::fmtfield);
    is.iword(manipFlagId) |= fmtflags::fmt;
    return is;
}
std::istream &ktu::buf_io::nofmt(std::istream &is) {
    is.iword(manipFlagId) &= ~(fmtflags::fmtfield);
    return is;
}


/*

void ktu::write(const std::filesystem::path &path, char *first, char *last) {
    if (!std::filesystem::exists(path))
        std::filesystem::create_directory(path.parent_path());
    char buffer [256 * 1024];
    std::ofstream f(path);
    f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    std::copy(first, last, std::ostream_iterator<char>(f));
}
*/
#include <iterator>

std::ostream &ktu::operator<<(std::ostream &os, const buf_io::output &obj) {
    if (!obj.size) return os;
    long flags = os.iword(ktu::buf_io::manipFlagId);
    if (!(flags & ktu::buf_io::fmtflags::fmt)) {
        char buffer [256 * 1024];
        os.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
        std::copy((const char*)obj.ptr, (const char*)obj.ptr + obj.size, std::ostream_iterator<char>(os));
        return os;
    }
    if (flags & ktu::buf_io::fmtflags::fmtinfo) {
        os << "[[";
        switch (flags & ktu::buf_io::fmtflags::encodingfield) {
            case ktu::buf_io::fmtflags::numeric:
                os << "numeric";
                break;
            case ktu::buf_io::fmtflags::ascii:
                os << "ascii";
                break;
            case ktu::buf_io::fmtflags::latin_1:
                os << "latin_1";
                break;
            case ktu::buf_io::fmtflags::utf_8:
                os << "utf_8";
                break;
            case ktu::buf_io::fmtflags::utf_16le:
                os << "utf_16le";
                break;
            case ktu::buf_io::fmtflags::utf_16be:
                os << "utf_16be";
                break;
            case ktu::buf_io::fmtflags::utf_32le:
                os << "utf_32le";
                break;
            case ktu::buf_io::fmtflags::utf_32be:
                os << "utf_32be";
                break;
        }
        os << ' ';
        switch (flags & ktu::buf_io::fmtflags::basefield) {
            case ktu::buf_io::fmtflags::bin:
                os << "bin";
                break;
            case ktu::buf_io::fmtflags::oct:
                os << "oct";
                break;
            case ktu::buf_io::fmtflags::dec:
                os << "dec";
                break;
            case ktu::buf_io::fmtflags::hex:
                os << "hex";
                break;
        }
        os << "]]";
    }

    std::ios tmpState(nullptr);
    tmpState.copyfmt(os);
    
    unsigned long grouping = os.iword(ktu::buf_io::manipGroupingId);
    int width;
    std::ostream &(*formatted)(std::ostream&, unsigned long);
    bool nozfill = (flags & ktu::buf_io::fmtflags::nozfill);
    switch (flags & ktu::buf_io::fmtflags::basefield) {
        case ktu::buf_io::fmtflags::bin:
            width = (nozfill) ? 0 : 8;
            formatted = &ktu::io::bits;
            break;
        case ktu::buf_io::fmtflags::oct:
            width = (nozfill) ? 0 : 3;
            formatted = &ktu::io::octal;
            break;
        case ktu::buf_io::fmtflags::dec:
            width = (nozfill) ? 0 : 3;
            formatted = &ktu::io::decimal;
            break;
        case ktu::buf_io::fmtflags::hex:
            width = (nozfill) ? 0 : 2;
            os << std::uppercase;
            formatted = &ktu::io::hexadecimal;
            break;
    }
    

    const uint8_t * cur = (const uint8_t*)obj.ptr, *end = cur + obj.size;
    switch (flags & ktu::buf_io::fmtflags::encodingfield) {
        case ktu::buf_io::fmtflags::numeric: {
            int j = 0;
            numeric_format_loop:
                os << std::setfill('0') << std::setw(width);
                formatted(os, *cur);
                if (++cur != end) {
                    if (++j == grouping) {
                        os << ' ';
                        j = 0;
                    } else if (nozfill) {
                        os << ':';
                    }
                    goto numeric_format_loop;
                }
            break;
        }
        case ktu::buf_io::fmtflags::ascii: {
            uint8_t value = *cur;
            while (cur != end) {
                if (0x1F < value && value < 0x7F)
                    do {
                        if (value == '\\' || value == '[') os << '\\';
                        os << value;
                        value = *(++cur);
                    } while (cur != end && 0x1F < value && value < 0x7F);
                else {
                    int j = 0;
                    os << '[';
                    ascii_invalid_loop:
                        os << std::setfill('0') << std::setw(width);
                        formatted(os, value);
                        value = *(++cur);
                        if (cur != end && (value < 0x20 || value > 0x7E)) {
                            if (++j == grouping) {
                                os << ' ';
                                j = 0;
                            } else if (nozfill) {
                                os << ':';
                            }
                            goto ascii_invalid_loop;
                        }
                    os << ']';
                }
            }
            break;
        }
        case ktu::buf_io::fmtflags::latin_1: {
            uint8_t value = *cur;
            while (cur != end) {
                if ((0x1F < value && value < 0x7F) || (value > 0xA0))
                    do {
                        char buf[4];
                        size_t size = ktu::u8::write(value, &buf[0]);
                        if (value == '\\' || value == '[') os << '\\';
                        os.write(&buf[0], size);
                        value = *(++cur);
                    } while (((0x1F < value && value < 0x7F) || (value > 0xA0)) && cur != end);
                else {
                    int j = 0;
                    os << '[';
                    latin_1_invalid_loop:
                        os << std::setfill('0') << std::setw(width);
                        formatted(os, value);
                        value = *(++cur);
                        
                        if ((value < 0x20 || (value > 0x7E && value < 0xA1)) && cur != end) {
                            if (++j == grouping) {
                                os << ' ';
                                j = 0;
                            } else if (nozfill) {
                                os << ':';
                            }
                            goto latin_1_invalid_loop;
                        }
                    os << ']';
                }
            }
            break;
        }
        default: {
            uint32_t (*read)(const void**);
            switch (flags & ktu::buf_io::fmtflags::encodingfield) {
                case ktu::buf_io::fmtflags::utf_8:
                    read = ktu::u8::ptr::read_ref;
                    break;
                case ktu::buf_io::fmtflags::utf_16le:
                    read = ktu::u16::ptr::read_ref_le;
                    break;
                case ktu::buf_io::fmtflags::utf_16be:
                    read = ktu::u16::ptr::read_ref_be;
                    break;
                case ktu::buf_io::fmtflags::utf_32le:
                    read = ktu::u32::ptr::read_ref_le;
                    break;
                case ktu::buf_io::fmtflags::utf_32be:
                    read = ktu::u32::ptr::read_ref_be;
                    break;
            }
            char buf[4];
            const uint8_t *tmp = cur;
            uint32_t codepoint = read((const void**)&tmp);
            size_t size;
            while (cur < end) {
                if (ktu::validUnicode(codepoint) && 0x1F < codepoint)
                    do {
                        cur = tmp;
                        size = ktu::u8::write(codepoint, &buf[0]);
                        if (codepoint == '\\' || codepoint == '[') os << '\\';
                        os.write(&buf[0], size);
                        codepoint = read((const void**)&tmp);
                    } while (ktu::validUnicode(codepoint) && 0x1F < codepoint && cur < end);
                else {
                    int j = 0;
                    os << '[';
                    unicode_invalid_loop:
                        if (cur != tmp) {
                            unicode_numeric_format_loop:
                                os << std::setfill('0') << std::setw(width);
                                formatted(os, *cur++);
                                if (cur != tmp && cur != end) {
                                    if (++j == grouping) {
                                        os << ' ';
                                        j = 0;
                                    } else if (nozfill) {
                                        os << ':';
                                    }
                                    goto unicode_numeric_format_loop;
                                }
                        }
                        codepoint = read((const void**)&tmp);
                        if ((!ktu::validUnicode(codepoint) || codepoint < 0x20 || codepoint == '[') && cur < end) {
                            if (++j == grouping) {
                                os << ' ';
                                j = 0;
                            } else if (nozfill) {
                                os << ':';
                            }
                            goto unicode_invalid_loop;
                        }
                    os << ']';
                }
                
            }
            break;
        }
    }
    os.copyfmt(tmpState);
    return os;
}


// std::ifstream& operator>>(std::ifstream &ifs, std::string &str) {
//     ifs.seekg(0, std::ios::end);
//     str.reserve(ifs.tellg());
//     ifs.seekg(0, std::ios::beg);
//     str.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
//     return ifs;
// }