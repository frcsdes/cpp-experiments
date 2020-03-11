#include <iostream>


struct Verbose {
    Verbose() {
        std::cout << "Default constructor\n";
    }

    Verbose(Verbose const&) {
        std::cout << "Copy constructor\n";
    }

    Verbose(Verbose&&) noexcept {
        std::cout << "Move constructor\n";
    }

    Verbose& operator=(Verbose const&) {
        std::cout << "Copy assignment\n";
        return *this;
    }

    Verbose& operator=(Verbose&&) noexcept {
        std::cout << "Move assignment\n";
        return *this;
    }
};

std::ostream& operator<<(std::ostream& os, Verbose const&) {
    std::cout << "Stream out\n";
    return os;
}

std::istream& operator<<(std::istream& is, Verbose const&) {
    std::cout << "Stream ind\n";
    return is;
}
