This code provides an implementation for the probabilistic computation of Dedekind numbers. The code is designed and tested on a 64-bit Linux system running on an x86-64 machine with g++ version 9.4.

The dimension of the computation is specified in the mbf16c.h file. It's worth noting that, due to the limitations of "long double" arithmetic, the dimension is restricted to a maximum value of 11.

Additionally, the number of loops for the computation is determined and configured in the mbfup.cpp file.

The code uses probabilistic techniques to estimate Dedekind numbers, which are combinatorial objects with known computational challenges due to their exponential growth.