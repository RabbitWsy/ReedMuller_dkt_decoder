#include <iostream>
#include "reed_muller_decoder.h"
#include "boolean_function.h"

int main() {
    // Example usage
    int m = 3; // Number of variables
    int s = 1; // Degree of RM code
    
    // Create a received Boolean function (this would normally come from the channel)
    BooleanFunction received(m);
    // Set some values in the truth table (simulating a received codeword with errors)
    received.set_value(0b000, false);
    received.set_value(0b001, true);
    received.set_value(0b010, true);
    received.set_value(0b011, false);
    received.set_value(0b100, true);
    received.set_value(0b101, false);
    received.set_value(0b110, false);
    received.set_value(0b111, true);
    
    double epsilon = 0.1; // Radius parameter
    
    // Decode the received function
    std::vector<BooleanFunction> candidates = ReedMullerDecoder::decode(received, s, epsilon);
    
    // Output the results
    std::cout << "Found " << candidates.size() << " candidate codewords:\n";
    for (const auto& candidate : candidates) {
        std::cout << "Candidate: ";
        for (uint32_t x = 0; x < (1 << m); x++) {
            std::cout << candidate.eval(x);
        }
        std::cout << " (distance: " << received.dist(candidate) << ")\n";
    }
    
    return 0;
}