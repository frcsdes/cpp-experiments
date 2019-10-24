#include <iostream>
#include <memory>


template<class T>
class Pointer {
public:
    Pointer() : pointer_(nullptr) {}

    template<class U>
    Pointer(Pointer<U> other) : Pointer(other.operator->()) {}

    template<template<class> class Smart, class U>
    Pointer(Smart<U> const& smart) : Pointer(smart.get()) {}

    template<template<class> class Smart, class U>
    Pointer(Smart<U>&&) = delete;

    T* operator->() const { return pointer_; }
    T& operator*() const { return *pointer_; }

    explicit operator bool() const { return pointer_; }

private:
    template<class U>
    explicit Pointer(U* raw) : pointer_(raw) {}

    T* pointer_;
};


struct Data {
    ~Data() { std::cout << "Destructor" << '\n'; }
};


int main() {
    // Only works if your compiler implements P0522R0 (GCC does)
    std::cout << "Scope begin" << '\n';

    auto unique = std::make_unique<Data>();
    Pointer<Data> dumb1(unique);

    auto shared = std::make_shared<Data>();
    Pointer<Data> dumb2(shared);

    // No thanks, I'm too dumb to assume ownership
    // Pointer<Data> dumb3(std::move(unique));
    // Pointer<Data> dumb4(std::make_shared<Data>());

    std::cout << "Scope end" << std::endl;
    return 0;
}
