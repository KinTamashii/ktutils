#pragma once
#include <cstddef>
#include <vector>

namespace ktu {

    template <typename T, std::unsigned_integral U = size_t>
    class list;

    template <typename T>
    using list8 = list<T, uint8_t>;

    template <typename T>
    using list16 = list<T, uint16_t>;

    template <typename T>
    using list32 = list<T, uint32_t>;

    template <typename T>
    using list64 = list<T, uint64_t>;

    template <typename T>
    using listsz = list<T, size_t>;

    namespace impl {
        template <typename T, std::unsigned_integral U = size_t>
        class list_base {
            
            public:
                using value_type = T;
                using index_type = U;
                using size_type = size_t;
                using difference_type = std::ptrdiff_t;

                static constexpr index_type terminator = static_cast<index_type>(-1);
                static constexpr size_type max_size() {return std::numeric_limits<index_type>::max();}
            protected:
                struct node {
                    index_type prev, next;
                };
                std::vector<value_type> values;
                std::vector<node> nodes;
                //node_allocator nodes;

                
                index_type head = terminator, tail = terminator;

                static constexpr void check_bounds(size_t size) {
                    if (size > max_size())
                        throw std::out_of_range("Size exceeds maximum bounds of list.");
                    }

                template <bool is_const>
                class iterator_base {
                    public:
                        using iterator_category = std::bidirectional_iterator_tag;
                        using reference = typename std::conditional<is_const, const T&, T&>::type;
                        using pointer   = typename std::conditional<is_const, const T*, T*>::type;
                        using this_type = iterator_base<is_const>;
                        using index_type = typename list_base<T,U>::index_type;
                        using list_type = typename std::conditional<is_const, const list<T,U>, list<T,U>>::type;
                        
                        constexpr iterator_base() = default;
                        template <bool _is_const>
                        constexpr iterator_base(const iterator_base<_is_const> &other) : list(other.list), index(other.index) {}
                        constexpr iterator_base(list_type *list, index_type index) : list(list), index(index) {}

                        constexpr reference operator*() const {return list->values[index];}
                        constexpr pointer operator->()  const {return &(list->values[index]);}

                        constexpr this_type &operator++() {
                            index = (index == list->terminator) ?
                                list->head : list->nodes[index].next;
                            return *this;
                        }
                        constexpr this_type &operator--() {
                            index = (index == list->terminator) ?
                                list->tail : list->nodes[index].prev;
                            return *this;
                        }

                        constexpr this_type operator++(int) {
                            this_type tmp = *this;
                            ++(*this);
                            return tmp;
                        }
                        constexpr this_type operator--(int) {
                            this_type tmp = *this;
                            --(*this);
                            return tmp;
                        }

                        constexpr bool operator==(const this_type &rhs) const {return (list == (rhs.list)) && index == rhs.index;}
                        constexpr bool operator!=(const this_type &rhs) const {return !(*this == rhs);}

                        friend list_base<value_type,index_type>;
                        friend list<value_type,index_type>;
                        friend iterator_base<!is_const>;
                    private:
                        
                        list_type *list;
                        index_type index;

                };
            public:
                
                friend iterator_base<false>;
                friend iterator_base<true>;
                
        };
    };


    template <typename T, std::unsigned_integral U>
    class list : impl::list_base<T,U> {
            using base_type = typename impl::list_base<T,U>;
            using this_type = list<T, U>;
            
        public:
            using iterator                  = typename base_type::template iterator_base<false>;
            using const_iterator            = typename base_type::template iterator_base<true>;
            using reverse_iterator          = std::reverse_iterator<iterator>;
            using const_reverse_iterator    = std::reverse_iterator<const_iterator>;
            using typename base_type::value_type;
            using typename base_type::size_type;
            using typename base_type::index_type;
            using typename base_type::difference_type;
            using typename base_type::node;

            friend iterator;
            friend const_iterator;
            friend reverse_iterator;
            friend const_reverse_iterator;
            

