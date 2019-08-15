#include <functional>
#include <iostream>


template<class T>
class Variable {
public:
    Variable(T value)
        : m_value(value) {}

    T operator()() {
        return m_value;
    }

private:
    T m_value;
};

template<class T>
class Multiply {
public:
    Multiply(Variable<T> variable)
        : m_variable(std::move(variable)) {};

    template<class U>
    auto operator()(U operand) {
        return m_variable() * operand();
    }

private:
    Variable<T> m_variable;
};


int main() {
    Multiply<int> times2(Variable<int>(2));
    Variable<float> pi(3.14);
    std::cout << times2(pi) << std::endl;
    return 0;
}
