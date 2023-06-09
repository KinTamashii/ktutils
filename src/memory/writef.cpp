#include <ktu/memory/writef.hpp>
#include <fstream>
#include <iterator>
void ktu::writef(const std::filesystem::path &path, char *first, char *last) {
    if (path.has_parent_path() && !std::filesystem::exists(path))
        std::filesystem::create_directories(path.parent_path());
    char buffer [256 * 1024];
    std::ofstream f(path);
    f.rdbuf()->pubsetbuf(buffer, sizeof(buffer));
    std::copy(first, last, std::ostream_iterator<char>(f));
}
