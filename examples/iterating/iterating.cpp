#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <stdcol>

const double pi = std::acos(-1);

int main() {
    using std::cout;
    using std::fixed;
    using std::setprecision;
    using std::endl;
    using std::sin;

    using stdcol::dynamic_array;

    using stdcol::iterate;
    using stdcol::enumerate;
    using stdcol::enumeration;
    using namespace stdcol::operators; //for += and -=


    const double sample_rate = 0.25;
    dynamic_array<double> samples;

    for (double x = 0; x <= 5; x += sample_rate) {
        samples += (x * sin(2*pi*x)) / 5;
    }

    cout << "samples:\n";
    cout << fixed << setprecision(3);

    //enumeration has index, and value. use enumerate() to get enumerations.
    for (enumeration<double&> e : enumerate(samples)) {
        double value = e;
        //Other possible ways:
        //double value = (double)e;
        //double value = e.value;
        cout << "\tat " << e.index * sample_rate << ":   " << (value < 0 ? "" : " " ) << value << '\n';
    }

    cout << endl;
    return 0;
}
