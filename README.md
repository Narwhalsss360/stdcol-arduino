# stdcol
stdcol is cross-platfrom library for storing data in data structures.
stdcol provides the interfaces, and implemented classes.

When you see `(const)` anywhere, that means that the declaration was declared as both const and non-const

# Helper Types
* `struct index`
    * Type Declarations
        * `using int_type`: The underlying integer type. use for `constexpr`s
    * Members
        * `int_type value`: The underlying value of the index
    * Constructors
        * `index()`
        * `index(const int_type& value)`
    * Methods
        * `int_type& operator=(const int_type& value)`: Pass through to the underlying value
        * `operator (const) int_type&() (const)`: Pass through to the underlying value
* `struct findex : index`
    * Constant Expressions
        * `static constexpr const int_type not_found`: The underlying int_type value that corresponds to a function returning the index of anything that does not exist.
    * Constructors
        * `findex()`
        * `findex(const int_type& value)`
    * Methods
        * `bool found() const`: Equivalent to value != not_found
* `template <typename key_t, typename value_t> struct key_value_pair`
    * Members
        * `key_t key`
        * `value_t value`
* `template <typename value_t> struct enumeration`
    * Members
        * `findex index`
        * `value_t value`
    * Constructors
        * `enumeration(stdcol::index index, value_t value)`
    * Methods
        * `operator value_t() (const)`: Pass through to the `value`
        * `void operator=(const value_t& value)`: Pass through to the `value`
* `template <typename collectable_t> struct prioritized_element`
    * Members
        * `int priority`:
        * `collectable_t item`
    * Constructors
        * `prioritized_element()`
        * `prioritized_element(int priority, collectable_t item)`
* `template <typename T> std::initializer_list` **Only defined on a platform with no STL (determined by stdcol_nostl macro)**, This is a general copy of STL's version

# Interfaces
stdcol is built around it's interfaces.
* `template <typename collectable_t> class collection`
    * Description: This abstract type is the underlying type for all of stdcol, it is a wrapper around 1-dimensional, 0-indexed collections.
    * Type Declarations
        * `using iterator_dereference_t`: What `operator*()` returns from the iterator used by a `collection`
        * `using iterator`: The type `begin()` and `end()` returns
    * Abstract Methods
        * `(const) collectable_t * const at(index index) (const)`: Returns a pointer to the element at the specified index
        * `index size() const`: returns how many elements exist in the collection
    * Methods
        * `virtual (const) collectable_t& operator[](index index) (const)`: equivalent to *at(index), *does not perform nullptr check*
        * `iterator begin()`: Returns an iterator pointing to the first element
        * `iterator end()`: Returns an iterator pointing 1 past the last element
* `template <typename collectable_t> class dynamic_collection : collection<collectable_t>`
    * Description: This abstract type derives from collection, but also wraps around collections that are of dynamic size, and capacity.
    * Abstract Methods:
        * `index capacity() const`: How many elements could fit within the allocated block
        * `bool reserve(index new_capacity)`: Changes the size of the allocated block to fit at least `new_capacity` amount of elements. Returns true if reservation is successful
        * `bool resize(index new_size)`: Changes how many items are in the collection, not the same as `capacity()`. More specifically, this should change what `size()` returns. Returns `true` is resize is successful
        * `bool insert(index index, const collectable_t& item)`: Inserts the `item` at the specified index, pushes the item currently at the index in front of `item`. Returns `true` if insertion is successful
        * `bool remove(index index)`: Removes the item at the specified index. Return `true` if removal was successful