            constexpr list() = default;

            constexpr list(const std::initializer_list<value_type> &rhs)  {assign(rhs);}
            
            constexpr explicit list(const std::vector<value_type> &rhs)   {assign(rhs);}
            constexpr explicit list(std::vector<value_type> &&rhs)        {assign(std::move(rhs));}
            constexpr explicit list(size_type count)                      {assign(count);}


            constexpr this_type &operator=(const std::initializer_list<value_type> &rhs) {
                size_type count = rhs.size();
                this->check_bounds(count);
                this->values = rhs;
                this->set_default_nodes(count);
                return *this;
            }
            constexpr this_type &operator=(const std::vector<value_type> &rhs) {
                size_type count = rhs.size();
                this->check_bounds(count);
                this->values = rhs;
                this->set_default_nodes(count);
                return *this;
            }
            constexpr this_type &operator=(std::vector<value_type> &&rhs) {
                size_type count = rhs.size();
                this->check_bounds(count);
                this->values = std::move(rhs);
                this->set_default_nodes(count);
                return *this;
            }

            constexpr void assign(size_type count, const value_type &value) {
                this->values.assign(count, value);
                this->set_default_nodes(count);
            }
            template <class InputIt>
            constexpr void assign(InputIt first, InputIt last) {
                this->values.assign(first, last);
                this->set_default_nodes(std::distance(first,last));
            }
            constexpr void assign(const std::initializer_list<value_type> &rhs) {
                this->values.assign(rhs);
                this->set_default_nodes(rhs.size());
            }
        

            // Element Access
                constexpr value_type &front()                     {return this->values[this->head];}
                constexpr value_type &back()                      {return this->values[this->tail];}
                constexpr value_type *data()                      {return this->values.data();}
                constexpr const value_type *data() const noexcept {return this->values.data();}

            // Capacity
                constexpr bool empty() const noexcept {return !size();}

                constexpr size_type size() const noexcept {return this->nodes.size();}
                constexpr size_type memsize() const noexcept {return this->values.size()*(sizeof(value_type)+sizeof(node));}

                constexpr void reserve(size_type count) {
                    this->values.reserve(count);
                    this->nodes.reserve(count);
                }

                constexpr size_type capacity() const noexcept {return this->values.capacity();}

                constexpr void shrink_to_fit() {
                    this->values.shrink_to_fit();
                    this->nodes.shrink_to_fit();
                }


            // Modifiers
                constexpr void clear() noexcept {
                    this->values.clear();
                    this->nodes.clear();
                    this->head = this->tail = this->terminator;
                }

                constexpr iterator insert(const_iterator pos, const value_type &value) {
                    this->values.push_back(value);
                    return insert_index_node(pos.index);
                }
                constexpr iterator insert(const_iterator pos, value_type &&value) {
                    this->values.push_back(std::move(value));
                    return insert_index_node(pos.index);
                }

                template <typename ...Types>
                constexpr iterator emplace(const_iterator pos, Types&&... args) {
                    this->values.emplace_back(std::forward<Types>(args)...);
                    return insert_index_node(pos.index);
                }

                constexpr iterator erase(const_iterator pos) {
                    return erase_index(pos.index);
                }
                constexpr iterator erase(const_iterator first, const_iterator last) {
                    while (first != last)
                        first = erase_index(first);
                    return iterator(first);
                }

                constexpr void push_front(const value_type &value) {
                    this->values.push_back(value);
                    push_front_node();
                }
                constexpr void push_front(value_type &&value) {
                    this->values.push_back(std::move(value));
                    push_front_node();
                }

                constexpr void push_back(const value_type &value) {
                    this->values.push_back(value);
                    push_back_node();
                }
                constexpr void push_back(value_type &&value) {
                    this->values.push_back(std::move(value));
                    push_back_node();
                }

