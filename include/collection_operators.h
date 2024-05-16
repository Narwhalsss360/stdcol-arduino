#ifndef collection_operators_h
#define collection_operators_h

#include "dynamic_collection.h"

namespace stdcol {
    namespace operators {
        template <typename collectable_t>
        dynamic_collection<collectable_t>& operator+=(dynamic_collection<collectable_t>& collection, const collectable_t& item) {
            collection.insert(collection.size(), item);
            return collection;
        }

        template <typename collectable_t>
        dynamic_collection<collectable_t>& operator-=(dynamic_collection<collectable_t>& collection, const collectable_t& item) {
            for (auto e : enumerate(collection)) {
                if (e.value == item) {
                    collection.remove(e.index);
                    break;
                }
            }
            return collection;
        }
    }
}

#endif

