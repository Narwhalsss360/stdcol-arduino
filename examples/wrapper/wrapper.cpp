#include <stdcol>
#include <iostream>

//Print collections in python style.
template <typename collectable_t>
std::ostream& operator<<(std::ostream& out, stdcol::collection<collectable_t>& collection) {
    using stdcol::enumerate;
    using stdcol::index;
    using stdcol::size;

    const index csize = size(collection);

    out << '[';
    for (auto e : enumerate(collection)) {
        out << e.value;
        if (e.index != csize - 1) {
            out << ", ";
        }
    }
    out << ']';
    return out;
}

int main() {
    using std::cout;
    using std::endl;
    using stdcol::wrap;
    using stdcol::size;
    using stdcol::index;

    char szText[] = "Lorem ipsum";
    char* pText = szText;

    auto zText = wrap(szText);
    auto text = wrap(pText, size(szText) - 1); //Must use pointer for wrapping c-array with different size.

    cout << "The characters of text are: " << text << endl;
    cout << "All characters are: " << zText << endl;

    return 0;
}

