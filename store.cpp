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
        : m_value(std::forward<TIn>(value)) {}

    Variable& operator=(TIn value) {
        update(std::forward<TIn>(value));
        return *this;
    }

    TOut operator*() const {
        return m_value;
    }

    void watch(Functor functor) {
        m_watchers.push_back(std::move(functor));
    }

private:
    T m_value{};
    std::vector<Functor> m_watchers;
    std::mutex m_mutex;
    bool m_updating = false;

    void update(TIn value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_updating) {
            m_updating = true;
            for (auto const& w : m_watchers) w(value);
            m_value = value;
            m_updating = false;
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
