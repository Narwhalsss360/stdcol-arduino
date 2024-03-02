#ifndef TestPlatform_h
#define TestPlatform_h

#if defined(ARDUINO)
#define plat_arduino "Arduino"
#define plat plat_arduino

#elif __has_include(<Windows.h>)
#define plat_windows "Windows"
#define plat plat_windows

#else
#define plat_unix "Unix"
#define plat plat_unix

#endif

#ifdef plat_arduino
#include <Arduino.h>
#include <StreamUtilities.h>
#define entry_symbol void setup()
#define entry_return
#else
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#define entry_symbol int main()
#define entry_return return 0
#endif

namespace stdcol {
	template <>
	struct stdcol_hasher<int> {
		index operator()(const int& n) const {
			return static_cast<index>(n);
		}
	};
}

namespace Platform {
#ifdef plat_arduino
    using string = String;

    using stringstream = StringStream;

    using ostream = Print;

    ostream& out = Serial;

    void setup() {
        Serial.begin(115200);
        while (!Serial);
    }

    void keywait() {
        Serial.read();
    }
#else
    using string = std::string;

    using stringstream = std::stringstream;

    using ostream = std::ostream;

    ostream& out = std::cout;

    void setup() {}

    void keywait() {
        std::cin.get();
    }
#endif
}

#endif
