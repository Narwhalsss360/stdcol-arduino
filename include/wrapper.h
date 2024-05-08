#ifndef wrapper_h
#define wrapper_h

#include "collection.h"

namespace stdcol {
    template <typename subscriptable_t, typename collectable_t>
    class wrapper : public collection<collectable_t> {
    public:
        wrapper(subscriptable_t& subscriptable, const index ssize) : subscriptable(subscriptable), ssize(ssize) {}

        collectable_t* const at(index index) override {
            return &(subscriptable[index]);
        }

        const collectable_t* const at(index index) const override {
            return &(subscriptable[index]);
        }

        index size() const override {
            return ssize;
        }

        subscriptable_t& subscriptable;
        const index ssize;
    };

    template <typename T, index::int_type ssize>
    constexpr const index::int_type size(const T (&array)[ssize]) {
        return ssize;
    }

    template <typename T, index::int_type ssize>
    wrapper<T[ssize], T> wrap(T(&array)[ssize]) {
        return wrapper<T[ssize], T>(array, ssize);
    }

    template <typename T>
    wrapper<T*, T> wrap(T*& array, index::int_type ssize) {
        return wrapper<T*, T>(array, ssize);
    }
}

#endif
