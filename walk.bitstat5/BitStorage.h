#ifndef BITSTORAGE_H
#define BITSTORAGE_H

#include <vector>
#include <cstdint>
#include <stdexcept>
#include <immintrin.h> // For AVX-512 intrinsics


#ifdef __AVX512VPOPCNTDQ__
#define popcount512_64 _mm512_popcnt_epi64
#else
static inline __m512i popcount512_64(__m512i v) {
    alignas(64) u_int64_t vv[8];
    _mm512_store_epi64(static_cast<void*>(vv), v);

    for(int i = 0; i < 8; i++) {
        vv[i] = __builtin_popcountll(vv[i]);
    }

    return _mm512_load_epi64(static_cast<const void*>(vv));
}
#endif



class BitStorage
{
public:
    // Constructor to initialize storage with a given number of bits
    BitStorage(size_t bitSize);
    ~BitStorage();

    BitStorage(const BitStorage&) = delete;
    BitStorage& operator=(const BitStorage&) = delete;
    BitStorage(BitStorage&&) noexcept;
    BitStorage& operator=(BitStorage&&) noexcept;

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
    alignas(64) uint64_t* storage; // Stores bits in 64-bit chunks
    size_t storage_size;
};

#endif // BITSTORAGE_H
