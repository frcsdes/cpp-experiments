#include <cmath>
#include <functional>
#include <iostream>


template<class T, class U>
class Unary {
    using Functor = std::function<U(T)>;

public:
    explicit Unary(Functor functor)
        : m_functor(std::move(functor)) {}

    U operator()(T operand) const {
        return m_functor(operand);
    }

    template<class V>
    auto operator,(Unary<U, V> const& other) const {
        return Unary<T, V>([this, &other](T operand) {
            return other((*this)(operand));
        });
    }

private:
    Functor m_functor;
};

int main() {
    Unary<float, float> square([](float x) {
        return x * x;
    });
    Unary<float, int> round([](float x) {
        return static_cast<int>(std::round(x));
    });
    auto composition = (square, round);
    std::cout << composition(3.14f) << std::endl;

    return 0;
}
