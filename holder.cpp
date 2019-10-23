#include <memory>
#include <tuple>


template<template<class> class Pointer, class... T>
class Holder {
public:
    explicit Holder(Pointer<T>... resources)
        : resources_{std::move(resources)...} {}

private:
    std::tuple<Pointer<T>...> resources_;
};


int main() {
    // Template template parameters ignored default values until C++17
    // - unique_ptr has 1 required and 1 optional template arguments
    // - shared_ptr has 1 required template argument
    // CLion's live code analysis still gets it wrong though (2019.2.4)

    Holder<std::unique_ptr, int, float, double>
    unique_holder(std::make_unique<int>(1),
                  std::make_unique<float>(2.f),
                  std::make_unique<double>(3.0));

    Holder<std::shared_ptr, int, float, double>
    shared_holder(std::make_shared<int>(1),
                  std::make_shared<float>(2.f),
                  std::make_shared<double>(3.0));

    return 0;
}
