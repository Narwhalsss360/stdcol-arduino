#ifndef dynamic_collection_h
#define dynamic_collection_h

#include "collection.h"

namespace stdcol {
    template <typename collectable_t>
    class dynamic_collection : public collection<collectable_t> {
    public:
        virtual index capacity() const = 0;

        virtual bool reserve(index new_capacity) = 0;

        virtual bool resize(index new_size) = 0;

        virtual bool insert(index index, const collectable_t& item) = 0;

        virtual bool remove(index index) = 0;
    };
}

#endif