            template <typename ...Types>
            constexpr void emplace_front(Types&&... args)  {
                this->values.emplace_back(std::forward<Types>(args)...);
                size_type
                    value_count = this->values.size(),
                    node_count  = this->nodes.size();
                while (node_count++ < value_count)
                    push_front_node();
            }

            template <typename ...Types>
            constexpr void emplace_back(Types&&... args)  {
                this->values.emplace_back(std::forward<Types>(args)...);
                size_type
                    value_count = this->values.size(),
                    node_count  = this->nodes.size();
                while (node_count++ < value_count)
                    push_back_node();
            }

            constexpr void pop_front() {erase_index(this->head);}
            constexpr void pop_back()  {erase_index(this->tail);}

            constexpr void resize(size_type count) {
                check_bounds(count);
                size_type current_size = size();
                if (count > current_size) {
                    this->values.resize(count);
                    this->nodes.resize(count);
                    return;
                }
                while (current_size > count) {
                    pop_back();
                    --current_size;
                }
            }
            constexpr void resize(size_type count, const value_type &value) {
                check_bounds(count);
                size_type current_size = size();
                if (count > current_size) {
                    this->values.resize(count, value);
                    this->nodes.resize(count);
                    return;
                }
                while (current_size > count) {
                    pop_back();
                    --current_size;
                }
            }

            constexpr void swap(list &rhs) {
                this->values.swap(rhs);
                this->nodes.swap(rhs);
                std::swap(this->head, rhs.head);
                std::swap(this->tail, rhs.tail);
            }
        
        // Iterator
            constexpr iterator begin()              noexcept {return iterator(this, this->head);}
            constexpr const_iterator begin()  const noexcept {return const_iterator(this, this->head);}
            constexpr iterator end()                noexcept {return iterator(this, this->terminator);}
            constexpr const_iterator end()    const noexcept {return const_iterator(this, this->terminator);}

            constexpr iterator rbegin()             noexcept {return reverse_iterator(this, this->head);}
            constexpr const_iterator rbegin() const noexcept {return const_reverse_iterator(this, this->head);}
            constexpr iterator rend()               noexcept {return reverse_iterator(this, this->terminator);}
            constexpr const_iterator rend()   const noexcept {return const_reverse_iterator(this, this->terminator);}

        // Operations
            template <typename Comp>
            constexpr void merge(this_type &rhs, Comp comp) {
                merge(std::move(rhs), comp);
                rhs.clear();
            }
            template <typename Comp>
            constexpr void merge(this_type &&rhs, Comp comp) {
                size_type left_size = size();
                size_type right_size = rhs.size();
                size_type sum_size = left_size + right_size;
                this->check_bounds(sum_size);

                this->values.reserve(sum_size);
                std::move( std::begin(rhs.values), std::end(rhs.values), std::back_inserter(this->values) );

                this->nodes.reserve(sum_size);
                std::move( std::begin(rhs.nodes), std::end(rhs.nodes), std::back_inserter(this->nodes) );

                // offset the new indexes
                for(size_type i=left_size;i<sum_size;++i) {
                    this->nodes[ i ].prev += index_type(left_size);
                    this->nodes[ i ].next += index_type(left_size);
                }

                // connect the head
                index_type right_head = rhs.head + index_type(left_size);
                this->nodes[this->tail].next = right_head;
                this->nodes[right_head].prev = this->tail;

                // connect the tail
                index_type right_tail = rhs.tail + index_type(left_size);
                this->tail = right_tail;
                this->nodes[ right_tail ].next = this->terminator;

                // merge the two parts
                merge_index(this->head, right_head, this->tail, comp);
            }

            constexpr void merge( this_type& rhs ) {
                merge( rhs, std::less<>() );
            }

            constexpr void merge( this_type&& rhs ) {
                merge( std::move(rhs), std::less<>() );
            }

            constexpr void remove( const T& value ) {
                index_type count = index_type(this->values.size());
                for(index_type i=0;i<count;++i) {
                    if( this->values[i] == value ) {
                        erase_index(i);
                        --i; --count;
                    }
                }
            }

