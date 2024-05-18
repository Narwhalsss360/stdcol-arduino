#ifndef array_h
#define array_h

#include "collection.h"
#include "stdcol_init_list.h"

namespace stdcol {
    template <typename collectable_t, index::int_type static_size>
    class array : public collection<collectable_t> {
    public:
        using c_array_t = collectable_t[static_size];

        array() : static_array{ collectable_t() } {}

        array(const initializer_list<collectable_t>& init_list) : array<collectable_t, static_size>() {
            *this = init_list;
        }

        collectable_t* const at(index index) override {
            return static_array + index;
        }

        const collectable_t* const at(index index) const override {
            return static_array + index;
        }

        array<collectable_t, static_size>& operator=(const initializer_list<collectable_t>& init_list) {
            for (auto eitem : enumerate(init_list)) {
                if (eitem.index == static_size) {
                    break;
                }
                static_array[eitem.index] = eitem.value;
            }
            return *this;
        }

        index size() const override {
            return static_size;
        }

        c_array_t& c_array() {
            return static_array;
        }

        const c_array_t& c_array() const {
            return static_array;
        }

    protected:
        collectable_t static_array[static_size];
    };
}

#endif
