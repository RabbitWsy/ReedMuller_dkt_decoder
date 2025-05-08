#include "boolean_function.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

BooleanFunction::BooleanFunction(int var_num) : var_num(var_num) {
    if (var_num < 0 || var_num > 30) {
        throw std::invalid_argument("Number of variables must be between 0 and 30");
    }
    table_size = 1 << var_num;
    truth_table = new uint32_t[(table_size + 31) / 32]();
}

BooleanFunction::BooleanFunction(const BooleanFunction& other) : var_num(other.var_num), table_size(other.table_size) {
    int array_size = (table_size + 31) / 32;
    truth_table = new uint32_t[array_size];
    std::copy(other.truth_table, other.truth_table + array_size, truth_table);
}

BooleanFunction::~BooleanFunction() {
    delete[] truth_table;
}

BooleanFunction BooleanFunction::restrict(int idx, int val) const {
    if (idx < 1 || idx > var_num) {
        throw std::invalid_argument("Variable index out of range");
    }
    
    BooleanFunction result(var_num - 1);
    int shift = idx - 1;
    uint32_t mask = ~(1 << shift);
    
    for (uint32_t x = 0; x < result.table_size; x++) {
        // Reconstruct the original input
        uint32_t upper = (x >> shift) << (shift + 1);
        uint32_t lower = x & ((1 << shift) - 1);
        uint32_t original_x = upper | lower | (val << shift);
        
        result.set_value(x, eval(original_x));
    }
    
    return result;
}

bool BooleanFunction::eval(uint32_t x) const {
    if (x >= table_size) {
        throw std::invalid_argument("Input out of range");
    }
    uint32_t word = truth_table[x / 32];
    return (word >> (x % 32)) & 1;
}

int BooleanFunction::deg() const {
    if (var_num == 0) return 0;
    return compute_degree();
}

int BooleanFunction::number_of_variables() const {
    return var_num;
}

int BooleanFunction::dist(const BooleanFunction& other) const {
    if (var_num != other.var_num) {
        throw std::invalid_argument("Functions must have same number of variables");
    }
    
    int distance = 0;
    for (uint32_t x = 0; x < table_size; x++) {
        if (eval(x) != other.eval(x)) {
            distance++;
        }
    }
    return distance;
}

BooleanFunction BooleanFunction::concate(const BooleanFunction& other) const {
    if (var_num != other.var_num) {
        throw std::invalid_argument("Functions must have same number of variables");
    }
    
    BooleanFunction result(var_num + 1);
    for (uint32_t x = 0; x < table_size; x++) {
        result.set_value(x, eval(x)); // f(x,0)
        result.set_value(x | (1 << var_num), other.eval(x)); // f(x,1)
    }
    return result;
}

BooleanFunction BooleanFunction::operator+(const BooleanFunction& other) const {
    if (var_num != other.var_num) {
        throw std::invalid_argument("Functions must have same number of variables");
    }
    
    BooleanFunction result(var_num);
    for (uint32_t x = 0; x < table_size; x++) {
        result.set_value(x, eval(x) != other.eval(x));
    }
    return result;
}

BooleanFunction& BooleanFunction::operator=(const BooleanFunction& other) {
    if (this != &other) {
        delete[] truth_table;
        var_num = other.var_num;
        table_size = other.table_size;
        int array_size = (table_size + 31) / 32;
        truth_table = new uint32_t[array_size];
        std::copy(other.truth_table, other.truth_table + array_size, truth_table);
    }
    return *this;
}

bool BooleanFunction::operator==(const BooleanFunction& other) const {
    if (var_num != other.var_num) return false;
    
    int array_size = (table_size + 31) / 32;
    for (int i = 0; i < array_size; i++) {
        if (truth_table[i] != other.truth_table[i]) {
            return false;
        }
    }
    return true;
}

bool BooleanFunction::operator<(const BooleanFunction& other) const {
    if (var_num != other.var_num) {
        return var_num < other.var_num;
    }
    
    int array_size = (table_size + 31) / 32;
    for (int i = 0; i < array_size; i++) {
        if (truth_table[i] != other.truth_table[i]) {
            return truth_table[i] < other.truth_table[i];
        }
    }
    return false;
}

void BooleanFunction::set_value(uint32_t x, bool val) {
    if (x >= table_size) {
        throw std::invalid_argument("Input out of range");
    }
    uint32_t mask = 1 << (x % 32);
    if (val) {
        truth_table[x / 32] |= mask;
    } else {
        truth_table[x / 32] &= ~mask;
    }
}

int BooleanFunction::compute_degree() const {
    // Using Fast Möbius Transform to compute algebraic degree
    std::vector<bool> f(table_size);
    for (uint32_t x = 0; x < table_size; x++) {
        f[x] = eval(x);
    }
    
    int degree = 0;
    for (int i = 0; i < var_num; i++) {
        int stride = 1 << i;
        for (int j = 0; j < table_size; j += 2 * stride) {
            for (int k = 0; k < stride; k++) {
                f[j + k + stride] = f[j + k] ^ f[j + k + stride];
            }
        }
    }
    
    for (uint32_t x = 0; x < table_size; x++) {
        if (f[x]) {
            int wt = __builtin_popcount(x);
            if (wt > degree) {
                degree = wt;
            }
        }
    }
    
    return degree;
}