#ifndef stdcol_init_list_h
#define stdcol_init_list_h

#include "stdcol_platform.h"

/*
#ifdef __has_include
#if !__has_include(<initializer_list>)
#define stdcol_init_list_required
#endif
#else
#if !defined(__STD_HEADER_INITIALIZER_LIST) && !defined(_INITIALIZER_LIST_)
#define stdcol_init_list_required
#endif
#endif
*/

#ifdef stdcol_nostl
#include <stddef.h>

namespace std {
    template <typename T>
    class initializer_list {
    public:
        constexpr initializer_list() noexcept : _begin(nullptr), _size(0) {}

        constexpr const T* begin() const noexcept { return _begin; }

        constexpr const T* end() const noexcept { return _begin + _size; }

        constexpr const size_t size() const noexcept { return _size; }

    private:
        const T* _begin;
        size_t _size;
    };
}
#else
#include <initializer_list>
#endif

#include "enumerables.h"

namespace stdcol {
    template <typename collectable_t>
    using initializer_list = std::initializer_list<collectable_t>;

    template <typename collectable_t>
    iterable<const collectable_t*> iterate(const initializer_list<collectable_t>& init_list) {
        return iterate(init_list.begin(), init_list.end());
    }

    template <typename collectable_t>
    enumerable<const collectable_t*, const collectable_t&> enumerate(const initializer_list<collectable_t>& init_list) {
        return enumerate<const collectable_t*, const collectable_t&>(init_list.begin(), init_list.end());
    }
}

#endif
