#ifndef iterables_h
#define iterables_h

#include "stdcol_index.h"

namespace stdcol {
    template <typename subscriptable_t, typename dereference_t>
    class general_iterator {
    public:
        general_iterator(subscriptable_t& subscriptable, index idx) : subscriptable(subscriptable), idx(idx) {}

        bool operator!=(const general_iterator<subscriptable_t, dereference_t>& other) const {
            return idx != other.idx;
        }

        general_iterator<subscriptable_t, dereference_t>& operator++() {
            ++idx;
            return *this;
        }

        general_iterator<subscriptable_t, dereference_t>& operator++(int) {
            ++idx;
            return *this;
        }

        dereference_t operator*() const {
            return subscriptable[idx];
        }

    protected:
        subscriptable_t& subscriptable;

        index idx;
    };

    template <typename iterator_t>
    class iterable {
    public:
        iterable(iterator_t begin, iterator_t end) : i_begin(begin), i_end(end) {}

        iterator_t begin() const { return i_begin; }

        iterator_t end() const { return i_end; }

    protected:
        iterator_t i_begin, i_end;
    };

    template <typename iterator_t>
    iterable<iterator_t> iterate(iterator_t begin, iterator_t end) {
        return iterable<iterator_t>(begin, end);
    }

    template <typename array_t, index::int_type size>
    iterable<array_t*> iterate(array_t (&array)[size]) {
        return iterable<array_t*>(array, array + size);
    }
}

#endif
