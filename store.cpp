#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <type_traits>
#include <vector>


template<class T, class TOut = T, class TIn = T>
class Variable {
    using Functor = std::function<void(TOut)>;

public:
    Variable() = default;
    explicit Variable(TIn value)
        : value_{std::forward<TIn>(value)} {}

    Variable& operator=(TIn value) {
        update(std::forward<TIn>(value));
        return *this;
    }

    TOut operator*() const {
        return value_;
    }

    void watch(Functor functor) {
        watchers_.push_back(std::move(functor));
    }

private:
    T value_{};
    std::vector<Functor> watchers_;
    std::mutex mutex_;
    bool updating_{false};

    void update(TIn value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!updating_) {
            updating_ = true;
            for (auto const& watcher : watchers_)
                watcher(value);
            value_ = value;
            updating_ = false;
        }
    }
};


namespace store {
    Variable<int> loop;
    Variable<std::string, std::string const&> type("T");
}


int main() {
    store::loop.watch([](auto) {
        std::cout << "Can I loop? ";
        store::loop = *store::loop + 1;
    });
    store::loop = 0;
    std::cout << "Apparently not." << std::endl;

    store::type.watch([](auto const& type) {
        std::cout
            << "A " << *store::type << " variable "
            << "can be watched as a " << type << ' '
            << "before its value changes." << std::endl;
    });
    store::type = "T const&";

    return 0;
}
