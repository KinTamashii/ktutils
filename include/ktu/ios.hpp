#pragma once
#include <iomanip>
#include <fstream>
#include <ktu/unicode.hpp>

namespace ktu {
    struct io {
        /* Output a value in binary to an ostream. */
        static std::ostream &bits(std::ostream &os, unsigned long value);

        /* Output a value in binary to an ostream. */
        template <typename T>
        requires ((std::integral<T> || std::floating_point<T>) && !std::is_same<T,unsigned long>::value)
        static inline std::ostream &bits(std::ostream &os, T value) {
            return bits(os, *(typename ktu::to_unsigned_integral<sizeof(T)>::type*)&value);
        }

        /* Output a value in octal to an ostream. */
        static std::ostream &octal(std::ostream &os, unsigned long value);

        /* Output a value in decimal to an ostream. */
        static std::ostream &decimal(std::ostream &os, unsigned long value);

        /* Output a value in hexadecimal to an ostream. */
        static std::ostream &hexadecimal(std::ostream &os, unsigned long value);

        
    };
    class buf_io {
        private:
        
        public:
            static int const manipGroupingId;
            static int const manipFlagId;
            struct fmtflags {
                enum {
                    basefield =         0b0'00000000000000000000000'0'0'0'000'11,
                        hex =           0b0'00000000000000000000000'0'0'0'000'00,
                        dec =           0b0'00000000000000000000000'0'0'0'000'01,
                        oct =           0b0'00000000000000000000000'0'0'0'000'10,
                        bin =           0b0'00000000000000000000000'0'0'0'000'11,
                    
                    encodingfield =     0b0'00000000000000000000000'0'0'0'111'00,
                        numeric =       0b0'00000000000000000000000'0'0'0'000'00,
                        ascii =         0b0'00000000000000000000000'0'0'0'001'00,
                        latin_1 =       0b0'00000000000000000000000'0'0'0'010'00,
                        utf_8 =         0b0'00000000000000000000000'0'0'0'011'00,
                        utf_16le =      0b0'00000000000000000000000'0'0'0'100'00,
                        utf_16be =      0b0'00000000000000000000000'0'0'0'101'00,
                        utf_32le =      0b0'00000000000000000000000'0'0'0'110'00,
                        utf_32be =      0b0'00000000000000000000000'0'0'0'111'00,
                    
                    zfillfield =        0b0'00000000000000000000000'0'0'1'000'00,
                        zfill =         0b0'00000000000000000000000'0'0'0'000'00,
                        nozfill =       0b0'00000000000000000000000'0'0'1'000'00,

                    fmtinfofield =      0b0'00000000000000000000000'0'1'0'000'00,
                        nofmtinfo =     0b0'00000000000000000000000'0'0'0'000'00,
                        fmtinfo =       0b0'00000000000000000000000'0'1'0'000'00,

                    eosfield =          0b0'00000000000000000000000'1'0'0'000'00,
                        endl =          0b0'00000000000000000000000'0'0'0'000'00,
                        endf =          0b0'00000000000000000000000'1'0'0'000'00,

                    fmtfield =          0b1'00000000000000000000000'0'0'0'000'00,
                        nofmt =         0b0'00000000000000000000000'0'0'0'000'00,
                        fmt =           0b1'00000000000000000000000'0'0'0'000'00,
                };
            };

            /* Set the basefield to binary mode.
                Each numeric representation of bytes in the buffer will be output in binary. 
            */
            static std::ostream &bin(std::ostream &os);
            
            /* Set the basefield to octal mode.
                Each numeric representation of bytes in the buffer will be output in binary.
            */
            static std::ostream &oct(std::ostream &os);
            
            /* Set the basefield to decimal mode.
                Each numeric representation of bytes in the buffer will be output in octal.
            */
            static std::ostream &dec(std::ostream &os);
            
            /* Set the basefield to hexadecimal mode.
                Each numeric representation of bytes in the buffer will be output in hexadecimal.
                On by default.
            */
            static std::ostream &hex(std::ostream &os);


            /* Set the encoding field to numeric.
                All bytes are output numerically.
                On by default.
            */
            static std::ostream &numeric(std::ostream &os);

