#include "reed_muller_decoder.h"
#include <cmath>
#include <algorithm>

std::vector<BooleanFunction> ReedMullerDecoder::decode(const BooleanFunction& received, int s, double epsilon) {
    int m = received.number_of_variables();
    if (s < 0 || s > m) {
        throw std::invalid_argument("Degree s must satisfy 0 <= s <= m");
    }
    return list_decode_recursive(received, s, epsilon, m);
}

std::vector<BooleanFunction> ReedMullerDecoder::list_decode_recursive(const BooleanFunction& f, int s, double epsilon, int m) {
    int n = 1 << m;
    double delta_s = pow(2.0, -s);
    int radius = compute_radius(n, delta_s, epsilon);

    // Base cases
    if (s == 0) {
        // RM(0, m) consists of the all-0 and all-1 functions
        std::vector<BooleanFunction> result;
        BooleanFunction zero(m);
        if (f.dist(zero) <= radius) {
            result.push_back(zero);
        }
        BooleanFunction one(m);
        for (uint32_t x = 0; x < (1 << m); x++) {
            one.set_value(x, true);
        }
        if (f.dist(one) <= radius) {
            result.push_back(one);
        }
        return result;
    }
    
    if (s == m) {
        // RM(m, m) consists of all Boolean functions in m variables
        // Just return the received function if it's within the radius
        std::vector<BooleanFunction> result;
        if (f.dist(f) <= radius) {
            result.push_back(f);
        }
        return result;
    }
    
    // Recursive case: 0 < s < m
    BooleanFunction fp = f.restrict(m, 0);
    BooleanFunction fpp = f.restrict(m, 1);
    BooleanFunction f_sum = fp + fpp;
    
    // Step 1: Decode f_sum with s-1 and 2*epsilon
    std::vector<BooleanFunction> L_sum = list_decode_recursive(f_sum, s-1, 2*epsilon, m-1);
    
    // Step 2: Decode fp and fpp with s and epsilon
    std::vector<BooleanFunction> L0 = list_decode_recursive(fp, s, epsilon, m-1);
    std::vector<BooleanFunction> L1 = list_decode_recursive(fpp, s, epsilon, m-1);
    
    // Step 3: Combine results using Plotkin construction
    std::vector<BooleanFunction> result;
    
    // First type: (u0, u0 + v)
    for (const auto& u0 : L0) {
        for (const auto& v : L_sum) {
            BooleanFunction candidate = u0.concate(u0 + v);
            if (f.dist(candidate) <= radius) {
                result.push_back(candidate);
            }
        }
    }
    
    // Second type: (u1 + v, u1)
    for (const auto& u1 : L1) {
        for (const auto& v : L_sum) {
            BooleanFunction candidate = (u1 + v).concate(u1);
            if (f.dist(candidate) <= radius) {
                result.push_back(candidate);
            }
        }
    }
    
    // Remove duplicates
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    
    return result;
}

int ReedMullerDecoder::compute_radius(int n, double delta_s, double epsilon) {
    return static_cast<int>(n * (delta_s - epsilon));
}