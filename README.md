# stdcol
stdcol is cross-platfrom library for storing data in data structures.
stdcol provides the interfaces, and implemented classes.

When you see `(const)` anywhere, that means that the declaration was declared as both const and non-const

# Helper (Non-collection) Types
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

# Implemented Types

# Functional Programming
stdcol also provides functions for manipulating collections

## Functions
* `size()`: Returns the size of a data structure
    * `template <typename collectable_t> const index size(collection<collectable_t>& collection)`: Returns the size of the collection
    * `template <typename T, index::int_type ssize> constexpr cosnt index::int_type size(const T (&array)[ssize])`: Returns the size of the c-style array
* `at()`: Returns a pointer to an element at a specified location
    * `template <typename collectable_t> (const) collectable_t* const at((const) collection<collectable_t>& collection, index idx)`: Returns a pointer to the element at the specified index
* `begin()`: Returns a begin iterator
* `end()`: Returns an end iterator
* `wrap()`: Wraps any subscriptable type to a `collection`
* `iterate()`: Iterate through iterator
* `enumerate()`: Enumerate iterator, returns an iterator that dereferences (operator*()) to a `enumeration`

## Operators
Some implemented types have their own operators defined, but some operator overloads exist for all collections.

# Best Practices