            template<typename Pred>
            constexpr void remove_if( Pred pred ) {
                index_type count = index_type(this->values.size());
                for(index_type i=0;i<count;++i) {
                    if( pred(this->values[i]) ) {
                        erase_index(i);
                        --i; --count;
                    }
                }
            }

            constexpr void reverse() noexcept {
                for( auto&& node : this->nodes ) {
                    std::swap( node.prev, node.next );
                }
                std::swap( this->head, this->tail );
            }

            constexpr void splice( const_iterator pos, this_type& rhs ) {
                splice( pos, std::move(rhs) );
                rhs.clear();
            }

            constexpr void splice( const_iterator pos, this_type&& rhs ) {
                size_type left_size = size();
                size_type right_size = rhs.size();
                size_type sum_size = left_size + right_size;
                this->check_bounds(sum_size);

                this->values.reserve(sum_size);
                std::move(std::begin(rhs.values), std::end(rhs.values), std::back_inserter(this->values));

                this->nodes.reserve(sum_size);
                std::move( std::begin(rhs.nodes), std::end(rhs.nodes), std::back_inserter(this->nodes));

                index_type right_head = rhs.head + index_type(left_size);
                index_type right_tail = rhs.tail + index_type(left_size);

                splice_index( pos.index, index_type(left_size), index_type(sum_size), right_head, right_tail );
            }

        constexpr void splice( const_iterator pos, this_type& rhs, const_iterator it ) {
            splice( pos, std::move(rhs), it );
            rhs.erase( it );
        }

        constexpr void splice( const_iterator pos, this_type&& rhs, const_iterator it ) {
            size_type left_size = size();
            this->check_bounds(left_size+1);

            this->values.emplace_back( std::move(*it) );
            
            index_type prev_index = prev_index(pos.index);
            this->nodes.push_back( { prev_index, pos.index } );

            if( prev_index == this->terminator ) {
                this->head = left_size;
            } else {
                this->nodes[prev_index].next = left_size;
            }

            if( pos.index == this->terminator ) {
                this->tail = left_size;
            } else {
                this->nodes[pos.index].prev = this->left_size;
            }
        }

        constexpr void splice( const_iterator pos, this_type& rhs, const_iterator first, const_iterator last) {
            splice( pos, std::move(rhs), first, last );
            rhs.erase( first, last );
        }

        constexpr void splice( const_iterator pos, this_type&&, const_iterator first, const_iterator last ) {
            size_type left_size = size();
            size_type right_size = std::distance( first, last );
            size_type sum_size = left_size + right_size;
            this->check_bounds(sum_size);

            reserve( sum_size );
            std::move( first, last, std::back_inserter(*this) );
        }

        // Delete repeated values
        template<typename Comp>
        constexpr void unique( Comp comp ) {
            index_type count = index_type(this->values.size());
            for(index_type i=0; i<count ;++i) {
                for(index_type j=i+1; j<count ;++j) {
                    if( comp(this->values[i], this->values[j] ) ) {
                        erase_index(j);
                        --j; --count;
                    }
                }
            }
        }

        constexpr void unique() {
            unique( std::equal_to<>() );
        }

        template<typename Comp>
        constexpr void sort( Comp comp ) {
            if(this->nodes.size() < 2) return;
            //merge_sort(head,tail,N,comp);
            insertion_sort(this->head, this->tail, comp);
            //selection_sort(head,tail,comp);
        }

        constexpr void sort() {
            sort( std::less<>() );
        }

    protected:

        // Assignment

        constexpr void set_default_nodes(size_type count) {
            this->nodes.resize( count );
            if(count == 0) return;
            this->nodes[0].prev = this->terminator;
            this->head = 0;
            this->tail = index_type(count-1);
            if(count == 1) {
                this->nodes[0].next = this->terminator;
                return;
            }
            this->nodes[0].next = 1;
            for(size_type i=1; i<count; ++i) {
                this->nodes[i].prev = index_type(i-1);
                this->nodes[i].next = index_type(i+1);
            }
            this->nodes[count-1].next = this->terminator;
        }

