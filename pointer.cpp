#include <iostream>
#include <memory>
#include <type_traits>

template<template<class> class Pointer, class Resource>
concept OwningPointer =
requires (Pointer<Resource> const pointer) {
    { pointer.get() } -> Resource*;
};

template<class T>
class Ptr {
    using LRef = std::add_lvalue_reference_t<T>;

public:
    constexpr Ptr() noexcept : pointer_{nullptr} {}
    constexpr Ptr(std::nullptr_t) noexcept : Ptr{} {}

    template<class U>
    Ptr(Ptr<U> other) noexcept : Ptr{other.get()} {}

    template<template<class> class Smart, class U>
    requires OwningPointer<Smart, U>
    Ptr(Smart<U> const& smart) noexcept : Ptr{smart.get()} {}

    template<template<class> class Smart, class U>
    requires OwningPointer<Smart, U>
    Ptr(Smart<U>&&) = delete;

    T* get() const noexcept { return pointer_; }
    T* operator->() const noexcept { return pointer_; }
    LRef operator*() const { return *pointer_; }

    explicit operator bool() const noexcept { return pointer_; }

private:
    template<class U>
    explicit Ptr(U* raw) noexcept : pointer_{raw} {}

    T* pointer_;
};


struct Data {
    ~Data() { std::cout << "Destructor" << '\n'; }
};


int main() {
    // Only works if your compiler implements P0522R0 (GCC does)
    std::cout << "Scope begin" << '\n';

    auto unique = std::make_unique<Data>();
    Ptr<Data> dumb1{unique};

    auto shared = std::make_shared<Data>();
    Ptr<Data> dumb2{shared};

    // No thanks, I'm too dumb to assume ownership
    // Pointer<Data> dumb3(std::move(unique));
    // Pointer<Data> dumb4(std::make_shared<Data>());

    std::cout << "Scope end" << std::endl;
    return 0;
}
