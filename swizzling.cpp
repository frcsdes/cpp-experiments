#include <array>
#include <iostream>
#include <tuple>


template<char... C>
struct Mask {};

template<char... C>
constexpr Mask<C...> operator""_m() {
    return {};
}


template<std::size_t N, class T = float>
struct Vec : std::array<T, N> {};

template<std::size_t N, class T, char... C>
constexpr Vec<sizeof...(C), T> operator|(Vec<N, T> const& vector, Mask<C...>) {
    return {std::get<static_cast<std::size_t>(C - 48)>(vector)...};
}

template<std::size_t N, class T>
std::ostream& operator<<(std::ostream& os, Vec<N, T> const& vector) {
    std::apply([&os](auto&&... coefficient) {
        ((os << coefficient << ' '), ...) << '\n';
    }, static_cast<std::array<T, N> const&>(vector));
    return os;
}


int main() {
    // Swizzling by using a mask of indices
    // Example: vec.zyx becomes vec | 210_m

    Vec<4> constexpr sequence{10, 11, 12, 13};
    Vec<4> constexpr reverse = sequence | 3210_m;
    std::cout << sequence << reverse;

    Vec<2> constexpr bernoulli{-1, 1};
    auto constexpr samples = bernoulli | 01110100_m;
    std::cout << samples;

    return 0;
}
