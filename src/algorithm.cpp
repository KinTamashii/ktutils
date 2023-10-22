#include <ktu/algorithm.hpp>

template size_t ktu::impl::hash<true, void>(const void *, size_t);
template size_t ktu::impl::hash<false, void>(const void *, size_t);