#include <ktu/memory/buffer.hpp>
#include <ktu/algorithm.hpp>


ktu::buffer::buffer() {}
ktu::buffer::buffer(const buffer &other) {
    assign((void*)other.priv.data, other.priv.size);
}
ktu::buffer::buffer(buffer &&other) noexcept {
    swap(other);
}

ktu::buffer::~buffer() {
    if (priv.data) free(priv.data);
}

ktu::buffer & ktu::buffer::operator=(const buffer &other) {
    assign((void*)other.priv.data, other.priv.size);
    return *this;
}
ktu::buffer & ktu::buffer::operator=(buffer &&other) noexcept {
    swap(other);
    return *this;
}

void ktu::buffer::assign(void *ptr, size_type count) {
    resize<value_type, false>(count);
    memcpy(priv.data, ptr, count);
}

// bool ktu::buffer::pushf(const std::filesystem::path &path) {
//     if (!std::filesystem::exists(path)) return false;
//     std::ifstream f(path, ::std::ios::in | ::std::ios::binary);

//     const auto size = std::filesystem::file_size(path);
//     size_t curSize = size();
//     resize(curSize+size);
//     f.read(data<char>()+curSize, size);
//     return true;
// }



bool ktu::buffer::insertf(size_type index, const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) return false;
    std::ifstream f(path, ::std::ios::in | ::std::ios::binary);

    char buffer [256 * 1024];
    f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));

    const auto size = std::filesystem::file_size(path);
    resize(this->size()+size);
    f.read(data<char>()+index, size);
    return true;
}

bool ktu::buffer::assign(const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) return false;
    std::ifstream f(path, ::std::ios::in | ::std::ios::binary);

    char buffer [256 * 1024];
    f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));

    const auto size = std::filesystem::file_size(path);
    resize(size);
    f.read(data<char>(), size);
    return true;
}

// Capacity


void ktu::buffer::shrink_to_fit() {
    if (priv.size == priv.capacity)
        return;
    //a$llocate(priv.data, priv.capacity, priv.size);
    priv.data = (pointer)realloc(priv.data, priv.size);
    priv.capacity = priv.size;
    
    
    if (!priv.data)
        throw std::bad_alloc();
}


// Modifiers
void ktu::buffer::shift(size_type pos, size_type count) {
    priv.size += count;
    if (priv.size > priv.capacity) {
        priv.capacity = std::max((priv.size - count) * 2, priv.size);
        priv.data = (pointer)realloc(priv.data, priv.capacity);
        
        if (!priv.data)
            throw std::bad_alloc();
    }
    memcpy(priv.data + pos + count, priv.data + pos, priv.size - count - pos);
}



void ktu::buffer::erase(size_type pos, size_type count) {
    priv.size -= count;
    memcpy(priv.data + pos, priv.data + pos + count, priv.size - pos);
}
void ktu::buffer::push_back(void *ptr, size_type count) {
    size_type index = priv.size, newSize = index + count;
    if (newSize > priv.capacity)
        reserve(std::max(priv.size * 2, newSize));
    priv.size = newSize;
    memcpy(priv.data + index, ptr, count);
}
void ktu::buffer::swap(buffer &other) noexcept {
    std::swap(priv.data, other.priv.data);
    std::swap(priv.size, other.priv.size);
    std::swap(priv.capacity, other.priv.capacity);
}



bool ktu::buffer::operator==(const buffer &other) const {
    if (priv.size != other.priv.size) return false;
    auto l_it = begin<size_t>(),
        l_end = end<size_t>(),
        r_it = other.begin<size_t>();
    for (;l_it != l_end; l_it++, r_it++) {
        if (*l_it != *r_it) return false;
    }
    pointer l_ptr = (pointer)l_it.to_ptr(), r_ptr = (pointer)r_it.to_ptr(),
    ptrend = priv.data + priv.size;
    for (;l_ptr != ptrend; l_ptr++, r_ptr++) {
        if (*l_ptr != *r_ptr) return false;
    }
    return true;
}

