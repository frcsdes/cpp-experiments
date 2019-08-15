#include <cmath>
#include <functional>
#include <iostream>


template<class T, class U>
class Unary {
    using Functor = std::function<U(T)>;

public:
    explicit Unary(Functor functor)
        : m_functor(std::move(functor)) {}

    U operator()(T x) const {
        return m_functor(x);
    }

private:
    Functor m_functor;
};

template<class T, class U, class V>
auto operator,(Unary<T, U> const& lhs, Unary<U, V> const& rhs) {
    return Unary<T, V>([&lhs, &rhs](T x) {
        return rhs(lhs(x));
    });
}

template<class T>
auto operator^(Unary<T, T> const& unary, unsigned int n) {
    return Unary<T, T>([&unary, n](T x) {
        for (unsigned int i = 0; i < n; ++i)
            x = unary(x);
        return x;
    });
}


int main() {
    Unary<int, int> add1([](int x) {
        return x + 1;
    });
    Unary<int, int> square([](int x) {
        return x * x;
    });
    Unary<int, double> sqrt([](int x) {
        return std::sqrt(x);
    });

    auto composition = (square, add1 ^ 5, sqrt);
    std::cout
        << "Computing sqrt(2 ^ 2 + 5) = "
        << composition(2) << std::endl;

    return 0;
}
