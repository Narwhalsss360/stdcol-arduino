#ifndef TestUtilities_h
#define TestUtilities_h

#include "TestPlatform.h"

#ifdef plat_arduino
#include <StreamUtilities.h>
#define ostream_typename Print
#else
#include <iostream>
#define ostream_typename std::ostream
#endif

#include <stdcol>

template <typename collectable_t>
ostream_typename& operator<<(ostream_typename& stream, stdcol::collection<collectable_t>& collection) {
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

