#include <algorithm>
#include <iostream>
#include <string>


template<std::size_t N>
struct KeyOrValue {
    constexpr KeyOrValue(char const (&key)[N]) {
        std::copy_n(key, N, buffer);
    }

    constexpr KeyOrValue(char value) {
        *buffer = value;
    }

    void print() const {
        std::cout << buffer << ": ";
    }

    char buffer[N];
};

KeyOrValue(char) -> KeyOrValue<1>;

template<>
void KeyOrValue<1>::print() const {
    std::cout << std::to_string(*buffer) << '\n';
}

template<KeyOrValue... Entries>
class Map {
public:
    void print() const {
        (Entries.print(), ...);
    }
};


int main() {
    Map<"Apples", 5, "Lemons", 1> map;
    map.print();
    return 0;
}
