#include <stdcol>
#include <string>
#include <iostream>
#include "stringifiers.h"

struct person {
    std::string name;
    int age;
};

std::string person_stringifier(person& p) {
    std::string str = p.name;
    str += ":";
    str += std::to_string(p.age);
    return str;
}

std::string int_ref_to_string(int& i) {
    return std::to_string(i);
}

int main() {
    using std::cout;
    using std::endl;

    using stdcol::dynamic_array;
    using namespace stdcol::operators;
    using namespace stdcol::stringifiers;
    using printers::global_printer;

    printers::basic basic_printer;
    printers::indexed indexed_printer;

    dynamic_array<int> ints;
    for (int i = -10; i <= 10; i += 2) {
        ints += i;
    }

    cout << to_string(ints) << '\n';
    cout << to_string(ints, indexed_printer) << '\n';
    cout << to_string(ints, global_printer, int_ref_to_string) << '\n';

    dynamic_array<person> people;
    people += { "Infant", 1 };
    people += { "Toddler", 3 };
    people += { "Child", 9 };
    people += { "Teenager", 15 };

    cout << to_string(people, global_printer, person_stringifier);

    cout << endl;
    return 0;
}
