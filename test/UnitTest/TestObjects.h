#pragma once
#include <string>

//Subscriptable
struct vector3 {
    double x, y, z;

    double& operator[](int idx) {
        idx %= 3;
        switch (idx) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
        return x;
    }
};

struct NonDefaultConstructable {
    unsigned int a;
    int b;

    NonDefaultConstructable(unsigned int a, int b)
        : a(a), b(b) {}
};
