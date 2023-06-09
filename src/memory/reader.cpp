#include <ktu/memory/reader.hpp>

#include <ktu/memory/view.hpp>

ktu::reader::operator ktu::view() const {
    return view;
}

#include <ktu/memory/buffer.hpp>

ktu::reader::operator ktu::buffer() const {
    return buffer(view.first, view.last);
}