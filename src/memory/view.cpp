#include <ktu/memory/view.hpp>



#include <ktu/memory/reader.hpp>


ktu::view::operator ktu::reader() const {
    return reader(first, last);
}

#include <ktu/memory/buffer.hpp>

ktu::view::operator ktu::buffer() const {
    return buffer(first, last);
}