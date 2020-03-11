#include <array>
#include <iostream>
#include <tuple>


template<char... C>
struct Mask {};

template<char... C>
constexpr Mask<C...> operator""_m() {
    return {};
}


template<auto N, class T = float>
struct Vector : std::array<T, N> {};

template<auto N, class T, char... C>
constexpr Vector<sizeof...(C), T>
operator|(Vector<N, T> const& vector, Mask<C...>) {
    return {std::get<static_cast<std::size_t>(C - 48)>(vector)...};
}

template<auto N, class T>
std::ostream& operator<<(std::ostream& os, Vector<N, T> const& vector) {
    std::apply([](auto... c) {
        ((std::cout << c << ' '), ...) << '\n';
    }, static_cast<std::array<T, N>>(vector));
    return os;
}


int main() {
    // Swizzling by using a sequence of indices
    // Example: vec.zyx becomes vec | 210_m

    constexpr Vector<4> sequence{10, 11, 12, 13};
    constexpr auto reverse = sequence | 3210_m;
    std::cout << sequence << reverse;

    constexpr Vector<2> bernoulli{-1, 1};
    constexpr auto samples = bernoulli | 01110100_m;
    std::cout << samples;

    return 0;
}
