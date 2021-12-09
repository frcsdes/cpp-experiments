#include <iostream>
#include <type_traits>


// Jorg Brown's "Reducing Template Compilation Overhead"
// Could variadic inheritance apply here?
template<class T, class... Rest>
class Collection : Collection<Rest...> {
    using Base = Collection<Rest...>;

public:
    explicit Collection(T&& value, Rest&&... rest)
        : Base{std::forward<Rest>(rest)...}, value_{value} {}

    template<class U>
    requires (std::is_same_v<T, U>)
    U& get() {
        return value_;
    }

    template<class U>
    requires (!std::is_same_v<T, U>)
    U& get() {
        return Base::template get<U>();
    }

    auto sum() const {
        return value_ + Base::sum();
    }

    void print() const {
        std::cout << value_ << ' ';
        Base::print();
    }

private:
    T value_;
};

template<class T>
class Collection<T> {
public:
    explicit Collection(T&& value)
        : value_{value} {}

    void print() const {
        std::cout << value_ << std::endl;
    }

    template<class U>
    U& get() {
        return value_;
    }

    auto sum() const {
        return value_;
    }

private:
    T value_;
};


int main() {
    // Much like a tuple with (unique) typed access and accumulating operations
    // An empty base case is simpler but does not allow specific tail behaviour

    Collection collection{32, 3.14f, 2.718};
    collection.get<double>() = 6.86;
    collection.print();
    std::cout << "Sum is " << collection.sum() << "\n\n";

    Collection woops{32, 3.14, 2.718};
    woops.get<double>() = 6.86;
    woops.print();

    return 0;
}
