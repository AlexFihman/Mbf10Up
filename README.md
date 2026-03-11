# MbfUp — Probabilistic Dedekind Number Estimator

A multi-threaded C++ tool that uses Monte Carlo sampling over monotone Boolean functions (MBFs) to probabilistically estimate **Dedekind numbers**.

## Background

The *n*-th Dedekind number D(n) counts the number of monotone Boolean functions on *n* variables — equivalently, the number of antichains in the power set lattice of an *n*-element set. These numbers grow doubly-exponentially and are notoriously hard to compute exactly. Only D(0)–D(9) are known exactly; D(9) was first confirmed in 2023.

This code estimates D(n) probabilistically by:

1. Splitting the Boolean lattice at the middle level `c2 = n/2`.
2. Randomly sampling subsets of elements at that level (using Mersenne Twister).
3. For each sample, independently counting MBFs consistent with the lower and upper halves of the lattice via `SumOfMbf`.
4. Combining the two half-counts with binomial coefficients `C[c3][i]` to form an unbiased estimator of D(n).
5. Writing raw `(seed, cycle, estimate)` triples to binary output files for offline aggregation.

## Configuration

| Setting | Location | Default | Notes |
|---|---|---|---|
| Dimension `n` | `mbf16c.h` line 10 | `11` | Valid range: 2–11 (limited by `long double` precision) |
| Samples per thread | `mbfup.cpp` line 16 | `1000` | `const int loops` |
| Threads per process | `mbfup.cpp` line 86 | `128` | `int nthread` |

## Requirements

- Linux x86-64
- g++ 9.4 or later (uses C++11 threads, `long double`)
- POSIX filesystem (output written to `./data/`)

## Build

```bash
make
```

This compiles all translation units with `-O3 -march=native -pthread` and links the `mbfup` binary.

To clean build artifacts:

```bash
make clean
```

## Running

Create the output directory first, then run the binary with a starting seed:

```bash
mkdir -p data
./mbfup <seed>
```

Each invocation spawns 128 threads using seeds `[seed, seed+127]`. Each thread writes one binary file:

```
./data/experiment<seed>.dat
```

Each record in the file is:
```
int seed | int cycle | long double sum
```
(4 + 4 + 16 = 24 bytes per record, `loops` records per file)

### Running many independent batches

`r.sh` automates running many non-overlapping seed ranges sequentially:

```bash
bash r.sh
```

This calls `./mbfup` with starting seeds 1, 129, 257, 385, … (each batch uses 128 seeds), producing a large pool of independent samples that can be averaged to get a precise estimate of D(n).

## Source Files

| File | Purpose |
|---|---|
| `mbf16c.h` / `mbf16c.cpp` | Precomputes the Boolean lattice structure: `ties[][]` (neighbors), `levelofitem[]` (Hamming weight), `levellist[][]` / `leveldepth[]` (level buckets), plus lookup tables for factorials, powers of 2, and Pascal's triangle `C[][]` |
| `fastlist.h` / `fastlist.cpp` | `TFastList` — an O(1) add / delete / random-pick data structure backed by a flat array + index map |
| `uMbfObj.h` / `uMbfObj.cpp` | `TMbfObj` — tracks a partially-constructed MBF (items partitioned into `InList`/`OutList` per level); implements `AddItem`, `AddRandItem`, `AddRandItem2`, `ClearLevel`. Also contains the key recursive function `SumOfMbf` which counts MBFs consistent with the current state |
| `mbfup.cpp` | Entry point: initializes lattice data, spawns worker threads, collects timing |
| `time_ms.h` / `time_ms.cpp` | Wall-clock timer in milliseconds |
| `Makefile` | Build rules |
| `r.sh` | Shell script to run many seed batches |
| `print.sh` | Helper script for inspecting output |

## License

MIT — see [LICENSE](LICENSE).
Copyright (c) 2023 AlexFihman
