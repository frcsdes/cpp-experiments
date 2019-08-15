#include <iostream>
#include <type_traits>


template<class T, class... TPack>
class Collection : Collection<TPack...> {
    using Base = Collection<TPack...>;

public:
    Collection(T value, TPack... args)
        : Base(args...), m_value(value) {}

    template<class U, std::enable_if_t<std::is_same_v<T, U>, char> = 0>
    U& get() {
        return m_value;
    }

    template<class U, std::enable_if_t<!std::is_same_v<T, U>, char> = 0>
    U& get() {
        return Base::template get<U>();
    }

    auto sum() {
        return m_value + Base::sum();
    }

    void print() const {
        std::cout << m_value << ' ';
        Base::print();
    }

private:
    T m_value;
};

template<class T>
class Collection<T> {
public:
    explicit Collection(T value)
        : m_value(value) {}

    void print() const {
        std::cout << m_value << std::endl;
    }

    template<class U>
    U& get() {
        return m_value;
    }

    auto sum() {
        return m_value;
    }

private:
    T m_value;
};


int main() {
    Collection<int, float, double> collection(42, 3.14f, 2.718);
    collection.get<double>() = 6.626;
    collection.print();
    std::cout << "Sum is " << collection.sum() << std::endl;
    return 0;
}
