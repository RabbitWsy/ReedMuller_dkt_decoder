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
  - ...
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
   - $s=0$: Returns constant $0/1$ functions
   - $s=m$: Returns input function (practical limitation)

2. **Recursive Case**:
   - Decomposes into three subproblems:
     - $f_0 = f\mid_{x_i=0}$
     - $f_1 = f\mid_{x_i=1}$
     - $f_{sum} = f_0 + f_1$
   - Recursively decodes with:
     - $f_{sum}$ at degree $s-1$
     - $f_0/f_1$ at degree $s$

3. **Result Combination**:
   - Uses Plotkin construction:
     - Type A: $(u_0, u_0 + v)$
     - Type B: $(u_1 + v, u_1)$
   - Filters candidates by radius check

### Critical Parameters
- Decoding radius: $\text{Radius} = n(2^{-s} - \epsilon)$
- Candidate filtering: $\text{Hamming distance} \leq \text{radius}$

## 3. Interface Specification

```cpp
std::vector<BooleanFunction> decode(
    const BooleanFunction& received,
    int s,          // Target degree 
    double epsilon  // Radius parameter
);
```

**Preconditions**:
- $0 \leq s \leq m$ (m = number of variables)
- $\epsilon < 2^{-s}$
- $m \leq 30$ (practical limitation)