* `template <typename hashable_t, typename collectable_t> class dictionary`
    * Description: This abstract type is the wrapper for all containers that use a `key_value_pair`
    * Type Declarations
        * `using kvp_t`: The key-value pair type for the class and its implementors
        * `using bucket_t`: The type that is a collection of key-value pairs
        * `using buckets_t`: The type that is a collection of bucket_t
    * Abstract Methods
        * `bool contains(const hashable_t& key) const`: Returns `true` if any key in any bucket exists
        * `bool add(const hashable_t& key, const collectable_t& value)`: Adds the key-value pair to a bucket, returns `true` if the key-value pair was added successfully
        * `kvp_t* get(const hashable_t& key)`: Returns a pointer to the key-value pair with the specified `key`, return nullptr if key does not exist
        * `collectable_t& get_add(const hashable_t& key)`: Returns the value of with the specified `key`, adds if does not exist
        * `bool remove(const hashable_t& key)`: Removes the specified `key`, returns `true` if the item was successfully removed
        * `bool resize(index bucket_count)`: Resizes the amount of buckets, return `true` if bucket resize was successful
        * `buckets_t& buckets()`: Returns the buckets
        * `bucket_t& bucket(const hashable_t& key)`: Returns the bucket the `key` would be in according to `hash()`
        * `index hash(const hashable_t& key)`: Returns the hash value of the `key`
    * Methods
        * `virtual collectable_t& operator[](const hashable_t& key)`: Equivalent to `*get_add(key)`
        * `virtual index size()`: Returns the number of key-value pairs
        * `virtual dynamic_array<hashable_t> keys()`: Returns a collection of all the keys
        * `virtual dynamic_array<hashable_t> values()`: Returns a collection of all the values
        * `virtual dynamic_array<hashable_t> key_values()`: Returns a collection of all the key-value pairs
* `template <typename T> class abstract_tree_node`
    * Description: This abstract type is a wrapper around a tree node of any tree data structure
    * Type Declarations
        * `using tree_link`: The type of a node pointer
    * Abstract Methods
        * `(const) T& get_value() (const)`: Returns the value
        * `collection<tree_link>& children()`: Returns a collection of children nodes
        * `tree_link parent()`: Returns the parent node, return `nullptr` if no parent exists
    * Methods
        * `(const) T* operator&() (const)`: Equivalent to `&get_value()`
        * `(const) T& operator*() (const)`: Equivalent to `get_value()`
* `template <typename T> class abstract_tree`
    * Description: This abstract type is a wrapper around all tree data structures
    * Type Declarations
        * `using link`: The type of a node pointer
        * `using const_link`: The type of a const node pointer
    * Abstract Methods
        * `(const_link) link root() (const)`: Returns the root node of the tree, `nullptr` if no root nodes exist

# Incomplete Types
* `template <typename hashable_t> struct stdcol_hasher`
    * This incomplete type should implement `index operator()(const hashable_t&) const`, and should be a **template specialization**

# Using-Declarations
* `template <typename hashable_t> using hasher`: The hasher type that data stdcol api uses to automatically search template specializations for
* `template <typename collectable_t> using initializer_list`: The brace-enclosed init-list type

# Collection Types
* `template <typename collectable_t, index::int_type static_size> array : public collection<collectable_t>`
    * Description: 1-dimensional, static size, homogoneous collection
    * Type Declarations
        * `using c_array_t`: The c-array type
    * Constructors
        * `array()`
        * `array(const initializer_list<collectable_t>& init_list)`
    * Methods
        * `(const) c_array_t& c_array() (const)`
* `template <typename collectable_t> class dynamic_array : public dynamic_collection<collectable_t`
    * Description: 1-dimensional, dynamic size, homogoneous collection
    * Constructors
        * `dynamic_array()`
        * `dynamic_array(const initializer_list<collectable_t>& init_list)`
        * `dynamic_array(const dynamic_array<collectable_t>& other)`
        * `dynamic_array(index size)`

# Functional Programming
stdcol also provides functions for manipulating collections

## Functions
* `size()`: Returns the size of a data structure
    * `template <typename collectable_t> const index size(collection<collectable_t>& collection)`: Returns the size of the collection
    * `template <typename T, index::int_type ssize> constexpr cosnt index::int_type size(const T (&array)[ssize])`: Returns the size of the c-style array