        // Iteration

        constexpr index_type prev_index( index_type i ) const {
            if (i == this->terminator) return this->tail;
            return this->nodes[i].prev;
        }

        constexpr index_type next_index( index_type i ) const {
            if( i == this->terminator ) return this->head;
            return this->nodes[i].next;
        }

        constexpr index_type prev_index( index_type index, index_type n ) const {
            for(index_type i=0;i<n;++i)
                index = prev_index(index);
            return index;
        }

        constexpr index_type next_index( index_type index, index_type n ) const {
            for(index_type i=0;i<n;++i)
                index = next_index(index);
            return index;
        }

        constexpr index_type advance_index( index_type index, difference_type n ) const {
            if( n > 0 )
                next_index( index, index_type(n) );
            if( n < 0 )
                prev_index( index, index_type(-n) );
            return index;
        }

        // Element Access

        constexpr index_type get_value_index( const T& val ) const {
            return (&val - &(this->values[0])) / sizeof(value_type);
        }

        constexpr node& get_value_node( const T& val ) {
            return this->nodes[get_value_index(val)];
        }

        constexpr index_type get_pos_index( index_type n ) const {
            index_type half = index_type(this->nodes.size() / 2);
            if( n < half ) {
                return next_index( this->head, n );
            } else {
                return prev_index( this->tail, index_type(this->nodes.size()-1) - n );
            }
        }

        // Modifiers

        constexpr iterator insert_index_node( index_type index ) {
            
            index_type N = index_type(this->nodes.size());
            if( index == this->terminator ) {
                this->nodes.push_back( {this->tail, this->terminator} );
                if( this->tail == this->terminator ) {
                    this->head = N;
                } else {
                    this->nodes[this->tail].next = N;
                }
                this->tail = N;
            } else {
                index_type prev_index = this->nodes[index].prev;
                this->nodes.push_back( { prev_index, index } );
                this->nodes[ index ].prev = N;
                if( prev_index == this->terminator ) {
                    this->head = N;
                } else {
                    this->nodes[prev_index].next = N;
                }
            }
            return iterator(this, N );
        }

        constexpr iterator erase_index( index_type index ) {

            index_type prev_index = this->nodes[index].prev;
            index_type next_index = this->nodes[index].next;

            if(prev_index == this->terminator) {
                this->head = next_index;
            } else {
                this->nodes[prev_index].next = next_index;
            }
            
            if(next_index == this->terminator) {
                this->tail = prev_index;
            } else {
                this->nodes[ next_index ].prev = prev_index;
            }

            index_type last_index = index_type(this->values.size() - 1);

            // move the last element to the erased index
            if( index < last_index ) {
                index_type last_prev = this->nodes[last_index].prev;
                index_type last_next = this->nodes[last_index].next;

                this->values[index] = std::move( this->values.back() );
                this->nodes[index] = std::move( this->nodes.back() );
            
                if( last_prev == this->terminator ) {
                    this->head = index;
                } else {
                    this->nodes[last_prev].next = index;
                }

                if( last_next == this->terminator ) {
                    this->tail = index;
                } else {
                    this->nodes[last_next].prev = index;
                }
            }
            this->values.pop_back();
            this->nodes.pop_back();
            return iterator( this, next_index );
        }

        constexpr void push_front_node() {
            index_type N = index_type(this->nodes.size());
            this->nodes.push_back( { this->terminator, this->head } );
            if (this->head == this->terminator ) {
                this->tail = N;
            } else {
                this->nodes[this->head].prev = N;
            }
            this->head = N;
        }

