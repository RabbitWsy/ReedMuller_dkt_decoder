#ifndef BOOLEAN_FUNCTION_H
#define BOOLEAN_FUNCTION_H

#include <vector>
#include <set>
#include <cstdint>

class BooleanFunction {
public:
    // Constructor with number of variables
    BooleanFunction(int var_num);
    
    // Copy constructor
    BooleanFunction(const BooleanFunction& other);
    
    // Destructor
    ~BooleanFunction();
    
    // Restrict a variable to a value
    BooleanFunction restrict(int idx, int val) const;
    
    // Evaluate the function at point x
    bool eval(uint32_t x) const;
    
    // Get the degree of the function
    int deg() const;
    
    // Get number of variables
    int number_of_variables() const;
    
    // Hamming distance between two functions
    int dist(const BooleanFunction& other) const;
    
    // Concatenate two functions
    BooleanFunction concate(const BooleanFunction& other) const;
    
    // Operator overloading
    BooleanFunction operator+(const BooleanFunction& other) const;
    BooleanFunction& operator=(const BooleanFunction& other);
    bool operator==(const BooleanFunction& other) const;
    bool operator<(const BooleanFunction& other) const;
    
    // Set truth table value
    void set_value(uint32_t x, bool val);
    
private:
    int var_num;
    uint32_t* truth_table;
    uint32_t table_size;
    
    // Helper function to compute degree
    int compute_degree() const;
};

#endif // BOOLEAN_FUNCTION_H