std::weak_ordering ktu::buffer::operator<=> (const buffer &other) const {
    std::weak_ordering result = size() <=> other.size();
    if (result != std::weak_ordering::equivalent)
        return result;
    auto l_it = begin(), r_it = other.begin(),
        l_end = end();
    
    for (;l_it != l_end; l_it++, r_it++) {
        result = *l_it <=> *r_it;
        if (result != std::weak_ordering::equivalent)
            return result;
    }
    return std::weak_ordering::equivalent;
}



constexpr unsigned long ktu::readBin(const char *&ptr, long size) {
    unsigned long ret = 0; 
    while (size--) {
        char cur = *ptr;
        if (cur != '0' && cur != '1') return ret;
        ptr++;
        ret = (ret << 1) | (cur & 1);
    }
    return ret;
}
constexpr bool ktu::isBin(const char c) {
    return c == '0' || c == '1';
}

constexpr unsigned long ktu::readOct(const char *&ptr, long size) {
    unsigned long ret = 0; 
    while (size--) {
        char cur = *ptr;
        if (cur < '0' || cur > '7') return ret;
        ptr++;
        ret = (ret << 3) | (cur & 0x7);
    }
    return ret;
}
constexpr bool ktu::isOct(const char c) {
    return '0' <= c && c <= '7';
}


constexpr unsigned long ktu::readDec(const char *&ptr, long size) {
    unsigned long ret = 0; 
    while (size--) {
        char cur = *ptr;
        if (cur < '0' || cur > '9') return ret;
        ptr++;
        ret = (ret * 10) + (cur - '0');
    }
    return ret;
}
constexpr bool ktu::isDec(const char c) {
    return '0' <= c && c <= '9';
}