        constexpr void push_back_node() {
            index_type N = index_type(this->nodes.size());
            this->nodes.push_back( { this->tail, this->terminator } );
            if (this->tail == this->terminator) {
                this->head = N;
            } else {
                this->nodes[this->tail].next = N;
            }
            this->tail = N;
        }

        constexpr void resize_nodes( size_type N ) {
            
            size_type current_size = size();
            if( current_size <= N ) return;
            
            // modify previous tail and add first new node
            if (this->tail == this->terminator) {
                this->head = 0;
                this->nodes[0].prev = this->terminator;
                this->nodes[0].next = 1;
            } else {
                this->nodes[this->tail].next = index_type(current_size);
                this->nodes[current_size].prev = this->tail;
                if( N > current_size + 1 ) {
                    this->nodes[current_size].next = index_type(current_size + 1);
                } else {
                    this->nodes[current_size].next = this->terminator;
                }
            }

            // add more new nodes
            for( size_type i = current_size + 1; i < N-1; ++i ) {
                this->nodes[i].prev = index_type(i - 1);
                this->nodes[i].next = index_type(i + 1);
            }

            // add the last node and update the tail
            if( N > current_size + 1 ) {
                this->tail = index_type(N - 1);
                this->nodes[this->tail].prev = this->tail - 1;
                this->nodes[this->tail].next = this->terminator;
            }
        }

        constexpr void swap_nodes( index_type left, index_type right ) {

            // nothing to do
            if( left == right ) return;

            // can't swap with the terminator
            if( left == this->terminator || right == this->terminator ) return;

            index_type left_prev = this->nodes[left].prev;
            index_type left_next = this->nodes[left].next;
            index_type right_prev = this->nodes[right].prev;
            index_type right_next = this->nodes[right].next;

            // check for adjacency left -> right
            if( right_prev == left ) {
                this->nodes[left].prev = left_next;
                this->nodes[left].next = right_next;
                this->nodes[right].prev = left_prev;
                this->nodes[right].next = right_prev;

                if( left_prev == this->terminator ) {
                    this->head = right;
                } else {
                    this->nodes[left_prev].next = right;
                }

                if( right_next == this->terminator ) {
                    this->tail = left;
                } else {
                    this->nodes[right_next].prev = left;
                }
                return;
            }

            // check for adjacency right -> left
            if( right_next == left ) {
                this->nodes[left].prev = right_prev;
                this->nodes[left].next = left_prev;
                this->nodes[right].prev = right_next;
                this->nodes[right].next = left_next;

                if( left_next == this->terminator ) {
                    this->tail = right;
                } else {
                    this->nodes[left_next].prev = right;
                }

                if( right_prev == this->terminator ) {
                    this->head = left;
                } else {
                    this->nodes[right_prev].next = left;
                }
                return;
            }

            // non-adjacent

            this->nodes[left].prev = right_prev;
            this->nodes[left].next = right_next;
            this->nodes[right].prev = left_prev;
            this->nodes[right].next = left_next;

            if( left_prev == this->terminator ) {
                this->head = right;
            } else {
                this->nodes[left_prev].next = right;
            }
            
            if( left_next == this->terminator ) {
                this->tail = right;
            } else {
                this->nodes[left_next].prev = right;
            }

            if( right_prev == this->terminator ) {
                this->head = left;
            } else {
                this->nodes[right_prev].next = left;
            }

            if( right_next == this->terminator ) {
                this->tail = left;
            } else {
                this->nodes[right_next].prev = left;
            }
        }

        // Operations

        constexpr index_type count( index_type first, index_type last ) {
            if( first == this->head && last == this->tail )
                return index_type(this->nodes.size());
            if( first == this->terminator || last == this->terminator )
                return 0;
            index_type N = 1;
            index_type index = first;
            while( index != last ) {
                if( index == this->terminator )
                    return 0;
                index = next_index(index);
                ++N;
            }
            return N;
        }