            /* Set the encoding field to ascii.
                All valid printable ascii bytes including spaces (0x20) will be output in ascii.
                All invalid ascii or unprintable bytes will be output numerically.
            */
            static std::ostream &ascii(std::ostream &os);

            /* Set the encoding field to latin-1.
                All valid printable latin-1 bytes including spaces (0x20) will be output in latin-1.
                All invalid latin-1 or unprintable bytes will be output numerically.
            */
            static std::ostream &latin_1(std::ostream &os);

            /* Set the encoding field to utf-8.
                All valid printable utf-8 bytes including spaces (0x20) will be output in utf-8.
                All invalid utf-8 or unprintable bytes will be output numerically.
            */
            static std::ostream &utf_8(std::ostream &os);

            /* Set the encoding field to utf-16le.
                All valid printable utf-16le bytes including spaces (0x20) will be output in utf-16le.
                All invalid utf-16le or unprintable bytes will be output numerically.
            */
            static std::ostream &utf_16le(std::ostream &os);

            /* Set the encoding field to utf-16be.
                All valid printable utf-16be bytes including spaces (0x20) will be output in utf-16be.
                All invalid utf-16be or unprintable bytes will be output numerically.
            */
            static std::ostream &utf_16be(std::ostream &os);

            /* Set the encoding field to utf-32le.
                All valid printable utf-32le bytes including spaces (0x20) will be output in utf-32le.
                All invalid utf-32le or unprintable bytes will be output numerically.
            */
            static std::ostream &utf_32le(std::ostream &os);

            /* Set the encoding field to utf-32be.
                All valid printable utf-32be bytes including spaces (0x20) will be output in utf-32be.
                All invalid utf-32be or unprintable bytes will be output numerically.
            */
            static std::ostream &utf_32be(std::ostream &os);

            /* Fill leading zeros to the relative byte width of the base.
                (bin -> 8 bits -> 8)
                (oct -> 3 places -> 3)
                (dec -> 3 places -> 3)
                (hex -> 2 nibbles -> 2)
                On by default.
            */
            static std::ostream &zfill(std::ostream &os);

            /* Disable leading zeros.
            */
            static std::ostream &nozfill(std::ostream &os);

            /* Include info relating to the formatting in the beginning of the data.
                This can be parsed to automatically determine the encoding and base by an input function.
            */
            static std::ostream &fmtinfo(std::ostream &os);

            /* Do not include info relating to the formatting in the beginning of the data.
            */
            static std::ostream &nofmtinfo(std::ostream &os);

            /* Enable formatted output.
                The buffer output is converted to a human readable format.
            */
            static std::ostream &fmt(std::ostream &os);

            /* Raw output.
                The buffer output is identical to the contents of the buffer.
                On by default.
            */
            static std::ostream &nofmt(std::ostream &os);

            /* Terminate reading the stream at the end of the line. 
                On by default.
            */
            static std::istream &endl(std::istream &is);

            /* Read the entire stream.
            */
            static std::istream &endf(std::istream &is);

            /* Enable formatted input.
                The buffer input is converted from a human readable format to binary.
            */
            static std::istream &fmt(std::istream &os);

            /* Raw output.
                The buffer input is identical to the contents of the souce.
                On by default.
            */
            static std::istream &nofmt(std::istream &os);

            
            /* Set the amount of numerically represented bytes to place between spaces.
                Set to zero for no spaces in-between.
            */
            class setgrouping {
                private:
                    unsigned long grouping;
                public:
                    setgrouping(unsigned long grouping) : grouping(grouping) {}
                    friend std::ostream &operator<<(std::ostream& os, const setgrouping &obj) {
                        os.iword(manipGroupingId) = obj.grouping;
                        return os;
                    }
            };

            /* Output a buffer to an ostream.
            */
            class output {
                private:
                    const void *ptr;
                    size_t size;
                public:
                    output(const void *ptr, size_t size) : ptr(ptr), size(size) {}
                    friend std::ostream &operator<<(std::ostream& os, const output &obj);
            };



            

            

            
    };
    

    
};

// std::ifstream& operator>>(std::ifstream &ifs, std::string &str);