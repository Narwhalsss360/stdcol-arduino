#ifndef enumerables_h
#define enumerables_h

#include "iterables.h"

namespace stdcol {
    template <typename value_t>
    struct enumeration {
        findex index;

        value_t value;

        enumeration(stdcol::index index, value_t value) : index(index), value(value) {}

        operator value_t() { return value; }

        operator value_t() const { return value; }

        void operator=(const value_t& value) { this->value = value; }
    };

    template <typename iterator_t, typename dereference_t>
    class enumerator {
    public:
        enumerator(iterator_t iterator, index idx = 0) : iterator(iterator), idx(idx) {}

        bool operator!=(const enumerator<iterator_t, dereference_t>& other) const { return iterator != other.iterator; }

        void operator++() { ++idx; ++iterator; }

        void operator++(int) { operator++(); }

        enumeration<dereference_t> operator*() const { return enumeration<dereference_t>(idx, *iterator); }

    protected:
        iterator_t iterator;

        index idx;
    };

    template <typename iterator_t, typename dereference_t>
    using enumerable = iterable<enumerator<iterator_t, dereference_t>>;

    template <typename iterator_t, typename dereference_t>
    enumerable<iterator_t, dereference_t>& iterate(enumerable<iterator_t, dereference_t>& enumerable) {
        return enumerable;
    }

    template <typename iterator_t, typename dereference_t>
    enumerable<iterator_t, dereference_t> iterate(enumerable<iterator_t, dereference_t>&& enumerable) {
        return enumerable;
    }

    template <typename iterator_t, typename dereference_t>
    enumerable<iterator_t, dereference_t> enumerate(iterator_t begin, iterator_t end) {
        return enumerable<iterator_t, dereference_t>(begin, end);
    }

    template <typename array_t, index::int_type size>
    enumerable<array_t*, array_t&> enumerate(array_t (&array)[size]) {
        return enumerable<array_t*, array_t&>(array, array + size);
    }
}

#endif
