#include <type_traits>


template<class T, class... B>
inline bool constexpr has_base_among = (std::is_base_of_v<B, T> || ...);

template<class T, bool LastValue, class LastType, class... Rest>
struct base_among_test_result {};

template<class T, class B, class... Rest>
struct base_among_test_maker
    : base_among_test_result<T, std::is_base_of_v<B, T>, B, Rest...> {};

template<class T, class LastType, class... Rest>
struct base_among_test_result<T, true, LastType, Rest...> {
    using type = LastType;
};

template<class T, class LastType, class... Rest>
struct base_among_test_result<T, false, LastType, Rest...>
    : base_among_test_maker<T, Rest...> {};

template<class T, class... B>
using base_among = typename base_among_test_maker<T, B...>::type;


struct B1 {};
struct B2 {};
struct D1 : B1 {};
struct D2 : B1, B2 {};
struct Z {};


int main() {
    // Straightforward for single inheritance
    static_assert(           has_base_among<D1, Z, B1, B2>);
    static_assert(std::is_same_v<base_among<D1, Z, B1, B2>, B1>);

    // But order-dependent for multiple inheritance...
    static_assert(           has_base_among<D2, Z, B1, B2>);
    static_assert(           has_base_among<D2, Z, B2, B1>);
    static_assert(std::is_same_v<base_among<D2, Z, B1, B1>, B1>);
    static_assert(std::is_same_v<base_among<D2, Z, B2, B1>, B2>);

    return 0;
}
