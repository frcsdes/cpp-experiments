#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <variant>


struct Error {
    std::string message;
};


template<class... T>
class Result : std::variant<std::tuple<T...>, Error> {
public:
    using Value = std::tuple<T...>;
    using std::variant<Value, Error>::variant;

    template<class... U>
    friend class Result;

    explicit operator bool() const {
        return valid();
    }

    auto operator||(Result const& other) const {
        return valid() ? *this : other;
    }

    template<class... U>
    auto operator&&(Result<U...> const& other) const {
        using Concat = Result<T..., U...>;
        return valid() && other.valid()
             ? Concat{std::tuple_cat(value(), other.value())}
             : valid() ? Concat{other.error()}
                       : Concat{error()};
    }

    template<class F>
    auto apply(F functor) {
        if (valid())
            std::apply(functor, value());
        return *this;
    }

    template<class F>
    auto transform(F functor) {
        using R = decltype(std::declval<F>()(std::declval<T>()...));
        return valid()
             ? Result<R>{std::apply(functor, value())}
             : Result<R>{error()};
    }

    auto value_or(Value or_default) const {
        return valid() ? value() : or_default;
    }

private:
    bool valid() const {
        return std::holds_alternative<Value>(*this);
    }

    Value& value() {
        return std::get<Value>(*this);
    }

    Value const& value() const {
        return std::get<Value>(*this);
    }

    Error const& error() const {
        return std::get<Error>(*this);
    }
};


int main() {
    auto report = [](auto const& result) {
        std::cout << (result ? "success" : "failure") << '\n';
    };
    auto print = [](auto x) {
        std::cout << x << '\n';
    };
    auto addition = [](auto... args) {
        return (args + ...);
    };

    Result<int>  i1{22};
    Result<int>  i2{37};
    Result<long> i3{41};
    report((i1 && i2 && i3).transform(addition).apply(print));

    Result<double> d1{Error{"error"}};
    Result<double> d2{3.14};
    report(d1 && d2);
    report(d1 || d2);
    d2.apply([](auto&& x) { x *= 2; }).apply(print);

    // More enable_if...
    // Andrei Alexandrescu's "Expect the Unexpected"
    Result<Error> woops;

    std::cout << std::endl;
    return 0;
}
