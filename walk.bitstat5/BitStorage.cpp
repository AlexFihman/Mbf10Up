#include "BitStorage.h"
#include <immintrin.h> // For AVX-512 intrinsics

BitStorage::BitStorage(size_t bitSize)
    : storage((bitSize + 63) / 64, 0) {}

BitStorage BitStorage::clone() const
{
    BitStorage copy(storage.size() * 64);
    copy.storage = storage;
    return copy;
}

void BitStorage::setBit(size_t index)
{
    storage[index / 64] |= (1ULL << (index % 64));
}

void BitStorage::clearBit(size_t index)
{
    storage[index / 64] &= ~(1ULL << (index % 64));
}

void BitStorage::toggleBit(size_t index)
{
    storage[index / 64] ^= (1ULL << (index % 64));
}

bool BitStorage::getBit(size_t index) const
{
    return (storage[index / 64] & (1ULL << (index % 64))) != 0;
}

void BitStorage::bitwiseAnd(const BitStorage &other)
{
    if (storage.size() != other.storage.size())
    {
        throw std::invalid_argument("BitStorage objects must have the same size for bitwise AND.");
    }

    size_t numChunks = storage.size() / 8; // Number of 512-bit chunks (8 * 64-bit words = 512 bits)
    size_t remainder = storage.size() % 8; // Remaining elements after 512-bit chunks

    uint64_t *thisData = storage.data();
    const uint64_t *otherData = other.storage.data();

    // Perform the AVX-512 bitwise AND on 512-bit chunks
    for (size_t i = 0; i < numChunks; ++i)
    {
        __m512i a = _mm512_loadu_si512(&thisData[i * 8]);  // Load 512 bits from this object
        __m512i b = _mm512_loadu_si512(&otherData[i * 8]); // Load 512 bits from the other object
        __m512i result = _mm512_and_si512(a, b);           // Perform bitwise AND
        _mm512_storeu_si512(&thisData[i * 8], result);     // Store the result back
    }

    // Handle the remaining elements that don't fit into a 512-bit chunk
    for (size_t i = numChunks * 8; i < storage.size(); ++i)
    {
        thisData[i] &= otherData[i];
    }
}

uint64_t *BitStorage::data()
{
    return storage.data();
}

size_t BitStorage::size() const
{
    return storage.size();
}

size_t BitStorage::countBitsSet() const
{
    size_t count = 0;

    size_t numChunks = storage.size() / 8; // Number of 512-bit chunks
    size_t remainder = storage.size() % 8; // Remaining elements after 512-bit chunks

    const uint64_t *thisData = storage.data();

    // Use AVX-512 to count bits in 512-bit chunks
    for (size_t i = 0; i < numChunks; ++i)
    {
        __m512i chunk = _mm512_loadu_si512(&thisData[i * 8]);

        // Sum the population counts of each 64-bit element
        __m512i popcnt = _mm512_popcnt_epi64(chunk);

        // Add the result to the total count
        count += _mm512_reduce_add_epi64(popcnt);
    }

    // Handle the remaining elements that don't fit into a 512-bit chunk
    for (size_t i = numChunks * 8; i < storage.size(); ++i)
    {
        count += __builtin_popcountll(thisData[i]);
    }

    return count;
}
