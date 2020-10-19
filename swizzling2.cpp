#include <array>
#include <iostream>
#include <tuple>
#include <type_traits>


constexpr std::size_t axisIndex(char axis) {
    switch (axis) {
        case 'x': return 0;
        case 'y': return 1;
        case 'z': return 2;
        case 'w': return 3;

        case 'r': return 0;
        case 'g': return 1;
        case 'b': return 2;
        case 'a': return 3;

        case 's': return 0;
        case 't': return 1;

        case 'u': return 0;
        case 'v': return 1;

        default: return 0;
    }
}

template<std::size_t N, class T = float>
struct Vec : std::array<T, N> {
    template<class... U>
    class Ref {
    public:
        constexpr explicit Ref(U&... references)
            : references_{references...} {}

        constexpr Ref& operator=(Vec<sizeof...(U), T> vector) {
            std::apply([this](auto&&... coefficient) {
                references_ = std::tuple{std::move(coefficient)...};
            }, vector.iterable());
            return *this;
        }

    private:
        std::tuple<U&...> references_;
    };

    template<std::size_t M>
    constexpr auto operator[](char const (&axes)[M]) const {
        return [this, &axes]<std::size_t... I>(std::index_sequence<I...>) {
            return Vec<M - 1, T>{this->at(axisIndex(axes[I]))...};
        }(std::make_index_sequence<M - 1>{});
    }

    template<std::size_t M>
    constexpr auto operator[](char const (&axes)[M]) {
        return [this, &axes]<std::size_t... I>(std::index_sequence<I...>) {
            return Ref{this->at(axisIndex(axes[I]))...};
        }(std::make_index_sequence<M - 1>{});
    }

    constexpr std::array<T, N> const& iterable() const {
        return *this;
    }
};

template<std::size_t N, class T>
std::ostream& operator<<(std::ostream& os, Vec<N, T> const& vector) {
    std::apply([&os](auto&&... coefficient) {
        ((os << coefficient << ' '), ...) << '\n';
    }, vector.iterable());
    return os;
}


int main() {
    // Compile-time swizzling
    Vec<3> constexpr sequence{2, 4, 6};
    Vec<3> constexpr reverse = sequence["zyx"];
    std::cout << sequence << reverse;

    // Can be used to change entries in place
    Vec<3> coordinates{1, 0, 0};
    coordinates["xy"] = {0, 1};
    coordinates["z"] = sequence["x"];
    std::cout << coordinates;

    // Note: value categories for operator[] could be refined...
    return 0;
}
