#include <ktu/memory/file.hpp>
#include <fstream>
#include <iterator>
void ktu::file::write(const std::filesystem::path &path, char *first, char *last) {
    if (path.has_parent_path() && !std::filesystem::exists(path))
        std::filesystem::create_directories(path.parent_path());
    char buffer [256 * 1024];
    std::ofstream f(path);
    f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    std::copy(first, last, std::ostream_iterator<char>(f));
}


void ktu::file::read(const std::filesystem::path &path, void *dst, size_t filesize) {
    std::ifstream f(path, ::std::ios::in | ::std::ios::binary);
    char buffer [256 * 1024];
    f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    f.read((char*)dst, filesize);
}