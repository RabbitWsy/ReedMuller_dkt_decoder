# ReedMuller_dkt_decoder
the implementation of the dkt algorithm from the paper "On complexity of decoding Reed-Muller codes within their code distance" by Dumer I, Kabatiansky G, Tavernier C, et al. which is available on [researchgate](https://www.researchgate.net/profile/Ilya-Dumer/publication/228999561_On_complexity_of_decoding_Reed-Muller_codes_within_their_code_distance/links/00b4953ccb0721a465000000/On-complexity-of-decoding-Reed-Muller-codes-within-their-code-distance.pdf)

## Usage
```bash
mkdir -p build && cd build
cmake ..
cmake --build .

# run tests
ctest -V

# install(default: /usr/local)
cmake --install . 

# install to a custom directory
cmake --install . --prefix ./install

```

you can add your own test cases in the tests directory , modify the `tests/CMakeLists.txt` file and run them with `ctest -V`. Additionaly, we have a `main_test.cpp` to generate a performance report saved in `decoder_performance_report.txt` and `boundary_tests.txt`.