* `at()`: Returns a pointer to an element at a specified location
    * `template <typename collectable_t> (const) collectable_t* const at((const) collection<collectable_t>& collection, index idx)`: Returns a pointer to the element at the specified index
* `begin()`: Returns a begin iterator
    * `template <typename collectable_t> typename collection<collectable_t>::iterator begin(collection<collectable_t>& collection)`
    * `template <typename T> linked_iterator<T> begin(linked<T>& linked)`
* `end()`: Returns an end iterator
    * `template <typename collectable_t> typename collection<collectable_t>::iterator end(collection<collectable_t>& collection)`
    * `template <typename T> linked_iterator<T> end(linked<T>& linked)`
* `wrap()`: Wraps any subscriptable type to a `collection`
    * `template <typename T, index::int_type ssize> wrapper<T[ssize], T> wrap(T(&array)[ssize])`
    * `template <typename T> wrapper<T*, T> wrap(T*& array, index::int_type ssize)`
* `iterate()`: Iterate through iterator
    * `template <typename iterator_t> iterable<iterator_t> iterate(iterator_t begin, iterator_t end)`
    * `template <typename array_t, index::int_type size> iterable<array_t*> iterate(array_t (&array)[size])`
    * `template <typename iterator_t, typename dereference_t> enumerable<iterator_t, dereference_t>& iterate(enumerable<iterator_t, dereference_t>& enumerable)`
    * `template <typename iterator_t, typename dereference_t> enumerable<iterator_t, dereference_t> iterate(enumerable<iterator_t, dereference_t>&& enumerable)`
    * `template <typename collectable_t> collection<collectable_t>& iterate(collection<collectable_t>& collection)`
    * `template <typename T> iterable<linked_iterator<T>> iterate(linked<T>& linked)`
    * `template <typename collectable_t> iterable<const collectable_t*> iterate(const initializer_list<collectable_t>& init_list)`
* `enumerate()`: Enumerate iterator, returns an iterator that dereferences (operator*()) to a `enumeration`
    * `template <typename iterator_t, typename dereference_t> enumerable<iterator_t, dereference_t> enumerate(iterator_t begin, iterator_t end)`
    * `template <typename array_t, index::int_type size> enumerable<array_t*, array_t&> enumerate(array_t (&array)[size])`
    * `template <typename collectable_t> enumerable<typename collection<collectable_t>::iterator, typename collection<collectable_t>::iterator_dereference_t> enumerate(collection<collectable_t>& collection)`
    * `template <typename T> enumerable<linked_iterator<T>, T&> enumerate(linked<T>& linked)`
    * `template <typename collectable_t> enumerable<const collectable_t*, const collectable_t&> enumerate(const initializer_list<collectable_t>& init_list)`
* `hash()`: hashes a `hashable_t` with default-hasher being `hasher` from using-declaration
    * `template <typename hashable_t, typename hasher_t = hasher<hashable_t>> index hash(const hashable_t& hashable)`

## Operators
Some implemented types have their own operators defined, but some operator overloads exist for all collections.
* `template <typename collectable_t> dynamic_collection<collectable_t>& operator+=(dynamic_collection<collectable_t>& collection, const collectable_t& item)`: for `col += item`, adds to the end
* `template <typename collectable_t> dynamic_collection<collectable_t>& operator-=(dynamic_collection<collectable_t>& collection, const collectable_t& item)`: for `col -= item`, removes if exists

# Definitions and Macros
* Platform-dependent Macros
    * stdcol_platform_arduino
    * stdcol_platform_macos
    * stdcol_platform_linux
    * stdcol_platform_windows
    * stdcol_platform_generic
    * stdcol_nostl

# Best Practices
* The functional programming paradigm is best for this library has some function overloads that are more efficient for the data structure such as `iterate(linked)`