static constexpr const char Hextable[] = { 
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1, 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

constexpr unsigned long ktu::readHex(const char *&ptr, long size) {
    unsigned long ret = 0; 
    while (size--) {
        char cur = Hextable[(int)*ptr];
        if (cur == -1) return ret;
        ptr++;
        ret = (ret << 4) | cur;
    }
    return ret;
}
constexpr bool ktu::isHex(const char c) {
    return Hextable[c] != -1;
}



const char *ktu::readVariable(const char *ptr) {
    char value = *ptr;
    if (
            (value < 'A' || 'Z' < value) &&
            (value < 'a' || 'z' < value) &&
            value != '_'
    ) return ptr;
    while (true) {
        value = *ptr;
        next:
        if (
            (value < 'A' || 'Z' < value) &&
            (value < 'a' || 'z' < value) &&
            (value < '0' || '9' < value) &&
            value != '_'
        ) {
            return ptr;
        }
        ptr++;
    }
}
constexpr bool ktu::isVariableStart(const char c) {
    return ('A' <= c && c <= 'Z') ||
        ('a' <= c && c <= 'z') ||
        c == '_';
}



void ktu::buffer::input(const char *ptr, size_t ssize, long flags) {

    auto eos = ptr + ssize;
    char value;
    
    
    unsigned long (*readNumeric)(const char *&ptr, long size);
    bool (*checkNumeric)(char c);
    long size;

    size_t (*write)(uint32_t,void*);
    int factor;

    char buf[4];
    uint32_t codepoint;

    enum {
        unchanged,
        numeric,
        latin_1,
        ascii,
        unicode,
    };

    auto updateFormatting = [&ptr, &eos, &value, &readNumeric, &checkNumeric, &size, &write, &factor]() {
        while (ptr != eos) {
            while (ptr != eos && !isVariableStart(value = (*ptr))) {
                ptr++;
                if (value == ']') {
                    return unchanged;
                }
                    
            }
            if (ptr == eos)
                return unchanged;
            
            const char *tmp = ptr;
            ptr = ktu::readVariable(ptr);
            switch (ktu::hash(tmp, ptr-tmp)) {
                case ktu::hash("bin"):
                    readNumeric = readBin;
                    checkNumeric = isBin;
                    size = 8;
                    break;
                case ktu::hash("oct"):
                    readNumeric = readOct;
                    checkNumeric = isOct;
                    size = 3;
                    break;
                case ktu::hash("dec"):
                    readNumeric = readDec;
                    checkNumeric = isDec;
                    size = 3;
                    break;
                case ktu::hash("hex"):
                    readNumeric = readHex;
                    checkNumeric = isHex;
                    size = 2;
                    break;
                case ktu::hash("numeric"):
                    return numeric;
                case ktu::hash("ascii"):
                    return ascii;
                case ktu::hash("latin_1"):
                    return latin_1;
                case ktu::hash("utf_8"):
                    write = ktu::u8::ptr::write;
                    factor = 0;
                    return unicode;
                case ktu::hash("utf_16le"):
                    write = ktu::u16::ptr::write_le;
                    factor = 1;
                    return unicode;
                case ktu::hash("utf_16be"):
                    write = ktu::u16::ptr::write_be;
                    factor = 1;
                    return unicode;
                case ktu::hash("utf_32le"):
                    write = ktu::u32::ptr::write_le;
                    factor = 2;
                    return unicode;
                case ktu::hash("utf_32be"):
                    write = ktu::u32::ptr::write_be;
                    factor = 2;
                    return unicode;
            }


        }
        return unchanged;
    };
    bool inExpression = false;
    
    auto numericSection = [this, &updateFormatting, &ptr, &eos, &value, &readNumeric, &checkNumeric, &size, &inExpression]
    <bool enableInternalNumeric = true>
    () {
        if (inExpression) goto expression;
        while (ptr != eos) {
            value = *ptr;
            if constexpr (enableInternalNumeric) {
                if (checkNumeric(value)) {
                    push_back<value_type>(readNumeric(ptr, size));
                    continue;
                }
            }
            
            ptr++;
            if (value == ']')
                break;
            if (value == '[') {
                expression:
                inExpression = true;
                switch (updateFormatting()) {
                    case unchanged:
                        break;
                    case numeric:
                        return numeric;
                    case ascii:
                        return ascii;
                    case latin_1:
                        return latin_1;
                    case unicode:
                        return unicode;
                }
                inExpression = false;
            }
        }
        return unchanged;
    };

    switch (flags & ktu::buf_io::fmtflags::basefield) {
        case ktu::buf_io::fmtflags::bin:
            readNumeric = readBin;
            checkNumeric = isBin;
            size = 8;
            break;
        case ktu::buf_io::fmtflags::oct:
            readNumeric = readOct;
            checkNumeric = isOct;
            size = 3;
            break;
        case ktu::buf_io::fmtflags::dec:
            readNumeric = readDec;
            checkNumeric = isDec;
            size = 3;
            break;
        case ktu::buf_io::fmtflags::hex:
            readNumeric = readHex;
            checkNumeric = isHex;
            size = 2;
            break;
    }

    auto writeChar = [this, &value]() {
        push_back<value_type>(value);
    };

    
    switch (flags & ktu::buf_io::fmtflags::encodingfield) {
        case ktu::buf_io::fmtflags::numeric:
            while (ptr != eos) {
                value = *ptr;
                if (checkNumeric(value)) {
                    push_back<value_type>(readNumeric(ptr, size));
                } else {
                    ptr++;
                    if (value == '[') {
                        numericNumericSection:
                        switch (numericSection.operator()<false>()) {
                            case numeric:
                                goto numericNumericSection;
                            case ascii:
                                goto asciiNumericSection;
                            case latin_1:
                                goto latin_1NumericSection;
                            case unicode:
                                goto unicodeNumericSection;
                            default:
                                break;
                        }
                    }
                }
            }
            break;
        case ktu::buf_io::fmtflags::ascii:
            while (ptr != eos) {
                value = *ptr++;
                if (value == '[') {
                    asciiNumericSection:
                    switch (numericSection()) {
                        case numeric:
                            goto numericNumericSection;
                        case ascii:
                            goto asciiNumericSection;
                        case latin_1:
                            goto latin_1NumericSection;
                        case unicode:
                            goto unicodeNumericSection;
                        default:
                            break;
                    }
                } else if (value == '\\') {
                    value = ktu::u8::read(&ptr);
                    if (value == '[' || value == '\\')
                        writeChar();
                } else if (value == '\n') {
                    push_back<value_type>(' ');
                } else if (0x1F < (unsigned char)value && (unsigned char)value < 0x7F) {
                    writeChar();
                }
            }
            break;
        case ktu::buf_io::fmtflags::latin_1:
            while (ptr != eos) {
                value = ktu::u8::read(&ptr);
                if (value == '[') {
                    latin_1NumericSection:
                    switch (numericSection()) {
                        case numeric:
                            goto numericNumericSection;
                        case ascii:
                            goto asciiNumericSection;
                        case latin_1:
                            goto latin_1NumericSection;
                        case unicode:
                            goto unicodeNumericSection;
                        default:
                            break;
                    }
                } else if (value == '\\') {
                    value = ktu::u8::read(&ptr);
                    if (value == '[' || value == '\\')
                        writeChar();
                } else if (value == '\n') {
                    push_back<value_type>(' ');
                } else if ((0x1F < (unsigned char)value && (unsigned char)value < 0x7F) || ((unsigned char)value > 0xA0)) {
                    writeChar();
                }
            }
            break;
        case ktu::buf_io::fmtflags::utf_8:
            write = ktu::u8::ptr::write;
            factor = 0;
            break;
        case ktu::buf_io::fmtflags::utf_16le:
            write = ktu::u16::ptr::write_le;
            factor = 1;
            break;
        case ktu::buf_io::fmtflags::utf_16be:
            write = ktu::u16::ptr::write_be;
            factor = 1;
            break;
        case ktu::buf_io::fmtflags::utf_32le:
            write = ktu::u32::ptr::write_le;
            factor = 2;
            break;
        case ktu::buf_io::fmtflags::utf_32be:
            write = ktu::u32::ptr::write_be;
            factor = 2;
            break;
    }

    

    
    while (ptr < eos) {
        codepoint = ktu::u8::read(&ptr);
        
        if (codepoint == '[') {
            unicodeNumericSection:
            switch (numericSection()) {
                case numeric:
                    goto numericNumericSection;
                case ascii:
                    goto asciiNumericSection;
                case latin_1:
                    goto latin_1NumericSection;
                case unicode:
                    goto unicodeNumericSection;
                default:
                    break;
            }
        } else if (codepoint == '\\') {
            codepoint = ktu::u8::read(&ptr);
            if (codepoint == '[' || codepoint == '\\')
                goto write_codepoint;
        } else if (codepoint == '\n') {
            codepoint = ' ';
            goto write_codepoint;
        } else if (codepoint > 0x1F) {
            write_codepoint:
            size_t size = write(codepoint, &buf[0]) << factor;
            insert(end(), &buf[0], size);
        }
        

        
        //std::cout << size << '\n';

        //std::cout << ktu::buf_io::fmt << ktu::buf_io::output(&buf[0], size) << '\n';
    }
}

std::istream &ktu::operator>>(std::istream &is, buffer &buf) {
    long flags = is.iword(buf_io::manipFlagId);
    if (flags & buf_io::fmtflags::eosfield) {
        if (is.iword(buf_io::manipFlagId) & buf_io::fmtflags::fmtfield) {
            std::string str;
            is.seekg(0, std::ios::end);
            str.reserve(is.tellg());
            is.seekg(0, std::ios::beg);
            str.assign((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());
            buf.input(str);
            return is;
        }
        size_t size = buf.size();
        is.seekg(0, std::ios::end);
        buf.resize(size + is.tellg());
        is.seekg(0, std::ios::beg);
        is.read(buf.data<char>() + size, buf.size());
        return is;   
    }
    if (flags & buf_io::fmtflags::fmtfield) {
        std::string str;
        std::getline(is, str);
        buf.input(str);
        return is;
    }
    char c;
    while ((c = is.get()) != '\n') {
        buf.push_back(c);
    }
    
    return is;
}

#include <ktu/memory/view.hpp>
ktu::buffer::operator ktu::view() const {
    return view(priv.data, priv.data + priv.size);
}




#include <ktu/memory/reader.hpp>


ktu::buffer::operator ktu::reader() const {
    return reader(priv.data, priv.data + priv.size);
}
