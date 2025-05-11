#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "reed_muller_decoder.h"
#include "boolean_function.h"

BooleanFunction generate_random_function(int m, double error_rate) {
    BooleanFunction bf(m);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution d(error_rate);

    for (uint32_t x = 0; x < (1 << m); x++) {
        bf.set_value(x, d(gen));
    }
    return bf;
}

// no error
void test_perfect_recovery(int m, int s, double epsilon, std::ostream& os) {
    BooleanFunction original(m);
    // generate original codeword (example: all 0)
    for (uint32_t x = 0; x < (1 << m); x++) {
        original.set_value(x, false);
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto candidates = ReedMullerDecoder::decode(original, s, epsilon);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

    os << "Test (m=" << m << ", s=" << s << ", ε=" << epsilon << "): Perfect recovery\n";
    os << "Candidates: " << candidates.size() 
       << " | Time: " << duration.count() << "ms\n";
    os << "Original in candidates: " << (std::find(candidates.begin(), candidates.end(), original) != candidates.end() ? "Yes" : "No") 
       << "\n\n";
}

// random error
void test_random_errors(int m, int s, double epsilon, int num_errors, std::ostream& os) {
    BooleanFunction original(m);
    // generate original codeword (example: all 0)
    for (uint32_t x = 0; x < (1 << m); x++) original.set_value(x, false);
    
    // introduce random errors
    BooleanFunction received = original;
    std::set<uint32_t> error_positions;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<uint32_t> dist(0, (1 << m) - 1);
    
    while (error_positions.size() < num_errors) {
        uint32_t pos = dist(gen);
        if (!error_positions.count(pos)) {
            received.set_value(pos, !received.eval(pos));
            error_positions.insert(pos);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto candidates = ReedMullerDecoder::decode(received, s, epsilon);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);

    os << "Test (m=" << m << ", s=" << s << ", ε=" << epsilon << "): " << num_errors << " errors\n";
    os << "Candidates: " << candidates.size() 
       << " | Min distance: " << [&](){
           int min_dist = (1 << m);
           for (const auto& c : candidates) min_dist = std::min(min_dist, received.dist(c));
           return min_dist;
       }()
       << " | Time: " << duration.count() << "ms\n\n";
}

void test_boundary_cases(int m, double epsilon) {
    std::ofstream fout("boundary_tests.txt");
    
    // s=0
    BooleanFunction all_zero(m);
    auto result_s0 = ReedMullerDecoder::decode(all_zero, 0, epsilon);
    fout << "s=0 case: " << result_s0.size() << " candidates (expected 1 or 2)\n";
    
    // s=m
    BooleanFunction random_func = generate_random_function(m, 0.5);
    auto result_sm = ReedMullerDecoder::decode(random_func, m, epsilon);
    fout << "s=m case: " << result_sm.size() << " candidates (expected 1 if within radius)\n";
}

int main() {
    std::ofstream report("decoder_performance_report.txt");
    
    // basic test
    test_perfect_recovery(3, 1, 0.1, report);
    test_perfect_recovery(4, 2, 0.05, report);
    
    // error test
    test_random_errors(3, 1, 0.1, 1, report);  // 1 error (within radius)
    test_random_errors(4, 2, 0.05, 5, report); // 5 errors
    test_random_errors(5, 3, 0.02, 10, report); // 10 errors
    test_random_errors(6, 4, 0.01, 11, report); // 15 errors
    test_random_errors(7, 5, 0.005, 13, report); // 20 errors
    test_random_errors(8, 6, 0.002, 14, report); // 25 errors
    test_random_errors(9, 7, 0.001, 15, report); // 30 errors
   
    // boundary test
    test_boundary_cases(3, 0.1);
    
    return 0;
}