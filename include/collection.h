#ifndef collection_h
#define collection_h

#include "stdcol_index.h"
#include "iterables.h"
#include "enumerables.h"

namespace stdcol {
    template <typename collectable_t>
    class collection {
    public:
        using iterator_dereference_t = collectable_t&;

        using iterator = general_iterator<collection<collectable_t>, iterator_dereference_t>;

        virtual collectable_t* const at(index index) = 0;

        virtual const collectable_t* const at(index index) const = 0;

        virtual index size() const = 0;

        virtual collectable_t& operator[](index index) { return *at(index); }

        virtual const collectable_t& operator[](index index) const { return *at(index); }

        iterator begin() { return iterator(*this, 0); }

        iterator end() { return iterator(*this, size()); }
    };

    template <typename collectable_t>
    const index size(collection<collectable_t>& collection) {
        return collection.size();
    }

    template <typename collectable_t>
    collectable_t* const at(collection<collectable_t>& collection, index idx) {
        return collection.at(idx);
    }

    template <typename collectable_t>
    const collectable_t* at(const collection<collectable_t>& collection, index idx) {
        return collection.at(idx);
    }

    template <typename collectable_t>
    typename collection<collectable_t>::iterator begin(collection<collectable_t>& collection) {
        return collection.begin();
    }

    template <typename collectable_t>
    typename collection<collectable_t>::iterator end(collection<collectable_t>& collection) {
        return collection.end();
    }

    template <typename collectable_t>
    collection<collectable_t>& iterate(collection<collectable_t>& collection) {
        return collection;
    }

    template <typename collectable_t>
    enumerable<typename collection<collectable_t>::iterator, typename collection<collectable_t>::iterator_dereference_t> enumerate(collection<collectable_t>& collection) {
        return enumerable<typename stdcol::collection<collectable_t>::iterator, typename stdcol::collection<collectable_t>::iterator_dereference_t>(collection.begin(), collection.end());
    }
}

#endif
