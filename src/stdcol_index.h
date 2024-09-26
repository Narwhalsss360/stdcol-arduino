#ifndef stdcol_index_h
#define stdcol_index_h

#include <stdint.h>
#include <stddef.h>

namespace stdcol {
    struct findex;

    struct index {
        using int_type = size_t;

        static constexpr const int_type max = SIZE_MAX;

        int_type value;

        index();

        index(const int_type& value);

        int_type& operator=(const int_type& value);

        operator int_type&();

        operator const int_type&() const;
    };

    struct findex : index {
        static constexpr const int_type not_found = max;

        findex();

        findex(const int_type& value);

        bool found() const;
    };
}

#endif
