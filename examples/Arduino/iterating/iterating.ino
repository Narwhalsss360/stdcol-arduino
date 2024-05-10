#include <stdcol.h>

constexpr const unsigned long BAUDRATE = 115200;

using namespace stdcol;

dynamic_array<int> ints;

byte pins[] = { 1, 2, 3, 4, 5 };

void setup() {
    using namespace stdcol::operators; //For += and -=

    for (int i = 1; i <= 30; i += 3) {
        ints += i;
    }

    Serial.begin(BAUDRATE);
    while (!Serial); //Wait for serial

    for (int i : ints) {
        Serial.println(i);
    }
    Serial.println();

    //Add 1 to each
    for (int& i : ints) {
        i += 1;
    }

    //no change, no copy
    for (const int& i : ints) {
        Serial.println(i);
    }
    Serial.println();

    //Regular iteration
    for (int i : iterate(ints)) {
        Serial.println(i);
    }
    Serial.println();

    //or for (auto e : enumerate(ints))
    for (enumeration<int&> e : enumerate(ints)) {
        Serial.print("[");
        Serial.print(e.index);
        Serial.print("]:");
        int value = e;
        //Other ways to get the value
        //value = (int)value;
        //value = e.value;
        Serial.println(value);
    }

    Serial.println("pins:");
    for (auto pin : iterate(pins)) { //or enumerate(pins)
        Serial.print((int)pin);
    }
}

void loop() {

}
