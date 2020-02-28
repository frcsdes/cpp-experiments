#include <chrono>
#include <iostream>
#include <list>
#include <vector>


template<class Unit>
struct chrono_strings;

template<>
struct chrono_strings<std::chrono::milliseconds> {
    static auto constexpr symbol = "ms";
};


template<
    class Unit  = std::chrono::milliseconds,
    class Clock = std::chrono::steady_clock
>
class Timer {
public:
    Timer()
        : start_{Clock::now()} {}

    ~Timer() {
        auto end = Clock::now();
        auto elapsed = std::chrono::duration_cast<Unit>(end - start_).count();
        std::cout << elapsed << chrono_strings<Unit>::symbol << '\n';
    }

private:
    typename Clock::time_point const start_;
};


int main() {
    // A little RAII timer
    {
        Timer timer;
        std::cout << "List: ";
        std::list<char> big(10e5, 'a');
    }
    {
        Timer timer;
        std::cout << "Vector: ";
        std::vector<char> big(10e5, 'a');
    }
    return 0;
}
