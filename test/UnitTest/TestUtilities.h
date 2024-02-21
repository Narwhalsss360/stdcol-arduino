#ifndef TestUtilities_h
#define TestUtilities_h

#include "TestPlatform.h"
#include <stdcol.h>

template <typename T>
Platform::ostream& operator<<(Platform::ostream& stream, stdcol::graph_node<T>* node) {
    stream << node->get_value();
    return stream;
}

template <typename collectable_t>
Platform::ostream& operator<<(Platform::ostream& stream, stdcol::collection<collectable_t>& collection) {
    stdcol::index size = collection.size();
    stream << '[';
    for (auto i : stdcol::enumerate(stdcol::iterate(collection))) {
        stream << i.value;
        if (i.index != size - 1) {
            stream << ", ";
        }
    }
    stream << ']';
    return stream;
}

#endif

