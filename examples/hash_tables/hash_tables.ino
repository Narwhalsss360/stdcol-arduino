#include <stdcol.h>

using namespace stdcol;

constexpr const unsigned long BAUDRATE = 115200;

/*
    To use a hash_table, you must make a hasher for the type key_t that you use for the hash_table.
    A hasher is function that converts a data type into a number.
    this one is auto-detected by all hash_tables that BELOW this struct declaration.
*/
template <>
struct stdcol::stdcol_hasher<String> {
    index operator()(const String& s) {
        index sum = 0;
        for (const char& c : s) {
            sum += (index::int_type)c;
        }
        return sum;
    }
};


/*
    If you have custom hasher:
struct hasher {
    index operator()(const String& s) {
        index sum = 0;
        for (const char& c : s) {
            sum += (index::int_type)c;
        }
        return sum;
    }
};

hash_table<String, String, hasher> userEntries;
*/

hash_table<String, String> userEntries;

void reset() {
    //Cast address 0 to a function and call it.
    ((void(*)())nullptr)();
}

void setup() {
    Serial.begin(BAUDRATE);
    while (!Serial); //Wait for serial to open
    Serial.println("Initialized! Use LF (Line Feed) line ending.");
}

void loop() {
    while (!Serial.available());
    String line = Serial.readStringUntil('\n');

    //CTRL-C: End Of Text
    if (line.startsWith("\03")) {
        reset();
    }

    if (line.startsWith("reset")) {
        reset();
    }

    if (line.startsWith("show")) {
        Serial.println('{');
        for (auto& bucket : userEntries.buckets()) {
            for (auto kvp : bucket) {
                Serial.print('\t');
                Serial.print(kvp.key);
                Serial.print(':');
                Serial.println(kvp.value);
            }
        }
        Serial.println('}');
        return;
    }

    int colonIndex = line.indexOf(':');

    if (colonIndex == -1) { //Colon not found.
        Serial.println("Error! must use key:value syntax\nyou can run 'show' command");
        return;
    }

    String key = line.substring(0, colonIndex);
    String value = line.substring(colonIndex + 1, line.length());

    if (userEntries.contains(key)) {
        Serial.println("Error! key already exists.");
        return;
    }

    userEntries.add(key, value);
    Serial.print("Entered { ");
    Serial.print(key);
    Serial.print(':');
    Serial.print(value);
    Serial.println(" }");
}
