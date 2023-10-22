#ifndef KTU__MULTI_STRING_HPP
#define KTU__MULTI_STRING_HPP
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>
#include <ktu/memory/file.hpp>

namespace ktu {
template <
    class CharT
> class basic_multi_string {
    public:
    using char_type = CharT;
    using string_type = std::basic_string<char_type>;
    using string_type_iterator = typename string_type::iterator;
    using string_type_const_iterator = typename string_type::const_iterator;
    using string_type_reverse_iterator = typename string_type::reverse_iterator;
    using string_type_const_reverse_iterator = typename string_type::const_reverse_iterator;
    using size_type = size_t;
    using ssize_type = ssize_t;
    static constexpr size_type npos = string_type::npos;
    using string_view_type = std::basic_string_view<char_type>;
    struct node_type {
        size_type index;
        size_type size;
    };
    class const_reverse_iterator;
    class const_iterator {
        public:
            using iterator_category = std::random_access_iterator_tag;
            using iterator_concept = std::random_access_iterator_tag;
            const_iterator(const basic_multi_string &parent, node_type *data) : priv{.parent=parent,.data=data} {}
            const_iterator &operator++() {
                ++priv.data;
                return *this;
            }
            const_iterator &operator++(int) {
                const_iterator &prev = *this;
                ++(*this);
                return prev;
            }
            const_iterator &operator--() {
                --priv.data;
                return *this;
            }
            const_iterator &operator--(int) {
                const_iterator &prev = *this;
                --(*this);
                return prev;
            }
            const_iterator &operator+=(ssize_type diff) {
                priv.data += diff;
                return *this;
            }
            const_iterator &operator-=(ssize_type diff) {
                priv.data -= diff;
                return *this;
            }

            friend size_type operator-(const_iterator lhs, const_iterator rhs) {
                return lhs.priv.data - rhs.priv.data;
            }

            friend const_iterator operator+(const_iterator obj, ssize_type diff) {
                return const_iterator(obj.priv.parent, obj.priv.data + diff);
            }
            friend const_iterator operator-(const_iterator obj, ssize_type diff) {
                return const_iterator(obj.priv.parent, obj.priv.data - diff);
            }
            string_view_type operator*() {
                return priv.data->index == basic_multi_string::npos ?
                    "" : string_view_type{priv.parent.priv.string.c_str() + priv.data->index, priv.data->size};
            }

            bool operator==(const const_iterator &other) const {
                return this->priv.data == other.priv.data;
            }
            
            std::weak_ordering operator<=>(const const_iterator &other) const {
                return this->priv.data <=> other.priv.data;
            }
            const node_type *base() {
                return priv.data;
            }
            const_reverse_iterator reverse() {
                return const_reverse_iterator(priv.parent, priv.data);
            }
            const char_type *data() const {
                return priv.parent.priv.string.data() + priv.data->index;
            }
            size_t index() const {
                return priv.data->index;
            }
            size_t size() const {
                return priv.data->size;
            }

        private:
            struct priv {
                node_type *data;
                const basic_multi_string &parent;
            } priv;
            friend basic_multi_string;
    };
    private:
    struct priv {
        static void nodes_shift_add(basic_multi_string::const_iterator pos, size_type shift) {
            pos.priv.data->size += shift;
            ++pos;
            while (pos != pos.priv.parent.end()) {
                pos.priv.data->index += shift;
                ++pos;
            }
        }
        static void nodes_shift_subtract(const_iterator pos, size_type shift) {
            pos.priv.data->size -= shift;
            ++pos;
            while (pos != pos.priv.parent.end()) {
                pos.priv.data->index -= shift;
                ++pos;
            }
        }
        static void nodes_shift_change(const_iterator pos, ssize_type shift) {
            pos.priv.data->size += shift;
            ++pos;
            while (pos != pos.priv.parent.end()) {
                pos.priv.data->index += shift;
                ++pos;
            }
        }
        string_type string;
        std::vector<node_type> nodes;
        size_type buildingIndex = 0;
    } priv;

    public:
    
    class const_reverse_iterator {
        public:
            const_reverse_iterator(const basic_multi_string &parent, node_type *data) : priv{.iterator=const_iterator(parent, data)} {}
            const_reverse_iterator &operator++() {
                --priv.iterator;
                return *this;
            }
            const_reverse_iterator &operator++(int) {
                const_iterator &prev = *this;
                ++(*this);
                return prev;
            }
            const_reverse_iterator &operator--() {
                ++priv.iterator;
                return *this;
            }
            const_reverse_iterator &operator--(int) {
                const_reverse_iterator &prev = *this;
                --(*this);
                return prev;
            }
            const_reverse_iterator &operator+=(ssize_type diff) {
                priv.iterator -= diff;
                return *this;
            }
            const_reverse_iterator &operator-=(ssize_type diff) {
                priv.iterator += diff;
                return *this;
            }

            friend size_type operator-(const_reverse_iterator lhs, const_reverse_iterator rhs) {
                return rhs.priv.iterator - lhs.priv.iterator;
            }

            friend const_reverse_iterator operator+(const_reverse_iterator obj, ssize_type diff) {
                return (obj.priv.iterator - diff).reverse();
            }
            friend const_reverse_iterator operator-(const_reverse_iterator obj, ssize_type diff) {
                return (obj.priv.iterator + diff).reverse();
            }
            string_view_type operator*() {return *priv.iterator;}

            bool operator==(const const_reverse_iterator &other) const {
                return this->priv.iterator == other.priv.iterator;
            }
            
            std::weak_ordering operator<=>(const const_reverse_iterator &other) const {
                return other.priv.iterator <=> this->priv.iterator;
            }
            const_iterator base() {
                return const_iterator(priv.iterator.parent, priv.iterator.data);
            }
            const char_type *data() const {
                return priv.iterator.data();
            }
            size_t index() const {
                return priv.iterator.index();
            }
            size_t size() const {
                return priv.iterator.size();
            }

        private:
            struct priv {
                const_iterator iterator;
            } priv;
    };

    /*`Main`*/
        /*`Constructor`*/
            basic_multi_string() noexcept {}
            basic_multi_string(size_type count, std::initializer_list <char_type> ilist = {}) {
                priv.string.reserve(ilist.size()*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(ilist);
                }
            }
            basic_multi_string(size_type count, char_type ch) {
                priv.string.reserve(count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(ch);
                }
            }
            basic_multi_string(size_type count, const char_type *s) : basic_multi_string(count, s, strlen(s)) {}
            basic_multi_string(size_type count, const char_type *s, size_type length) {
                priv.string.reserve(length*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(s, length);
                }
            }
            basic_multi_string(size_type count, string_view_type value) {
                priv.string.reserve(value.size()*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(value);
                }
            }
            basic_multi_string(size_type count, const string_type &value) {
                priv.string.reserve(value.size()*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(value);
                }
            }
            template <class InputIt>
            basic_multi_string(InputIt first, InputIt last) {
                priv.nodes.reserve(last-first);
                for (InputIt it = first; it != last; ++it) {
                    push_back(*it);
                }
            }
            template <typename T>
            basic_multi_string(std::initializer_list<T> init) {
                if constexpr (std::is_same<T, char_type>::value) {
                    priv.string.reserve(init.size());
                    push_back(init);
                } else {
                    priv.nodes.reserve(init.size());
                    for (auto item : init) {
                        push_back(item);
                    }
                }
            }
            basic_multi_string(const basic_multi_string &other) {
                priv.string = other.priv.string;
                priv.nodes = other.priv.nodes;
            }
            basic_multi_string(basic_multi_string &&other) noexcept {
                priv.string.swap(other.priv.string);
                priv.nodes.swap(other.priv.nodes);
            }
        /*`Operator=`*/
            basic_multi_string &operator=(const basic_multi_string &other) {
                priv.string = other.priv.string;
                priv.nodes = other.priv.nodes;
                return *this;
            }
            basic_multi_string &operator=(basic_multi_string &&other) {
                priv.string.swap(other.priv.string);
                priv.nodes.swap(other.priv.nodes);
                return *this;
            }
            template <typename T>
            basic_multi_string &operator=(std::initializer_list<T> init) {
                assign(init);
            }
        /*`Assign`*/
            void assign(size_type count, std::initializer_list <char_type> ilist = {}) {
                clear();
                priv.string.reserve(ilist.size()*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(ilist);
                }
            }
            void assign(size_type count, char_type ch) {
                clear();
                priv.string.reserve(count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(ch);
                }
            }
            void assign(size_type count, const char_type *s) {
                assign(count, s, strlen(s));
            }
            void assign(size_type count, const char_type *s, size_type length) {
                clear();
                priv.string.reserve(length*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(s, length);
                }
            }
            void assign(size_type count, string_view_type value) {
                clear();
                priv.string.reserve(value.size()*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(value);
                }
            }
            void assign(size_type count, const string_type &value) {
                clear();
                priv.string.reserve(value.size()*count);
                priv.nodes.reserve(count);
                for (size_type i = 0; i < count; ++i) {
                    push_back(value);
                }
            }
            template <class InputIt>
            void assign(InputIt first, InputIt last) {
                clear();
                priv.nodes.reserve(std::distance(first, last));
                for (InputIt it = first; it != last; ++it) {
                    push_back(*it);
                }
            }
            template <typename T>
            void assign(std::initializer_list<T> init) {
                clear();
                if constexpr (std::is_same<T, char_type>::value) {
                    priv.string.reserve(init.size());
                    push_back(init);
                } else {
                    priv.nodes.reserve(init.size());
                    for (auto item : init) {
                        push_back(item);
                    }
                }
            }

    /*`Element Access`*/
        /*`Cumulative String Elements`*/
            char_type &cumulative_at(size_type pos) {
                return priv.string.at(pos);
            }
            const char_type &cumulative_at(size_type pos) const {
                return priv.string.at(pos);
            }
            char_type &cumulative_access(size_type pos) {
                return priv.string[pos];
            }
            const char_type &cumulative_access(size_type pos) const {
                return priv.string[pos];
            }
            char_type &cumulative_front() {
                return priv.string.front();
            }
            const char_type &cumulative_front() const {
                return priv.string.front();
            }
            char_type &cumulative_back() {
                return priv.string.back();
            }
            const char_type &cumulative_back() const {
                return priv.string.back();
            }
            const char_type *cumulative_c_str() const {
                return priv.string.c_str();
            }
            char_type *cumulative_data() {
                return priv.string.data();
            }
        /*`Building Substring Elements`*/
            char_type &building_at(size_type pos) {
                return priv.string.at(priv.buildingIndex + pos);
            }
            const char_type &building_at(size_type pos) const {
                return priv.string.at(priv.buildingIndex + pos);
            }
            char_type &building_access(size_type pos) {
                return priv.string[priv.buildingIndex + pos];
            }
            const char_type &building_access(size_type pos) const {
                return priv.string[priv.buildingIndex + pos];
            }
            char_type &building_front() {
                return priv.string[priv.buildingIndex];
            }
            const char_type &building_front() const {
                return priv.string[priv.buildingIndex];
            }
            char_type &building_back() {
                return priv.string.back();
            }
            const char_type &building_back() const {
                return priv.string.back();
            }
            const char_type *building_c_str() const {
                return &priv.string[priv.buildingIndex];
            }
            char_type *building_data() {
                return &priv.string[priv.buildingIndex];
            }
        /*`Standard Elements`*/
            string_view_type at(size_type pos) const {
                if (pos >= priv.nodes.size()) {
                    throw std::out_of_range("Index out of bounds.");
                }
                return string_view_type(priv.string.c_str() + priv.nodes[pos].index, priv.nodes[pos].size);
            }
            string_view_type operator[](size_type pos) const {
                return string_view_type(priv.string.c_str() + priv.nodes[pos].index, priv.nodes[pos].size);
            }
            string_view_type front() const {
                return string_view_type(priv.string.c_str() + priv.nodes.front().index, priv.nodes.front().size);
            }
            string_view_type back() const {
                return string_view_type(priv.string.c_str() + priv.nodes.back().index, priv.nodes.back().size);
            }

    /*`Iterators`*/
        /*`Cumulative Iterators`*/
            string_type_iterator cumulative_begin() {
                return priv.string.begin();
            }
            string_type_const_iterator cumulative_begin() const {
                return priv.string.begin();
            }
            string_type_const_iterator cumulative_cbegin() const {
                return priv.string.begin();
            }
            string_type_iterator cumulative_end() {
                return priv.string.end();
            }
            string_type_const_iterator cumulative_end() const {
                return priv.string.end();
            }
            string_type_const_iterator cumulative_cend() const {
                return priv.string.end();
            }
            string_type_reverse_iterator cumulative_rbegin() {
                return priv.string.rbegin();
            }
            string_type_const_reverse_iterator cumulative_rbegin() const {
                return priv.string.rbegin();
            }
            string_type_const_reverse_iterator cumulative_crbegin() const {
                return priv.string.rbegin();
            }
            string_type_reverse_iterator cumulative_rend() {
                return priv.string.rend();
            }
            string_type_const_reverse_iterator cumulative_rend() const {
                return priv.string.rend();
            }
            string_type_const_reverse_iterator cumulative_crend() const {
                return priv.string.rend();
            }
        /*`Building Substring Iterators`*/
            string_type_iterator building_begin() {
                return priv.string.begin() + priv.buildingIndex;
            }
            string_type_const_iterator building_begin() const {
                return priv.string.begin() + priv.buildingIndex;
            }
            string_type_const_iterator building_cbegin() const {
                return priv.string.begin() + priv.buildingIndex;
            }
            string_type_iterator building_end() {
                return priv.string.end();
            }
            string_type_const_iterator building_end() const {
                return priv.string.end();
            }
            string_type_const_iterator building_cend() const {
                return priv.string.end();
            }
            string_type_reverse_iterator building_rbegin() {
                return priv.string.rbegin();
            }
            string_type_const_reverse_iterator building_rbegin() const {
                return priv.string.rbegin();
            }
            string_type_const_reverse_iterator building_crbegin() const {
                return priv.string.rbegin();
            }
            string_type_reverse_iterator building_rend() {
                return priv.string.rend() - priv.buildingIndex;
            }
            string_type_const_reverse_iterator building_rend() const {
                return priv.string.rend() - priv.buildingIndex;
            }
            string_type_const_reverse_iterator building_crend() const {
                return priv.string.rend() - priv.buildingIndex;
            }
        /*`Standard Iterators`*/
            const_iterator begin() const {
                return const_iterator(*this, (node_type*)(priv.nodes.data()));
            }
            const_iterator end() const {
                return const_iterator(*this, (node_type*)(priv.nodes.data()+priv.nodes.size()));
            }
            const_reverse_iterator rbegin() const {
                return const_reverse_iterator(*this, (node_type*)(priv.nodes.data()+priv.nodes.size()-1));
            }
            const_reverse_iterator rend() const {
                return const_reverse_iterator(*this, (node_type*)(priv.nodes.data()-1));
            }

    /*`Capacity`*/
        /*`Cumulative Capacity`*/
            bool cumulative_empty() const {
                return priv.string.empty();
            }
            size_type cumulative_size() const {
                return priv.string.size();
            }
            size_type cumulative_length() const {
                return priv.string.length();
            }
            size_type cumulative_max_size() const {
                return priv.string.max_size();
            }
            void cumulative_reserve(size_type n) {
                priv.string.reserve(n);
            }
            size_type cumulative_capacity() const {
                return priv.string.capacity();
            }
            void cumulative_shrink_to_fit() {
                priv.string.shrink_to_fit();
            }
        /*`Building Substring Capacity`*/
            bool building_empty() const {
                return priv.string.size() > priv.buildingIndex;
            }
            size_type building_size() const {
                return priv.string.size() - priv.buildingIndex;
            }
            size_type building_length() const {
                return priv.string.length() - priv.buildingIndex;
            }
            size_type building_max_size() const {
                return priv.string.max_size() - priv.buildingIndex;
            }
            void building_reserve(size_type n) {
                priv.string.reserve(priv.buildingIndex + n);
            }
            size_type building_capacity() const {
                return priv.string.capacity() - priv.buildingIndex;
            }
            void building_shrink_to_fit() {
                priv.string.shrink_to_fit();
            }
        /*`Standard Capacity`*/
            bool empty() const {
                return priv.nodes.empty();
            }
            size_type size() const {
                return priv.nodes.size();
            }
            size_type max_size() const {
                return priv.nodes.max_size();
            }
            void reserve(size_type new_cap) {
                priv.nodes.reserve(new_cap);
            } 
            size_type capacity() const {
                return priv.nodes.capacity();
            }
            void shrink_to_fit() {
                priv.string.shrink_to_fit();
                priv.nodes.shrink_to_fit();
            }

    /*`Standard Modifiers`*/
        void clear() {
            priv.string.clear();
            priv.nodes.clear();
        }
        /*`insert`*/
            template <typename ...Types>
            const_reverse_iterator insert(const_reverse_iterator pos, Types&&... args) {
                return insert(pos.base(), args...).reverse();
            }
            const_iterator insert(const_iterator pos, const char_type *s) {
                return insert(pos, s, strlen(s));
            }
            const_iterator insert(const_iterator pos, const char_type *s, size_type length) {
                return insert(pos, string_view_type(s, length));
            }
            const_iterator insert(const_iterator pos, string_view_type value) {
                size_type
                    iteratorIndex = pos - begin(),
                    insertedNodeStringIndex = pos.priv.data->index;
                priv.string.insert(insertedNodeStringIndex, value);
                while (pos != end()) {
                    pos.priv.data->index += value.size();
                    ++pos;
                }
                priv.nodes.insert(
                    priv.nodes.begin() + iteratorIndex,
                    node_type{ insertedNodeStringIndex, value.size()}
                );

                return begin() + iteratorIndex;
            }
            const_iterator insert(const_iterator pos, const string_type &value) {
                return insert(pos, string_view_type(value));
            }
            template <class InputIt>
            basic_multi_string &insert(size_type pos, InputIt first, InputIt last) {
                insert(begin() + pos, first, last);
                return *this;
            }
            const_iterator insert(const_iterator pos, const_iterator first, const_iterator last) {
                size_type
                    insertionCount = last - first,
                    iteratorIndex = pos - begin(),
                    insertedNodesStringIndex = pos.priv.data->index;
                string_view_type totalStringToInsert(first.priv.parent.priv.string.data() + first.priv.data->index, ((last-1).priv.data->index - first.priv.data->index) + (last-1).priv.data->size);
                priv.string.insert(insertedNodesStringIndex, totalStringToInsert);
                while (pos != end()) {
                    pos.priv.data->index += totalStringToInsert.size();
                    ++pos;
                }
                priv.nodes.insert(priv.nodes.begin() + iteratorIndex, insertionCount, {});
                auto nodeIterator = priv.nodes.begin() + iteratorIndex;
                for (auto it = first; it != last; ++it, ++nodeIterator) {
                    nodeIterator->index = insertedNodesStringIndex;
                    nodeIterator->size = it.priv.data->size;
                    insertedNodesStringIndex += it.priv.data->size;
                }
                return begin() + iteratorIndex;
            }

            template <class InputIt>
            requires (!std::is_same_v<InputIt, const_iterator>)
            const_iterator insert(const_iterator pos, InputIt first, InputIt last) {
                size_type iteratorIndex = pos - begin();
                using element_type = std::remove_const_t<std::remove_reference_t<decltype(*first)>>;
                constexpr bool IS_C_STR_LIKE = std::is_same_v<element_type, const char_type*> || std::is_same_v<element_type, char_type*>;
                constexpr bool IS_CONTAINER_LIKE = []() consteval {
                    if constexpr (
                        std::is_same_v<element_type, string_type> ||
                        std::is_same_v<element_type, string_view_type> ||
                        std::is_same_v<element_type, std::initializer_list<char_type>>
                    ) return true;
                    else {
                        return !IS_C_STR_LIKE && std::is_same_v<char_type,std::decay_t<decltype(std::declval<element_type>()[0])>>;
                    }
                }();
                
                static_assert(IS_CONTAINER_LIKE || IS_C_STR_LIKE);
                
                auto iteratorData = [](InputIt it){
                    if constexpr (IS_CONTAINER_LIKE && !std::is_array_v<element_type>) {
                        if constexpr (std::is_same_v<element_type, std::initializer_list<char_type>>) {
                            return it->begin();
                        } else if constexpr (std::is_same_v<InputIt, const_reverse_iterator>) {
                            return it.data();
                        } else {
                            return it->data();
                        }
                    } else if constexpr (IS_C_STR_LIKE || std::is_array_v<element_type>) {
                        return *it;
                    }
                };
                
                auto iteratorSize = [](InputIt it){
                    if constexpr (IS_CONTAINER_LIKE) {
                        if constexpr (std::is_array_v<element_type>) {
                            return sizeof(element_type);
                        } else if constexpr (std::is_same_v<InputIt, const_reverse_iterator>) {
                            return it.size();
                        } else {
                            return it->size();
                        }
                    } else if constexpr (IS_C_STR_LIKE) {
                        return strlen(*it);
                    }
                };
                constexpr bool is_random_access = std::is_same_v<InputIt, std::random_access_iterator_tag>;
                size_type insertionCount;
                if constexpr (is_random_access) {
                    insertionCount = last - first;
                } else {
                    insertionCount = 0;
                }
                size_type totalInsertionSize = 0;
                size_type newItemsIndex = pos.priv.data->index;
                auto itemPos = priv.nodes.begin() + (pos.priv.data - priv.nodes.data());
                
                for (InputIt it = first; it != last; ++it) {
                    totalInsertionSize += iteratorSize(it);
                    if constexpr (!is_random_access) {++insertionCount;}
                }
                
                priv.string.insert(newItemsIndex, totalInsertionSize, 0);
                
                while (pos != end()) {
                    pos.priv.data->index += totalInsertionSize;
                    ++pos;
                }
                
                priv.nodes.insert(itemPos, insertionCount, {});
                char_type *insertPointer = priv.string.data() + newItemsIndex;
                
                for (InputIt it = first; it != last; ++it) {
                    const char_type *src = iteratorData(it);
                    size_type n = iteratorSize(it);
                    
                    memcpy(insertPointer, src, n);
                    itemPos->index = insertPointer - priv.string.data();
                    itemPos->size = n;
                    insertPointer += n;
                    ++itemPos;
                }
                
                return begin() + iteratorIndex;
            }
            template <class Container>
            basic_multi_string &insert(size_type pos, const Container &container) {
                insert(pos, container.begin(), container.end());
                return *this;
            }
            template <class Container>
            const_iterator insert(const_iterator pos, const Container &container) {
                return insert(pos, container.begin(), container.end());
            }
            template <typename element>
            basic_multi_string &insert(size_type pos, std::initializer_list<element> container) {
                insert(pos, container.begin(), container.end());
                return *this;
            }
            template <typename element>
            const_iterator insert(const_iterator pos, std::initializer_list<element> container) {
                return insert(pos, container.begin(), container.end());
            }
        /*`erase`*/
            basic_multi_string &erase(size_type pos) {
                erase(begin() + pos);
                return *this;
            }
            template <typename ...Types>
            const_reverse_iterator erase(const_reverse_iterator pos, Types &&...args) {
                return erase(pos.base(), args...).reverse();
            }
            const_iterator erase(const_iterator pos) {
                priv.string.erase(pos.priv.data->index, pos.priv.data->size);
                size_type iteratorIndex = pos - begin();
                auto nodePositionToErase = priv.nodes.begin() + iteratorIndex;
                while (pos != end()) {
                    pos.priv.data->index -= nodePositionToErase->size;
                    ++pos;
                }
                priv.nodes.erase(nodePositionToErase);
                return begin() + iteratorIndex;
            }
            
            basic_multi_string &erase(size_type pos, size_type count) {
                erase(begin() + pos, begin() + pos + count);
                return *this;
            }
            const_reverse_iterator erase(const_reverse_iterator first, const_reverse_iterator last) {
                return erase(first.base(), last.base()).reverse();
            }
            const_iterator erase(const_iterator first, const_iterator last) {
                size_type erasureCount = (last-1).priv.data->index + (last-1).priv.data->size - first.priv.data->index;
                priv.string.erase(first.priv.data->index, erasureCount);
                size_type iteratorIndex = first - begin();
                auto nodesFirst = priv.nodes.begin() + iteratorIndex, nodesLast = nodesFirst + (last-first);
                for (auto it = nodesLast; it != priv.nodes.end(); ++it) {
                    it->index -= erasureCount;
                }
                priv.nodes.erase(nodesFirst, nodesLast);
                return begin() + iteratorIndex;
            }
        /*`push_back`*/
            void push_building() {
                priv.nodes.push_back({
                    .index = priv.buildingIndex,
                    .size = building_size()
                });
                priv.buildingIndex = priv.string.size();
            }
            void push_back(string_view_type str) {
                priv.string.append(str);
                push_building();
            }
            void push_back(const string_type &str) {
                priv.string.append(str);
                push_building();
            }
            void push_back(const string_type &str, size_type subpos, size_type sublen = string_type::npos) {
                priv.string.append(str, subpos, sublen);
                push_building();
            }
            template <size_type N>
            void push_back(const char_type (&s)[N]) {
                push_back(s, N);
            }
            void push_back(const char_type * s) {
                priv.string.append(s);
                push_building();
            }
            void push_back(const char_type *s, size_type n) {
                priv.string.append(s, n);
                push_building();
            }
            void push_back(size_type n, char_type c) {
                priv.string.append(n, c);
                push_building();
            }
            template <class _InputIterator>
            void push_back(_InputIterator first, _InputIterator last) {
                priv.string.append(first, last);
                push_building();
            }
            void push_back(std::initializer_list <char_type> il) {
                priv.string.append(il);
                push_building();
            }
        void pop_back() {
            priv.buildingIndex = priv.nodes.back().index;
            priv.string.resize(priv.buildingIndex);
            priv.nodes.pop_back();
        }
        void resize(size_type sz) {
            auto order = sz <=> priv.nodes.size();
            if (order == std::strong_ordering::equal) {
                return;
            }
            if (order == std::strong_ordering::greater) {
                priv.nodes.resize(sz, {.index = priv.buildingIndex, .size = 0});
                priv.nodes[sz].index = priv.buildingIndex;
                priv.nodes[sz].size = building_size();
                priv.buildingIndex = priv.string.size();
                return;
            }
            
            priv.buildingIndex = priv.nodes[sz].index;
            priv.nodes.resize(sz);
        }
        void resize(size_type sz, string_view_type str) {
            priv.string.clear();
            priv.string.reserve(str.size()*sz);
            priv.nodes.clear();
            priv.nodes.reserve(sz);
            for (size_type i = 0; i < sz; ++i) {
                push_back(str);
            }
        }
        void resize(size_type sz, const string_type &str) {
            priv.string.clear();
            priv.string.reserve(str.size()*sz);
            priv.nodes.clear();
            priv.nodes.reserve(sz);
            for (size_type i = 0; i < sz; ++i) {
                push_back(str);
            }
        }
        void resize(size_type sz, const char_type *s) {
            resize(sz, s, strlen(s));
        }
        template <size_type N>
        void resize(size_type sz, const char_type (&s)[N]) {
            resize(sz, s, N);
        }
        void resize(size_type sz, const char_type *s, size_type len) {
            priv.string.clear();
            priv.string.reserve(len*sz);
            priv.nodes.clear();
            priv.nodes.reserve(sz);
            for (size_type i = 0; i < sz; ++i) {
                push_back(s, len);
            }
        }
        template <class InputIt>
        void resize(size_type sz, InputIt first, InputIt last) {
            priv.string.clear();
            priv.string.reserve((std::distance(first, last))*sz);
            priv.nodes.clear();
            priv.nodes.reserve(sz);
            for (size_type i = 0; i < sz; ++i) {
                push_back(first,last);
            }
        }
        void resize(size_type sz, std::initializer_list<char_type> il) {
            priv.string.clear();
            priv.string.reserve(il.size()*sz);
            priv.nodes.clear();
            priv.nodes.reserve(sz);
            for (size_type i = 0; i < sz; ++i) {
                push_back(il);
            }
        }
        void swap(basic_multi_string &other) {
            priv.string.swap(other.priv.string);
            priv.nodes.swap(other.priv.nodes);
        }

    
    /*`String operations`*/
        /*Cumulative String operations*/
            void cumulative_clear() {
                priv.string.clear();
            }
            
        /*`Substring operations`*/
            /*`clear`*/
                void substring_clear(size_type pos) {
                    substring_clear(begin() + pos);
                }
                void substring_clear(const_reverse_iterator pos) {
                    substring_clear(pos.base());
                }
                void substring_clear(const_iterator pos) {
                    size_t reduced_size = pos.priv.data->size;
                    priv.string.erase(pos.priv.data->index, reduced_size);
                    priv::nodes_shift_subtract(pos, reduced_size);
                }
            /*`insert`*/
                template <typename ...Types>
                auto substring_insert(size_type pos, Types&&...args) -> decltype(substring_insert(begin()+pos, args...)) {
                    return substring_insert(begin()+pos, args...);
                }
                template <typename ...Types>
                auto substring_insert(const_reverse_iterator pos, Types&&...args) -> decltype(substring_insert(pos.base(), args...)) {
                    return substring_insert(pos.base(), args...);
                }
                basic_multi_string &substring_insert(const_iterator pos, size_type index, size_type count, char_type ch) {
                    priv.string.insert(pos.priv.data->index + index, count, ch);
                    priv::nodes_shift_add(pos, count);
                    return *this;
                }
                template <size_t N>
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const char_type (&s)[N]) {
                    return substring_insert(pos, index, s, N-1);
                }
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const char_type * s) {
                    return substring_insert(pos, index, s, strlen(s));
                }
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const char_type * s, size_type count) {
                    return substring_insert(pos, index, string_view_type(s, count));
                }
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const string_type &str) {
                    priv.string.insert(pos.priv.data->index + index, str);
                    priv::nodes_shift_add(pos, str.size());
                    return *this;
                }
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const string_type &str, size_type s_index, size_type count = npos) {
                    priv.string.insert(pos.priv.data->index + index, str, s_index, count);
                    size_t insertionSize = (count==npos) ? str.size()-s_index : count;
                    priv::nodes_shift_add(pos, insertionSize);
                    return *this;
                }
                string_type_iterator substring_insert(const_iterator pos, string_type_const_iterator s_pos, char_type ch) {
                    size_t iteratorIndex = (pos.priv.data->index + s_pos) - priv.string.begin();
                    priv.string.insert(pos.priv.data->index + s_pos, ch);
                    priv::nodes_shift_add(pos, 1);
                    return priv.string.begin() + iteratorIndex;
                }
                string_type_iterator substring_insert(const_iterator pos, string_type_const_iterator s_pos, size_type count, char_type ch) {
                    size_t iteratorIndex = (pos.priv.data->index + s_pos) - priv.string.begin();
                    priv.string.insert(pos.priv.data->index + s_pos, count, ch);
                    priv::nodes_shift_add(pos, count);
                    return priv.string.begin() + iteratorIndex;
                }
                template <class InputIt>
                string_type_iterator substring_insert(const_iterator pos, string_type_const_iterator s_pos, InputIt first, InputIt last) {
                    size_t iteratorIndex = (pos.priv.data->index + s_pos) - priv.string.begin();
                    size_t insertionSize = priv.string.size();
                    priv.string.insert(pos.priv.data->index + s_pos, first, last);
                    insertionSize = priv.string.size() - insertionSize;
                    priv::nodes_shift_add(pos, insertionSize);
                    return priv.string.begin() + iteratorIndex;
                }
                string_type_iterator substring_insert(const_iterator pos, string_type_const_iterator s_pos, std::initializer_list<char_type> ilist) {
                    size_t iteratorIndex = (pos.priv.data->index + s_pos) - priv.string.begin();
                    priv.string.insert(pos.priv.data->index + s_pos, ilist);
                    priv::nodes_shift_add(pos, ilist.size());
                    return priv.string.begin() + iteratorIndex;
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const StringViewLike& t) {
                    priv.string.insert(pos.priv.data->index + index, t);
                    priv::nodes_shift_add(pos, t.size());
                    return *this;
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &substring_insert(const_iterator pos, size_type index, const StringViewLike& t, size_type t_index, size_type count = npos) {
                    size_t insertionSize = (count==npos) ? t.size()-t_index : count;
                    priv.string.insert(pos.priv.data->index + index, t, t_index, npos);
                    priv::nodes_shift_add(pos, insertionSize);
                    return *this;
                }
            /*`erase`*/
                template <typename ...Types>
                auto substring_erase(size_type pos, Types&&...args) -> decltype(substring_erase(begin()+pos, args...)) {
                    return substring_erase(begin()+pos, args...);
                }
                template <typename ...Types>
                auto substring_erase(const_reverse_iterator pos, Types&&...args) -> decltype(substring_erase(pos.base(), args...)) {
                    return substring_erase(pos.base(), args...);
                }
                basic_multi_string &substring_erase(const_iterator pos, size_type index = 0, size_type count = npos) {
                    size_t erasureSize = (count == npos) ? pos.priv.data->size - index : count;
                    priv.string.erase(pos.priv.data->index + index, erasureSize);
                    priv::nodes_shift_subtract(pos, erasureSize);
                    return *this;
                }
                string_type_iterator substring_erase(const_iterator pos, string_type_const_iterator s_position) {
                    size_t
                        iteratorIndex = s_position - priv.string.begin(),
                        erasureSize = pos.priv.data->size - (iteratorIndex);
                    priv.string.erase(pos.priv.data->index, erasureSize);
                    priv::nodes_shift_subtract(pos, erasureSize);
                    return priv.string.begin() + iteratorIndex;
                }
                string_type_iterator substring_erase(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last) {
                    size_t
                        iteratorIndex = first - priv.string.begin(),
                        erasureSize = last - first;
                    priv.string.erase(first, last);
                    priv::nodes_shift_subtract(pos, erasureSize);
                    return  priv.string.begin() + iteratorIndex;
                }
            /*`push_back`*/
                void substring_push_back(size_type pos, char_type ch) {substring_push_back(begin()+pos, ch);}    
                void substring_push_back(const_reverse_iterator pos, char_type ch) {substring_push_back(pos.base(), ch);}
                void substring_push_back(const_iterator pos, char_type ch) {
                    priv.string.push_back(ch);
                    substring_insert(pos, priv.string.begin()+pos.priv.data->size, ch);
                }
            /*`append`*/
                template <typename ...Types>
                auto substring_append(size_type pos, Types&&...args) -> decltype(substring_append(begin()+pos, args...)) {
                    return substring_append(begin()+pos, args...);
                }
                template <typename ...Types>
                auto substring_append(const_reverse_iterator pos, Types&&...args) -> decltype(substring_append(pos.base(), args...)) {
                    return substring_append(pos.base(), args...);
                }
                basic_multi_string &substring_append(const_iterator pos, size_type count, char_type ch) {
                    return substring_insert(pos, pos.priv.data->size, count, ch);
                }
                basic_multi_string &substring_append(const_iterator pos, const string_type &str) {
                    return substring_insert(pos, pos.priv.data->size, str);
                }
                basic_multi_string &substring_append(const_iterator pos, const string_type &str, size_type subpos, size_type sublen = string_type::npos) {
                    return substring_insert(pos, pos.priv.data->size, str, subpos, sublen);
                }
                basic_multi_string &substring_append(const_iterator pos, const char_type *s, size_type n) {
                    return substring_insert(pos, pos.priv.data->size, s, n);
                }
                basic_multi_string &substring_append(const_iterator pos, const char_type *s) {
                    return substring_insert(pos, pos.priv.data->size, s);
                }
                template <class InputIt>
                basic_multi_string &substring_append(const_iterator pos, InputIt first, InputIt last) {
                    substring_insert(pos, priv.string.begin()+pos.priv.data->size, first, last);
                    return *this;
                }
                basic_multi_string &substring_append(const_iterator pos, std::initializer_list <char_type> il) {
                    return substring_insert(pos, pos.priv.data->size, il);
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &substring_append(const_iterator pos, const StringViewLike& t) {
                    return substring_insert(pos, pos.priv.data->size, t);
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &substring_append(const_iterator pos, const StringViewLike& t, size_type t_index, size_type count = npos) {
                    return substring_insert(pos, pos.priv.data->size, t, t_index, count);
                }
            /*`compare`*/
                int substring_compare(const_iterator pos, const string_type& str) const noexcept {
                    return priv.string.compare(pos.priv.data->index, pos.priv.data->size, str);
                }
                int substring_compare(const_iterator pos, size_type s_pos, size_type len, const string_type& str) const {
                    return priv.string.compare(pos.priv.data->index + s_pos, len, str);
                }
                int substring_compare(const_iterator pos, size_type s_pos, size_type len, const string_type& str, size_type subpos, size_type sublen = string_type::npos) const {
                    return priv.string.compare(pos.priv.data->index + s_pos, len, str, subpos, sublen);
                }
                int substring_compare(const_iterator pos, const char_type* s) const {
                    return priv.string.compare(pos.priv.data->index, pos.priv.data->size, s);
                }
                int substring_compare(const_iterator pos, size_type s_pos, size_type len, const char_type* s) const {
                    return priv.string.compare(pos.priv.data->index + s_pos, len, s);
                }
                int substring_compare(const_iterator pos, size_type s_pos, size_type len, const char_type* s, size_type n) const {
                    return priv.string.compare(pos.priv.data->index + s_pos, len, s, n);
                }
                template<class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                int substring_compare(const_iterator pos, const StringViewLike& t) const noexcept {
                    return priv.string.compare(pos.priv.data->index, pos.priv.data->size, t);
                }
                template<class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                int substring_compare(const_iterator pos, size_type s_pos1, size_type count1, const StringViewLike& t ) const {
                    return priv.string.compare(pos.priv.data->index + s_pos1, count1, t);
                }
                template<class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                int substring_compare(const_iterator pos, size_type s_pos1, size_type count1, const StringViewLike& t, size_type s_pos2, size_type count2 = npos) const {
                    return priv.string.compare(pos.priv.data->index + s_pos1, count1, t, s_pos2, count2);
                }
            /*`starts_with`*/
                bool substring_starts_with(const_iterator pos, string_view_type sv) const noexcept {
                    return std::string_view(priv.string.data() + pos.priv.data->index, pos.priv.data->size).starts_with(sv);
                }
                bool substring_starts_with(const_iterator pos, char_type ch) const noexcept {
                    return std::string_view(priv.string.data() + pos.priv.data->index, pos.priv.data->size).starts_with(ch);
                }
                bool substring_starts_with(const_iterator pos, const char_type* s) const noexcept {
                    return std::string_view(priv.string.data() + pos.priv.data->index, pos.priv.data->size).starts_with(s);
                }
            /*`ends_with`*/
                bool substring_ends_with(const_iterator pos, string_view_type sv) const noexcept {
                    return std::string_view(priv.string.data()+pos.priv.data->index, pos.priv.data->size).ends_with(sv);
                }
                bool substring_ends_with(const_iterator pos, char_type ch) const noexcept {
                    return std::string_view(priv.string.data()+pos.priv.data->index, pos.priv.data->size).ends_with(ch);
                }
                bool substring_ends_with(const_iterator pos, const char_type* s) const noexcept {
                    return std::string_view(priv.string.data()+pos.priv.data->index, pos.priv.data->size).ends_with(s);
                }
            /*`contains`*/
                bool substring_contains(const_iterator pos, string_view_type sv) const noexcept {
                    return std::string_view(priv.string.data()+pos.priv.data->index, pos.priv.data->size).find(sv) != npos;
                }
                bool substring_contains(const_iterator pos, char_type ch) const noexcept {
                    return std::string_view(priv.string.data()+pos.priv.data->index, pos.priv.data->size).find(ch) != npos;
                }
                bool substring_contains(const_iterator pos, const char_type * s) const {
                    return std::string_view(priv.string.data()+pos.priv.data->index, pos.priv.data->size).find(s) != npos;
                }
            /*`replace`*/
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, const string_type& str ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, str);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last, const string_type& str ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(first, last, str);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, const string_type& str, size_type s_pos2, size_type count2 = npos ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, str, s_pos2, count2);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, const char_type* cstr, size_type count2 ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, cstr, count2);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last, const char_type* cstr, size_type count2 ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(first, last, cstr, count2);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, const char_type *cstr ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, cstr);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last, const char_type* cstr ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(first, last, cstr);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, size_type count2, char_type ch ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, count2, ch);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                template< class InputIt >
                basic_multi_string& substring_replace(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last, InputIt first2, InputIt last2 ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(first, last, first2, last2);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                basic_multi_string& substring_replace(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last, std::initializer_list<char_type> ilist ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(first, last, ilist);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                template< class StringViewLike >
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, const StringViewLike& t ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, t);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                template< class StringViewLike >
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string& substring_replace(const_iterator pos, string_type_const_iterator first, string_type_const_iterator last, const StringViewLike& t ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(first, last, t);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
                template< class StringViewLike >
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string& substring_replace(const_iterator pos, size_type s_pos, size_type count, const StringViewLike& t, size_type s_pos2, size_type count2 = npos ) {
                    ssize_t change = priv.string.size();
                    priv.string.replace(s_pos + pos.priv.data->index, count, t, s_pos2, count2);
                    change = priv.string.size() - change;
                    priv::nodes_shift_change(pos, change);
                    return *this;
                }
            /*`substring`*/
                string_type substring(size_type pos) const {return substring(begin()+pos);}
                string_type substring(const_reverse_iterator pos) const {return substring(pos.base());}
                string_type substring(const_iterator pos) const {
                    return priv.string.substr(pos.priv.data->index, pos.priv.data->size);
                }
            /*`substring_view`*/
                string_view_type substring_view(size_type pos) const {return substring_view(begin()+pos);}
                string_view_type substring_view(const_reverse_iterator pos) const {return substring_view(pos.base());}
                string_view_type substring_view(const_iterator pos) const {
                    return string_view_type(priv.string.data() + pos.priv.data->index, pos.priv.data->size);
                }
            /*`substring_substr`*/
                string_type substring_substr(size_type pos) const {return substring_substr(begin()+pos);}
                string_type substring_substr(const_reverse_iterator pos) const {return substring_substr(pos.base());}
                string_type substring_substr(const_iterator pos, size_type s_pos = 0, size_type count = npos ) const {
                    return priv.string.substr(pos.priv.data->index + s_pos, (count == npos) ? pos.priv.data->size - s_pos : count);
                }
            /*`substring_substrview`*/
                string_view_type substring_substrview(size_type pos) const {return substring_substrview(begin()+pos);}
                string_view_type substring_substrview(const_reverse_iterator pos) const {return substring_substrview(pos.base());}
                string_view_type substring_substrview(const_iterator pos, size_type s_pos = 0, size_type count = npos) const {
                    return string_view_type(priv.string.data() + s_pos + pos.priv.data->index, (count == npos) ? pos.priv.data->size - s_pos : count);
                }
            /*`substring_copy`*/
                string_view_type substring_copy(size_type pos) const {return substring_copy(begin()+pos);}
                string_view_type substring_copy(const_reverse_iterator pos) const {return substring_copy(pos.base());}
                size_type substring_copy(const_iterator pos, char_type* dest, size_type count, size_type s_pos = 0 ) const {
                    return priv.string.copy(dest, count, s_pos + pos.priv.data->index);
                }
            /*`resize`*/
                template <typename ...Types>
                void substring_resize(size_type pos, Types&&...args) {substring_resize(begin()+pos, args...);}
                template <typename ...Types>
                void substring_resize(const_reverse_iterator pos, Types&&...args) {substring_resize(pos.base(), args...);}
                void substring_resize(const_iterator pos, size_type n) {
                    substring_resize(pos, n, '\0');
                }
                void substring_resize(const_iterator pos, size_type n, char_type ch) {
                    if (n == pos.priv.data->size)
                        return;
                    if (n < pos.priv.data->size) {
                        substring_erase(pos, pos.priv.data->size-n);
                    } else {
                        substring_insert(pos, pos.priv.data->size, n - pos.priv.data->size, ch);
                    }
                    if constexpr (sizeof(char_type)==1) {
                        memset(priv.string.data()+pos.priv.data->index, ch, pos.priv.data->size);
                    } else {
                        for (size_t i = pos.priv.data->index, i_end = pos.priv.data->index+pos.priv.data->size; i < i_end; ++i) {
                            priv.string[i] = ch;
                        }
                    }
                    pos.priv.data->size = n;
                    
                    // substring_insert(pos, pos.priv.data->size - n, n, ch);
                }
                
        /*Building Substring operations*/
            void building_clear() {
                priv.string.resize(priv.buildingIndex);
            }
            /*`insert`*/
                basic_multi_string &building_insert(size_type index, size_type count, char_type ch) {
                    priv.string.insert(priv.buildingIndex + index, count, ch);
                    return *this;
                }
                basic_multi_string &building_insert(size_type index, const char_type * s) {
                    priv.string.insert(priv.buildingIndex + index, s);
                    return *this;
                }
                basic_multi_string &building_insert(size_type index, const char_type * s, size_type count) {
                    priv.string.insert(priv.buildingIndex + index, s, count);
                    return *this;
                }
                basic_multi_string &building_insert(size_type index, const string_type &str) {
                    priv.string.insert(priv.buildingIndex + index, str);
                    return *this;
                }
                basic_multi_string &building_insert(size_type index, const string_type &str, size_type s_index, size_type count = npos) {
                    priv.string.insert(priv.buildingIndex + index, str, s_index, count);
                    return *this;
                }
                string_type_iterator building_insert(string_type_const_iterator pos, char_type ch) {
                    return priv.string.insert(pos, ch);
                }
                string_type_iterator building_insert(string_type_const_iterator pos, size_type count, char_type ch) {
                    return priv.string.insert(pos, count, ch);
                }
                template <class InputIt>
                string_type_iterator building_insert(string_type_const_iterator pos, InputIt first, InputIt last) {
                    return priv.string.insert(pos, first, last);
                }
                string_type_iterator building_insert(string_type_const_iterator pos, std::initializer_list<char_type> ilist) {
                    return priv.string.insert(pos, ilist);
                }
                
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &building_insert(size_type index, const StringViewLike& t) {
                    priv.string.insert(priv.buildingIndex + index, t);
                    return *this;
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &building_insert(size_type index, const StringViewLike& t, size_type t_index, size_type count = npos) {
                    priv.string.insert(priv.buildingIndex + index, t, t_index, npos);
                    return *this;
                }
            /*`erase`*/
                basic_multi_string &building_erase(size_type index = 0, size_type count = npos) {
                    priv.string.erase(priv.buildingIndex + index, count);
                    return *this;
                }
                string_type_iterator erase(string_type_const_iterator position) {
                    return priv.string.erase(position);
                }
                string_type_iterator erase(string_type_const_iterator first, string_type_const_iterator last) {
                    return priv.string.erase(first, last);
                }
            void building_push_back(char_type ch) {
                priv.string.push_back(ch);
            }
            /*`append`*/
                basic_multi_string &building_append(size_type count, char_type ch) {
                    priv.string.append(count, ch);
                    return *this;
                }
                basic_multi_string &building_append(const string_type &str) {
                    priv.string.append(str);
                    return *this;
                }
                basic_multi_string &building_append(const string_type &str, size_type subpos, size_type sublen = string_type::npos) {
                    priv.string.append(str, subpos, sublen);
                    return *this;
                }
                basic_multi_string &building_append(const char_type *s, size_type n) {
                    priv.string.append(s, n);
                    return *this;
                }
                basic_multi_string &building_append(const char_type * s) {
                    priv.string.append(s);
                    return *this;
                }
                template <class _InputIterator>
                basic_multi_string &building_append(_InputIterator first, _InputIterator last) {
                    priv.string.append(first, last);
                    return *this;
                }
                basic_multi_string &building_append(std::initializer_list <char_type> il) {
                    priv.string.append(il);
                    return *this;
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &building_append(const StringViewLike& t) {
                    priv.string += t;
                    return *this;
                }
                template <class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string &building_append(const StringViewLike& t, size_type t_index, size_type count = npos) {
                    priv.string += std::string_view(t.data() + t_index, count == npos ? t.size() - t_index : count);
                    return *this;
                }
            /*`compare`*/
                int building_compare(const string_type& str) const noexcept {
                return priv.string.compare(priv.buildingIndex, priv.string.size()-priv.buildingIndex, str);
                }
                int building_compare(size_type pos, size_type len, const string_type& str) const {
                    return priv.string.compare(priv.buildingIndex + pos, len, str);
                }
                int building_compare(size_type pos, size_type len, const string_type& str, size_type subpos, size_type sublen = string_type::npos) const {
                    return priv.string.compare(priv.buildingIndex + pos, len, str, subpos, sublen);
                }
                int building_compare(const char_type* s) const {
                    return priv.string.compare(priv.buildingIndex, priv.string.size()-priv.buildingIndex, s);
                }
                int building_compare(size_type pos, size_type len, const char_type* s) const {
                    return priv.string.compare(priv.buildingIndex + pos, len, s);
                }
                int building_compare(size_type pos, size_type len, const char_type* s, size_type n) const {
                    return priv.string.compare(priv.buildingIndex + pos, len, s, n);
                }
                template<class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                int building_compare( const StringViewLike& t ) const noexcept {
                    return priv.string.compare(priv.buildingIndex, priv.string.size()-priv.buildingIndex, t);
                }
                template<class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                int building_compare(size_type pos1, size_type count1, const StringViewLike& t ) const {
                    return priv.string.compare(priv.buildingIndex + pos1, count1, t);
                }
                template<class StringViewLike>
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                int building_compare(size_type pos1, size_type count1, const StringViewLike& t, size_type pos2, size_type count2 = npos) const {
                    return priv.string.compare(priv.buildingIndex + pos1, count1, t, pos2, count2);
                }
            /*`starts_with`*/
                bool building_starts_with(string_view_type sv) const noexcept {
                    return std::string_view(priv.string.data() + priv.buildingIndex, priv.string.size() -priv.buildingIndex).starts_with(sv);
                }
                bool building_starts_with(char_type ch) const noexcept {
                    return std::string_view(priv.string.data() + priv.buildingIndex, priv.string.size() -priv.buildingIndex).starts_with(ch);
                }
                bool building_starts_with(const char_type* s) const noexcept {
                    return std::string_view(priv.string.data() + priv.buildingIndex, priv.string.size() -priv.buildingIndex).starts_with(s);
                }
            /*`ends_with`*/
                bool building_ends_with(string_view_type sv) const noexcept {
                    return priv.string.ends_with(sv);
                }
                bool building_ends_with(char_type ch) const noexcept {
                    return priv.string.ends_with(ch);
                }
                bool building_ends_with(const char_type* s) const noexcept {
                    return priv.string.ends_with(s);
                }
            /*`contains`*/
                bool building_contains( string_view_type sv ) const noexcept {
                    return building_find(sv) != npos;
                }
                bool building_contains(char_type ch) const noexcept {
                    return building_find(ch) != npos;
                }
                bool building_contains(const char_type * s) const {
                    return building_find(s) != npos;
                }
            /*`replace`*/
                basic_multi_string& building_replace( size_type pos, size_type count, const string_type& str ) {
                    priv.string.replace(pos + priv.buildingIndex, count, str);
                    return *this;
                }
                basic_multi_string& building_replace( string_type_const_iterator first, string_type_const_iterator last, const string_type& str ) {
                    priv.string.replace(first, last, str);
                    return *this;
                }
                basic_multi_string& building_replace( size_type pos, size_type count, const string_type& str, size_type pos2, size_type count2 = npos ) {
                    priv.string.replace(pos + priv.buildingIndex, count, str, pos2, count2);
                    return *this;
                }
                basic_multi_string& building_replace( size_type pos, size_type count, const char_type* cstr, size_type count2 ) {
                    priv.string.replace(pos + priv.buildingIndex, count, cstr, count2);
                    return *this;
                }
                basic_multi_string& building_replace( string_type_const_iterator first, string_type_const_iterator last, const char_type* cstr, size_type count2 ) {
                    priv.string.replace(first, last, cstr, count2);
                    return *this;
                }
                basic_multi_string& building_replace( size_type pos, size_type count, const char_type *cstr ) {
                    priv.string.replace(pos + priv.buildingIndex, count, cstr);
                    return *this;
                }
                basic_multi_string& building_replace(string_type_const_iterator first, string_type_const_iterator last, const char_type* cstr ) {
                    priv.string.replace(first, last, cstr);
                    return *this;
                }
                basic_multi_string& building_replace( size_type pos, size_type count, size_type count2, char_type ch ) {
                    priv.string.replace(pos + priv.buildingIndex, count, count2, ch);
                    return *this;
                }
                template< class InputIt >
                basic_multi_string& building_replace( string_type_const_iterator first, string_type_const_iterator last, InputIt first2, InputIt last2 ) {
                    priv.string.replace(first, last, first2, last2);
                    return *this;
                }
                basic_multi_string& building_replace( string_type_const_iterator first, string_type_const_iterator last, std::initializer_list<char_type> ilist ) {
                    priv.string.replace(first, last, ilist);
                    return *this;
                }
                template< class StringViewLike >
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string& building_replace( size_type pos, size_type count, const StringViewLike& t ) {
                    priv.string.replace(pos + priv.buildingIndex, count, t);
                    return *this;
                }
                template< class StringViewLike >
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string& building_replace( string_type_const_iterator first, string_type_const_iterator last, const StringViewLike& t ) {
                    priv.string.replace(first, last, t);
                    return *this;
                }
                template< class StringViewLike >
                requires (std::is_convertible_v<StringViewLike, string_view_type> && !std::is_convertible_v<StringViewLike, const char_type*>)
                basic_multi_string& building_replace( size_type pos, size_type count, const StringViewLike& t, size_type pos2, size_type count2 = npos ) {
                    priv.string.replace(pos + priv.buildingIndex, count, t, pos2, count2);
                    return *this;
                }
            string_type building_string() const {
                return priv.string.substr(priv.buildingIndex);
            }
            string_view_type building_string_view() const {
                return building_substrview();
            }
            string_type building_substr( size_type pos = 0, size_type count = npos ) const {
                return priv.string.substr(pos + priv.buildingIndex, count);
            }
            string_view_type building_substrview(size_type pos = 0, size_type count = npos) const {
                return string_view_type(priv.string.data() + pos + priv.buildingIndex, (count == npos) ? priv.string.size() - (priv.buildingIndex + pos) : count);
            }
            size_type building_copy(char_type* dest, size_type count, size_type pos = 0 ) const {
                return priv.string.copy(dest, count, pos + priv.buildingIndex);
            }
            void building_resize(size_type n) {
                priv.string.resize(priv.buildingIndex + n);
            }
            void building_resize(size_type n, char_type ch) {
                priv.string.resize(priv.buildingIndex + n, ch);
            }
    /*`Substring Search`*/
        /*`Standard Substring Search`*/
            /*`find`*/
                size_type find(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find(str, pos);
                }
                size_type find(const char_type* s, size_type pos = 0) const {
                    return priv.string.find(s, pos);
                }
                size_type find(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find(s, pos, n);
                }
                size_type find(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find(c, pos);
                }
            /*`rfind`*/
                size_type rfind(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.rfind(str, pos);
                }
                size_type rfind(const char_type* s, size_type pos = 0) const {
                    return priv.string.rfind(s, pos);
                }
                size_type rfind(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.rfind(s, pos, n);
                }
                size_type rfind(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.rfind(c, pos);
                }
            /*`find_first_of`*/
                size_type find_first_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_first_of(str, pos);
                }
                size_type find_first_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_first_of(s, pos);
                }
                size_type find_first_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_first_of(s, pos, n);
                }
                size_type find_first_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_first_of(c, pos);
                }
            /*`find_first_not_of`*/
                size_type find_first_not_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_first_not_of(str, pos);
                }
                size_type find_first_not_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_first_not_of(s, pos);
                }
                size_type find_first_not_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_first_not_of(s, pos, n);
                }
                size_type find_first_not_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_first_not_of(c, pos);
                }
            /*`find_last_of`*/
                size_type find_last_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_last_of(str, pos);
                }
                size_type find_last_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_last_of(s, pos);
                }
                size_type find_last_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_last_of(s, pos, n);
                }
                size_type find_last_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_last_of(c, pos);
                }
            /*`find_last_not_of`*/
                size_type find_last_not_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_last_not_of(str, pos);
                }
                size_type find_last_not_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_last_not_of(s, pos);
                }
                size_type find_last_not_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_last_not_of(s, pos, n);
                }
                size_type find_last_not_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_last_not_of(c, pos);
                }
        /*`Substring Substring Search`*/
            /*`find`*/
                template <typename ...Types>
                size_type substring_find(size_type pos, Types&& ...args) {return substring_find(begin()+pos, args...);}
                template <typename ...Types>
                size_type substring_find(const_reverse_iterator pos, Types&& ...args) {return substring_find(pos.base(), args...);}
                size_type substring_find(const_iterator pos, const string_type& str, size_type s_pos = 0) const noexcept {
                    return substring(pos).find(str, s_pos);
                }
                size_type substring_find(const_iterator pos, const char_type* s, size_type s_pos = 0) const {
                    return substring(pos).find(s, s_pos);
                }
                size_type substring_find(const_iterator pos, const char_type* s, size_type s_pos, size_type n) const {
                    return substring(pos).find(s, s_pos, n);
                }
                size_type substring_find(const_iterator pos, char_type c, size_type s_pos = 0) const noexcept {
                    return substring(pos).find(c, s_pos);
                }
            /*`rfind`*/
                template <typename ...Types>
                size_type substring_rfind(size_type pos, Types&& ...args) {return substring_rfind(begin()+pos, args...);}
                template <typename ...Types>
                size_type substring_rfind(const_reverse_iterator pos, Types&& ...args) {return substring_rfind(pos.base(), args...);}
                size_type substring_rfind(const_iterator pos, const string_type& str, size_type s_pos = 0) const noexcept {
                    return substring(pos).rfind(str, s_pos);
                }
                size_type substring_rfind(const_iterator pos, const char_type* s, size_type s_pos = 0) const {
                    return substring(pos).rfind(s, s_pos);
                }
                size_type substring_rfind(const_iterator pos, const char_type* s, size_type s_pos, size_type n) const {
                    return substring(pos).rfind(s, s_pos, n);
                }
                size_type substring_rfind(const_iterator pos, char_type c, size_type s_pos = 0) const noexcept {
                    return substring(pos).rfind(c, s_pos);
                }
            /*`find_first_of`*/
                template <typename ...Types>
                size_type substring_find_first_of(size_type pos, Types&& ...args) {return substring_find_first_of(begin()+pos, args...);}
                template <typename ...Types>
                size_type substring_find_first_of(const_reverse_iterator pos, Types&& ...args) {return substring_find_first_of(pos.base(), args...);}
                size_type substring_find_first_of(const_iterator pos, const string_type& str, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_first_of(str, s_pos);
                }
                size_type substring_find_first_of(const_iterator pos, const char_type* s, size_type s_pos = 0) const {
                    return substring(pos).find_first_of(s, s_pos);
                }
                size_type substring_find_first_of(const_iterator pos, const char_type* s, size_type s_pos, size_type n) const {
                    return substring(pos).find_first_of(s, s_pos, n);
                }
                size_type substring_find_first_of(const_iterator pos, char_type c, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_first_of(c, s_pos);
                }
            /*`find_first_not_of`*/
                template <typename ...Types>
                size_type substring_find_first_not_of(size_type pos, Types&& ...args) {return substring_find_first_not_of(begin()+pos, args...);}
                template <typename ...Types>
                size_type substring_find_first_not_of(const_reverse_iterator pos, Types&& ...args) {return substring_find_first_not_of(pos.base(), args...);}
                size_type substring_find_first_not_of(const_iterator pos, const string_type& str, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_first_not_of(str, s_pos);
                }
                size_type substring_find_first_not_of(const_iterator pos, const char_type* s, size_type s_pos = 0) const {
                    return substring(pos).find_first_not_of(s, s_pos);
                }
                size_type substring_find_first_not_of(const_iterator pos, const char_type* s, size_type s_pos, size_type n) const {
                    return substring(pos).find_first_not_of(s, s_pos, n);
                }
                size_type substring_find_first_not_of(const_iterator pos, char_type c, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_first_not_of(c, s_pos);
                }
            /*`find_last_of`*/
                template <typename ...Types>
                size_type substring_find_last_of(size_type pos, Types&& ...args) {return substring_find_last_of(begin()+pos, args...);}
                template <typename ...Types>
                size_type substring_find_last_of(const_reverse_iterator pos, Types&& ...args) {return substring_find_last_of(pos.base(), args...);}
                size_type substring_find_last_of(const_iterator pos, const string_type& str, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_last_of(str, s_pos);
                }
                size_type substring_find_last_of(const_iterator pos, const char_type* s, size_type s_pos = 0) const {
                    return substring(pos).find_last_of(s, s_pos);
                }
                size_type substring_find_last_of(const_iterator pos, const char_type* s, size_type s_pos, size_type n) const {
                    return substring(pos).find_last_of(s, s_pos, n);
                }
                size_type substring_find_last_of(const_iterator pos, char_type c, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_last_of(c, s_pos);
                }
            /*`find_last_not_of`*/
                template <typename ...Types>
                size_type substring_find_last_not_of(size_type pos, Types&& ...args) {return substring_find_last_not_of(begin()+pos, args...);}
                template <typename ...Types>
                size_type substring_find_last_not_of(const_reverse_iterator pos, Types&& ...args) {return substring_find_last_not_of(pos.base(), args...);}
                size_type substring_find_last_not_of(const_iterator pos, const string_type& str, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_last_not_of(str, s_pos);
                }
                size_type substring_find_last_not_of(const_iterator pos, const char_type* s, size_type s_pos = 0) const {
                    return substring(pos).find_last_not_of(s, s_pos);
                }
                size_type substring_find_last_not_of(const_iterator pos, const char_type* s, size_type s_pos, size_type n) const {
                    return substring(pos).find_last_not_of(s, s_pos, n);
                }
                size_type substring_find_last_not_of(const_iterator pos, char_type c, size_type s_pos = 0) const noexcept {
                    return substring(pos).find_last_not_of(c, s_pos);
                }
        /*`Building Substring Search`*/
            /*`find`*/
                size_type building_find(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find(str, priv.buildingIndex + pos);
                }
                size_type building_find(const char_type* s, size_type pos = 0) const {
                    return priv.string.find(s, priv.buildingIndex + pos);
                }
                size_type building_find(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find(s, priv.buildingIndex + pos, n);
                }
                size_type building_find(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find(c, priv.buildingIndex + pos);
                }
            /*`rfind`*/
                size_type building_rfind(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.rfind(str, priv.buildingIndex + pos);
                }
                size_type building_rfind(const char_type* s, size_type pos = 0) const {
                    return priv.string.rfind(s, priv.buildingIndex + pos);
                }
                size_type building_rfind(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.rfind(s, priv.buildingIndex + pos, n);
                }
                size_type building_rfind(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.rfind(c, priv.buildingIndex + pos);
                }
            /*`find_first_of`*/
                size_type building_find_first_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_first_of(str, priv.buildingIndex + pos);
                }
                size_type building_find_first_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_first_of(s, priv.buildingIndex + pos);
                }
                size_type building_find_first_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_first_of(s, priv.buildingIndex + pos, n);
                }
                size_type building_find_first_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_first_of(c, priv.buildingIndex + pos);
                }
            /*`find_first_not_of`*/
                size_type building_find_first_not_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_first_not_of(str, priv.buildingIndex + pos);
                }
                size_type building_find_first_not_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_first_not_of(s, priv.buildingIndex + pos);
                }
                size_type building_find_first_not_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_first_not_of(s, priv.buildingIndex + pos, n);
                }
                size_type building_find_first_not_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_first_not_of(c, priv.buildingIndex + pos);
                }
            /*`find_last_of`*/
                size_type building_find_last_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_last_of(str, priv.buildingIndex + pos);
                }
                size_type building_find_last_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_last_of(s, priv.buildingIndex + pos);
                }
                size_type building_find_last_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_last_of(s, priv.buildingIndex + pos, n);
                }
                size_type building_find_last_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_last_of(c, priv.buildingIndex + pos);
                }
            /*`find_last_not_of`*/
                size_type building_find_last_not_of(const string_type& str, size_type pos = 0) const noexcept {
                    return priv.string.find_last_not_of(str, priv.buildingIndex + pos);
                }
                size_type building_find_last_not_of(const char_type* s, size_type pos = 0) const {
                    return priv.string.find_last_not_of(s, priv.buildingIndex + pos);
                }
                size_type building_find_last_not_of(const char_type* s, size_type pos, size_type n) const {
                    return priv.string.find_last_not_of(s, priv.buildingIndex + pos, n);
                }
                size_type building_find_last_not_of(char_type c, size_type pos = 0) const noexcept {
                    return priv.string.find_last_not_of(c, priv.buildingIndex + pos);
                }        

    /*`Read`*/
    bool pushf(file::info info) {
        priv.string.resize(cumulative_size()+info.size());
        info.read(building_data());
        push_building();
    }
    bool insertf(size_type pos, file::info info) {return insertf(begin()+pos, info).success;}
    file::result_type<const_reverse_iterator> insertf(const_reverse_iterator pos, file::info info) {
        auto resultType = insertf(pos.base(), info);
        return file::result_type<const_reverse_iterator>{.result=resultType.result.reverse(), .success=resultType.success};
    }
    file::result_type<const_iterator> insertf(const_iterator pos, file::info info) {
        if (!info.exists()) {
            return file::result_type<const_iterator>{.result=pos,.success=false};
        }
        
        size_t iteratorIndex = pos-begin();
        priv.nodes.insert(priv.nodes.begin()+(iteratorIndex), node_type{pos.priv.data->index, 0});
        file::result_type<const_iterator> resultValue{.result=begin()+iteratorIndex, .success=true};
        substring_resize(resultValue.result, info.size());
        info.read((void*)substring_view(resultValue.result).data());
        return resultValue;
    }
};


using multi_string = basic_multi_string<char>;
using wmulti_string = basic_multi_string<wchar_t>;
using u8multi_string = basic_multi_string<char8_t>;
using u16multi_string = basic_multi_string<char16_t>;
using u32multi_string = basic_multi_string<char32_t>;

};
#endif