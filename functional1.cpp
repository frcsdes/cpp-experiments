#include <iostream>
#include <string>


template<class T>
class Literal {
public:
    explicit Literal(T value)
        : m_value(std::forward<T>(value)) {}

    auto operator+(T right) { return m_value + right; }
    auto operator-(T right) { return m_value - right; }
    auto operator*(T right) { return m_value * right; }
    auto operator/(T right) { return m_value / right; }

    auto operator()() const {
        return m_value;
    }

    template<class F>
    auto operator()(F functor) const {
        return functor(m_value);
    }

    template<class F, class... FPack>
    auto operator()(F functor, FPack... pack) const {
        return Literal<decltype(functor(m_value))>(functor(m_value))(pack...);
    }

private:
    T const m_value;
};

template<class T, class F>
class PartialBinary {
public:
    explicit PartialBinary(T right)
        : m_right(std::forward<T>(right)) {};

    template<class U>
    auto operator()(U left) const {
        return F::apply(left, m_right);
    }

private:
    T m_right;
};

template<class T>
struct PlusFunctor {
    template<class U>
    static auto apply(U left, T right) {
        return left + right;
    }
};

template<class T>
struct MinusFunctor {
    template<class U>
    static auto apply(U left, T right) {
        return left - right;
    }
};

template<class T>
struct TimesFunctor {
    template<class U>
    static auto apply(U left, T right) {
        return left * right;
    }
};

template<class T>
struct DividedFunctor {
    template<class U>
    static auto apply(U left, T right) {
        return left / right;
    }
};

template<class T>
using Plus = PartialBinary<T, PlusFunctor<T>>;
template<class T>
using Minus = PartialBinary<T, MinusFunctor<T>>;
template<class T>
using Times = PartialBinary<T, TimesFunctor<T>>;
template<class T>
using Divided = PartialBinary<T, DividedFunctor<T>>;

class Square {
public:
    template<class U>
    auto operator()(U operand) const {
        return operand * operand;
    }
};


int main() {
    Times<int> times2(2);
    Plus<int> plus3(3);
    Square square;
    Divided<int> divided4(4);

    Literal<int> one(1);
    auto result = one(times2, plus3, times2, square, divided4);

    Plus<std::string> times2_d(" * 2");
    Plus<std::string> plus3_d(" + 3");
    Plus<std::string> square_d(" ^ 2");
    Plus<std::string> divided4_d(" / 4");
    Literal<std::string> one_d("1");
    auto detail = one_d(times2_d, plus3_d, times2_d, square_d, divided4_d);

    std::cout
        << "Folding from left to right,\n"
        << detail << " = " << result << std::endl;

    return 0;
}
