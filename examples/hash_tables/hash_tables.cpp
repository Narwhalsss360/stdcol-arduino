#include <iostream>
#include <string>
#include <stdcol>
#include <chrono>
#include <algorithm>

#ifdef stdcol_platform_windows
#include <xhash>
#else
#include <system_error>
#endif

class sensor_sample {
public:
    sensor_sample(std::time_t at = 0, double value = 0)
        : at(at), value(value) {}

    std::time_t time_of_measurement() const {
        return at;
    }

    double measurement() const {
        return value;
    }

    bool operator==(const sensor_sample& other) const {
        return this == &other;
    }

private:
    std::time_t at;
    double value;
};

//To use a custom type as a key_t in a dictionary, you must create a hasher. the type must also define == operator
//use this type name for automatic detection. if not you must specify the type in the template parameter for hash_table<>
template <>
struct stdcol::stdcol_hasher<sensor_sample> {
    index operator()(const sensor_sample& sample) {
        return std::hash<std::time_t>()(sample.time_of_measurement());
    }
};

int main() {
    using namespace std::chrono;

    using std::string;
    using std::cout;
    using std::cin;
    using std::endl;
    using std::getline;
    using std::find;
    using std::hash;

    using stdcol::hash_table;
    using stdcol::collection;
    using stdcol::size;
    using stdcol::index;

    //using stl std::hash for std::string since it already exists. must create your own if it does not.
    hash_table<string, string, std::hash<string>> user_entries;
    while (true) {
        string line;
        getline(cin, line);

        if (line.rfind("exit") == 0) {
            break;
        }

        if (line.rfind("show") == 0) {
            cout << "{\n";
            for (auto& bucket : user_entries.buckets()) {
                for (auto kvp : bucket) {
                    cout << '\t' << kvp.key << ':' << kvp.value << '\n';
                }
            }
            cout << "}\n";
            continue;
        }

        auto colon_location = find(line.begin(), line.end(), ':');

        if (colon_location == line.end()) {
            cout << "Error! must use key:value syntax\nyou can run 'show' or 'exit' command";
            continue;
        }

        string key = string(line.begin(), colon_location);
        string value = string(colon_location + 1, line.end());
        cout << "Entered { " << key << ':' << value << " }\n";
        if (user_entries.contains(key)) {
            cout << "Error! key already exists\n";
            continue;
        }
        user_entries.add(key, value);
    }

    hash_table<sensor_sample, int> samples_to_ints;
    time_t now;
    time(&now);
    samples_to_ints.add(sensor_sample(now, 1), 1);

    cout << endl;
    return 0;
}
