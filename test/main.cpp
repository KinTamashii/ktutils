// #_define __KTU_DEBUG__

#include <ktu/array.hpp>
#include <ktu/memory.hpp>
#include <ktu/unicode.hpp>
#include <ktu/algorithm.hpp>
#include <iostream>
#include <sstream>

#include <array>

#include <ktu/vector.hpp>



#define EXIT(__Message) \
    do {std::cout << __Message << '\n'; exit(-1);} while(0)





#include <experimental/functional>
int main() {

    // std::string str = "fuck";


    // std::cout << ktu::file::read(str, 2, "/Users/kin_tamashii/Desktop/c++/projects/released/ktutils/test/main.cpp") << '\n';
    // std::cout << str << '\n';


    ktu::buffer buffer;


    //ktu::file::read(buffer, 0, "/Users/kin_tamashii/Desktop/c++/projects/released/ktutils/test/main.cpp");

    buffer.insertf(buffer.end(), "/Users/kin_tamashii/Desktop/c++/projects/released/ktutils/test/main.cpp");


    

    std::cout <<
        //ktu::buf_io::fmt << ktu::buf_io::hex <<
        buffer << '\n';


    // ktu::buffer buffer("/Users/kin_tamashii/Desktop/c++/projects/in-progress/ktutils/test/main.cpp");

    // ktu::reader reader(buffer);

    // auto string = ktu::array_null_range("define");

    
    // reader.find<[](int _c){return isalpha(_c) || _c == '_';}>();
    
    // //reader.seek(reader.cur()+100);
    // // reader.skip(100);
    // // reader.find('\'');
    // std::cout << std::string(reader.cur<char>(), 10) << '\n';
    

    //std::cout << isalpha('_') << '\n';

    
    /*
    
    
    
    
    */
    // ktu::reader reader(buffer);


    // reader.find<ishexnumber>();

    // std::cout << reader.slice(reader.cur(), reader.cur()+20) << '\n';
    

    return 0;
}


/*



    token1 token2 token3 quote[
        "
        '
        R"(
        `{}`
    ]



ktu::buffer buffer{(uint8_t)0xFF,(uint8_t)0xFF,(uint8_t)0xFF,(uint8_t)0xFF};

    size_t position = 0;
    size_t segmentSize = 12;

    buffer.shift(position, segmentSize);
    
    
    uint8_t *segmentPtr = buffer.data() + position;
    for (uint8_t i = 0; i < segmentSize; ++i) {
        segmentPtr[i] = i;
    }

    std::cout <<
        ktu::buf_io::fmt <<
        ktu::buf_io::setgrouping(1) <<
        buffer <<
    '\n';

    size_t newSegmentSize = 8;

    buffer.erase(position+newSegmentSize, segmentSize-newSegmentSize);
    // buffer.shift(position);


    std::cout <<
        ktu::buf_io::fmt <<
        ktu::buf_io::setgrouping(1) <<
        buffer <<
    '\n';
    '\o{176}!'
    '\x{447E}!'
    '\x447E!'
    '\147!'
    '\u{000000DFF}s'
    '\U000000FF'
    '\N{EMOJI MODIFIER\
 FITZPATRICK TYPE-6}'

*/

//cmake --install ./build
//