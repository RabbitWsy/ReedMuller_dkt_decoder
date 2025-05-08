# Reed-Muller Decoder Design Document

## 1. Core Components

### BooleanFunction Class
- **Purpose**: Represents Boolean functions with truth table storage and operations
- **Key Methods**:
  - `restrict()`: Variable restriction (f|xi=b)
  - `eval()`: Function evaluation at a point
  - `concate()`: Concatenates two functions
  - `dist()`: Hamming distance calculation
  - `operator+`: GF(2) addition
- **Data Structure**: 
  - Compressed truth table (bit array)
  - Variable count tracking

### ReedMullerDecoder Class
- **Purpose**: Implements DKT list decoding algorithm
- **Key Methods**:
  - `decode()`: Main decoding interface
  - `list_decode_recursive()`: Recursive implementation
  - `compute_radius()`: Decoding radius calculation

## 2. Algorithm Implementation

### Recursive Structure
1. **Base Cases**:
   - `s=0`: Returns constant 0/1 functions
   - `s=m`: Returns input function (practical limitation)

2. **Recursive Case**:
   - Decomposes into three subproblems:
     - `f0 = f|xi=0`
     - `f1 = f|xi=1` 
     - `f_sum = f0 + f1`
   - Recursively decodes with:
     - `f_sum` at degree s-1
     - `f0/f1` at degree s

3. **Result Combination**:
   - Uses Plotkin construction:
     - Type A: (u0, u0 + v)
     - Type B: (u1 + v, u1)
   - Filters candidates by radius check

### Critical Parameters
- Decoding radius: `radius = n*(2^{-s} - ε)`
- Subproblem radii adjusted for recursion:
  - `sub_radius = n/2*(2^{-s} - ε)`
  - `sub_radius_sum = n/2*(2^{-(s-1)} - 2ε)`

## 3. Interface Specification

```cpp
std::vector<BooleanFunction> decode(
    const BooleanFunction& received,
    int s,          // Target degree 
    double epsilon  // Radius parameter
);
```

**Preconditions**:
- 0 ≤ s ≤ m (m = number of variables)
- ε < 2^{-s}
- m ≤ 30 (practical limitation)

