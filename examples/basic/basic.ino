#include <stdcol.h>

const constexpr unsigned long BAUDRATE = 115200;

template <typename collectable_t>
String toString(stdcol::collection<collectable_t>& collection) {
    String out;
    const stdcol::index last = size(collection) - 1;
    out += '[';
    for (auto e : stdcol::enumerate(collection)) {
        out += e.value;
        if (e.index != last) {
            out += ", ";
        }
    }
    out += ']';
    return out;
}

template <typename collectable_t>
void copy_to(stdcol::collection<collectable_t>& src, stdcol::collection<collectable_t>& dst) {
    int csize = size(src);
    {
        int dstsize = size(dst);
        if (dstsize < csize) {
            csize = dstsize;
        }
    }

    for (stdcol::index i = 0; i < csize; i++) {
        dst[i] = src[i];
    }
}

template <typename collectable_t>
void copy_to(stdcol::collection<collectable_t>& src, stdcol::dynamic_collection<collectable_t>& dst) {
    using namespace stdcol::operators;

    int csize = size(src);
    dst.resize(0);
    dst.reserve(csize);

    for (stdcol::index i = 0; i < csize; i++) {
        dst += src[i];
    }
}

void setup() {
    //collection
    using stdcol::array; //Static size.

    //dynamic_collection : collection
    //Dynamic size lists, you can insert, remove, resize, reserve and find capacity.
    using stdcol::dynamic_array;
    using stdcol::set; //Only unique items
    using stdcol::queue; //dynamic_array with FIFO order of (enqueue()) += and (dequeue()) -= operators
    using stdcol::stack; //dynamic_array with LIFO order of (push()) += and (pop()) -= operators
    using stdcol::priority_queue; //queue with specific priority for each element.
    using stdcol::linked; //linked list, with emplace() for constructing items in place (no copy constructor).
    using stdcol::linked_node; //linked list node

    Serial.begin(BAUDRATE);
    while (!Serial); //Wait for serial

    dynamic_array<int> starting_ints;
    for (int x = -8; x <= 8; x += 2) {
        using namespace stdcol::operators; //+= and -= for dynamic_collection
        (starting_ints += x) += x;
    }

    Serial.print("ints: ");
    Serial.println(toString(starting_ints));

    set<int> intset;
    copy_to(starting_ints, intset);
    Serial.print("int set");
    Serial.println(toString(intset));

    queue<int> intq;
    copy_to(starting_ints, intq);
    Serial.print("q:");
    Serial.println(toString(intq));

    Serial.println("dequeueing...");
    int value;
    while (intq -= value) {
        Serial.print('\t');
        Serial.println(value);
    }

    priority_queue<int> pintq;
    for (int n : starting_ints) {
        pintq += { n, n };
    }

    Serial.println("dequeueing prioritized...");
    while (pintq -= value) {
        Serial.print('\t');
        Serial.println(value);
    }
}

void loop() {}
