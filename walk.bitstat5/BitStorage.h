#ifndef BITSTORAGE_H
#define BITSTORAGE_H

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <immintrin.h> // For AVX-512 intrinsics
class BitStorage
{
public:
    // Constructor to initialize storage with a given number of bits
    BitStorage(size_t bitSize);

    // Clone method to create a deep copy of the object
    BitStorage clone() const;

    // Set a specific bit to 1
    void setBit(size_t index);

    // Clear a specific bit to 0
    void clearBit(size_t index);

    // Toggle a specific bit
    void toggleBit(size_t index);

    // Get the value of a specific bit (0 or 1)
    bool getBit(size_t index) const;

    __m512i getChunk(size_t index) const;

    // Bitwise AND operation with another BitStorage
    void bitwiseAnd(const BitStorage &other);

    size_t countBitsSet() const;
    // Get a pointer to the underlying data for AVX-512 operations
    uint64_t *data();

    // Get the size of the data in 64-bit words
    size_t size() const;

private:
    std::vector<uint64_t> storage; // Stores bits in 64-bit chunks
};

#endif // BITSTORAGE_H
