#include <stdcol>
#include <string>
#include <iostream>
#include "../stringifiers/stringifiers.h"

using std::string;

class Occupation {
public:
    Occupation()
        : title("None") {}

    Occupation(string title)
        : title(title) {}

    string title;
};

class Person {
public:
    Person(string name, Occupation occupation)
        : name(name), occupation(occupation) {}

    string get_name() { return name; }

    Occupation get_occupation() { return occupation; }

private:
    string name;
    Occupation occupation;
};

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

template <typename collectable_t>
stdcol::stringifiers::string_t prioritized_to_string(stdcol::prioritized_element<collectable_t>& pe) {
    return to_string(pe.item);
}

int main() {
    using std::cout;
    using std::endl;

    //These are all 1-dimensional lists.

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

    using namespace stdcol::stringifiers;

    dynamic_array<int> starting_ints;
    for (int x = -8; x <= 8; x += 2) {
        using namespace stdcol::operators; //+= and -= for dynamic_collection
        (starting_ints += x) += x;
    }
    cout << "starting ints:" << to_string(starting_ints) << '\n';

    set<int> intset;
    copy_to(starting_ints, intset);
    cout << "int set" << to_string(intset) << '\n';

    queue<int> intq;
    copy_to(starting_ints, intq);
    cout << "q:" << to_string(intq) << '\n';
    cout << "dequeueing...\n";
    int value;
    while (intq -= value) {
        cout << '\t' << value << '\n';
    }

    priority_queue<int> pintq;
    for (int n : starting_ints) {
        pintq += { n, n };
    }
    cout << "dequeueing prioritized...\n";
    while (pintq -= value) {
        cout << '\t' << value << '\n';
    }

    linked<Person> people;
    people.emplace(0, "Name", Occupation("Job"));
    people.emplace(1, "Nikolai Chub", Occupation("Astronaut"));

    cout << "The people are:\n";
    for (auto& person : stdcol::iterate(people)) { //Use iterate for most optimized iterator, or use built-in begin/end in collection for all general cases
        cout << '\t' << person.get_name() << ":" << person.get_occupation().title << '\n';
    }

    cout << endl;
    return 0;
}