        // produces sorted [first,last] from sorted [first,mid) and sorted [mid,last]
        template<typename Comp>
        constexpr void merge_index( index_type first, index_type mid, index_type last, Comp comp ) {
            index_type last_next = next_index(last);
            for( index_type i = first; i != mid; i = next_index(i) ) {
                for( index_type j = mid; j != last_next; j = next_index(j) ) {
                    if(comp(this->values[j], this->values[i])) {
                        swap_nodes( i, j );
                        if( mid == j )
                            mid = i;
                        std::swap( i, j );
                    } else {
                        break;
                    }
                }
            }
            for( index_type i = mid; i != last_next; i = next_index(i) ) {
                for( index_type j = next_index(i); j != last_next; j = next_index(j) ) {
                    if(comp(this->values[j], this->values[i])) {
                        swap_nodes( i, j );
                        std::swap( i, j );
                    } else {
                        break;
                    }
                }
            }
        }

        // splice [right_head,right_tail] into [head,right_head) at index
        constexpr void splice_index( index_type index, index_type left_size, index_type sum_size, index_type right_head, index_type right_tail ) {
            index_type prev_pos = prev_index(index);
            
            // offset the new indexes
            for(index_type i=left_size;i<sum_size;++i) {
                this->nodes[i].prev += left_size;
                this->nodes[i].next += left_size;
            }
            
            // connect the head
            if( prev_pos == this->terminator ) {
                this->head = right_head;
            } else {
                this->nodes[prev_pos].next = right_head;
            }
            this->nodes[ right_head ].prev = prev_pos;

            // connect the tail
            if( index == this->terminator ) {
                this->tail = right_tail;
            } else {
                this->nodes[index].prev = right_tail;
            }
            this->nodes[right_tail].next = index;
        }

        // insertion sort -- O(N^2) compares/swaps, adaptive, [first,last]
        template<typename Comp>
        constexpr void insertion_sort( index_type first, index_type last, Comp comp ) {
            index_type first_prev = prev_index(first);
            for( index_type i = first; i != last; i = next_index(i) ) {
                for( index_type j = i; j != first_prev; j = prev_index(j) ) {
                    index_type j_next = next_index(j);
                    if(!comp(this->values[j_next], this->values[j])) break;
                    swap_nodes( j, j_next );
                    if( i == j ) i = j_next;
                    else if( i == j_next ) i = j;
                    if( last == j_next ) last = j;
                    j = j_next;
                }
            }
        }

        // selection sort -- O(N^2) compares, O(N) swaps, non-adaptive, [first,last]
        template<typename Comp>
        constexpr void selection_sort( index_type first, index_type last, Comp comp ) {
            index_type last_next = next_index(last);
            for( index_type i = first; i != last_next; i = next_index(i) ) {
                index_type min_index = i;
                for( index_type j = next_index(i); j != last_next; j = next_index(j) ) {
                    if(comp(this->values[j], this->values[min_index])) {
                        min_index = j;
                    }
                }
                if( min_index != i ) {
                    swap_nodes( i, min_index );
                    i = min_index;
                }
            }
        }

        // merge sort, [first,last] -- fix me
        template<typename Comp>
        constexpr void merge_sort( index_type first, index_type last, size_type N, Comp comp ) {
            if( N < 2 ) return;

            if( N * sizeof(node) <= 64 ) {
                insertion_sort(first,last,comp);
                return;
            }

            index_type first_pos = index_type(count(this->head, this->first) - 1);
            index_type half_size = index_type((N-1) / 2);
            index_type mid = next_index( first, half_size );
            index_type mid_next = next_index( mid );

            merge_sort( first, mid, half_size+1, comp );
            merge_sort( mid_next, last, N-(half_size+1), comp );

            // sorting invalidates indices. recompute them.
            first = get_pos_index( first_pos );
            mid_next = next_index( first, half_size + 1 );
            last = next_index( mid_next, index_type(N - 2) - half_size );
            
            merge_index( first, mid_next, last, comp );
        }

        private:
    };

};