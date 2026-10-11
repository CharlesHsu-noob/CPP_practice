// https://godbolt.org/z/WMsWzETd7
#include <chrono>
#include <iostream>

template <typename Clock>
void print_resolution(const char* name) {
    long double ns_per_tick = (1'000'000'000 * Clock::period::num) / Clock::period::den;
    std::cout << name << " tick: ~" << ns_per_tick << " ns\n";
}

int main() {
    print_resolution<std::chrono::system_clock>("system_clock");
    print_resolution<std::chrono::steady_clock>("steady_clock");
    print_resolution<std::chrono::high_resolution_clock>("high_resolution_clock");
}