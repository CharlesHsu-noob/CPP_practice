#include <iostream>
#include <chrono>
#include <iomanip>

double nornalWayPolynomial(const double coff[], const int capacity, const double x) {
    double ans = 0;
    double temp_x = 1;
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < i; j++) {
            temp_x *= x;
        }
        ans += temp_x * coff[i];
        temp_x = 1;
    }
    return ans;
}

template <typename T> T honorWayPolynomial(const T coff[], const int capacity, const T x) {
    T ans = coff[capacity - 1];
    for (int i = capacity - 2; i > (-1); i--) {
        ans *= x;
        ans += coff[i];
    }
    return ans;
}

int main() {
    int capacity = 6;
    double coff[] = {46, 55, 95, 54, 60, 41};
    double x = 15;
    
    const int iterations = 1000*1000*1000; // 1 billion 

    std::cout << std::fixed << std::setprecision(4);

    auto start_normal = std::chrono::steady_clock::now();
    
    double dummy_sum_normal = 0; // 用來防止編譯器優化掉迴圈
    for (int i = 0; i < iterations; i++) {
        dummy_sum_normal += nornalWayPolynomial(coff, capacity, x);
    }
    
    auto end_normal = std::chrono::steady_clock::now();
    
    // 計算總時間 (奈秒)
    auto total_ns_normal = std::chrono::duration_cast<std::chrono::nanoseconds>(end_normal - start_normal).count();
    // 計算單次平均時間
    double avg_ns_normal = static_cast<double>(total_ns_normal) / iterations;

    std::cout << "Normal way:\n";
    std::cout << "Total cost time: " << total_ns_normal << " ns\n";
    std::cout << "Avg cost time: " << avg_ns_normal << " ns\n\n";

    auto start_honor = std::chrono::steady_clock::now();
    
    double dummy_sum_honor = 0;
    for (int i = 0; i < iterations; i++) {
        dummy_sum_honor += honorWayPolynomial(coff, capacity, x);
    }
    
    auto end_honor = std::chrono::steady_clock::now();
    
    auto total_ns_honor = std::chrono::duration_cast<std::chrono::nanoseconds>(end_honor - start_honor).count();
    double avg_ns_honor = static_cast<double>(total_ns_honor) / iterations;

    std::cout << "Honor way:\n";
    std::cout << "Total cost time: " << total_ns_honor << " ns\n";
    std::cout << "Avg cost time: " << avg_ns_honor << " ns\n\n";

    // 印出 dummy sum 確保編譯器不會認為計算是無效的
    // (通常隨便印一下或是輸出到檔案即可)
    if (dummy_sum_normal == dummy_sum_honor) {
        std::cout << "(Verification: Both methods produce the same result)\n";
    }

    return 0;
}