#ifndef REED_MULLER_DECODER_H
#define REED_MULLER_DECODER_H

#include "boolean_function.h"
#include <vector>

class ReedMullerDecoder {
public:
    // Decode a received vector using the DKT algorithm
    static std::vector<BooleanFunction> decode(const BooleanFunction& received, int s, double epsilon);
    
private:
    // Helper function for recursive decoding
    static std::vector<BooleanFunction> list_decode_recursive(const BooleanFunction& f, int s, double epsilon, int m);
    
    // Compute the decoding radius T(s, m, epsilon)
    static int compute_radius(int n, double delta_s, double epsilon);
};

#endif // REED_MULLER_DECODER_H