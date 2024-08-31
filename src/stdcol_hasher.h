#ifndef stdcol_hasher_h
#define stdcol_hasher_h

#include "stdcol_index.h"

namespace stdcol {
    template <typename hashable_t>
    struct stdcol_hasher;

    template <typename hashable_t>
    using hasher = stdcol_hasher<hashable_t>;

    template <typename hashable_t, typename hasher_t = hasher<hashable_t>>
    index hash(const hashable_t& hashable) {
        return hasher_t()(hashable);
    }
}

#endif
