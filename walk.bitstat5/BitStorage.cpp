#include "BitStorage.h"
#include <immintrin.h> // For AVX-512 intrinsics
#include <string.h>
#include <assert.h>

size_t alignedByteSize(size_t bitSize) {
    return (bitSize + 511) / 512 * 64;
}

BitStorage::BitStorage(size_t bitSize)
    // Alloc 512 bit aligned blocks, aligned to 64 byte page size
    : storage((uint64_t*) aligned_alloc(64, alignedByteSize(bitSize))), storage_size(alignedByteSize(bitSize) / sizeof(uint64_t)) {

    }

BitStorage::~BitStorage() {
    if(this->storage) {
        free(this->storage);
    }
}

BitStorage::BitStorage(BitStorage&& other) noexcept : storage(other.storage), storage_size(other.storage_size) {
    other.storage = nullptr;
    other.storage_size = 0;
}
BitStorage& BitStorage::operator=(BitStorage&& other) noexcept {
    uint64_t* tmp = this->storage;
    this->storage = other.storage;
    other.storage = tmp;
    size_t tmpSize = this->storage_size;
    this->storage_size = other.storage_size;
    other.storage_size = tmpSize;
}

BitStorage BitStorage::clone() const
{
    BitStorage copy(this->storage_size * sizeof(uint64_t) * 8);
    memcpy(copy.storage, this->storage, this->storage_size * sizeof(uint64_t));
    return copy;
}

void BitStorage::setBit(size_t index)
{
    assert(index < 64*this->storage_size);
    storage[index / 64] |= (1ULL << (index % 64));
}

void BitStorage::clearBit(size_t index)
{
    assert(index < 64*this->storage_size);
    storage[index / 64] &= ~(1ULL << (index % 64));
}

void BitStorage::toggleBit(size_t index)
{
    assert(index < 64*this->storage_size);
    storage[index / 64] ^= (1ULL << (index % 64));
}

bool BitStorage::getBit(size_t index) const
{
    assert(index < 64*this->storage_size);
    return (storage[index / 64] & (1ULL << (index % 64))) != 0;
}

__m512i BitStorage::getChunk(size_t index) const
{
    assert(index < this->storage_size / 8);
    return _mm512_load_si512(&storage[index * 8]);
}

void BitStorage::bitwiseAnd(const BitStorage &other)
{
    assert(this->storage_size == other.storage_size);

#ifdef __AVX512F__
    size_t numChunks = this->storage_size / 8; // Number of 512-bit chunks (8 * 64-bit words = 512 bits)
    size_t remainder = this->storage_size % 8; // Remaining elements after 512-bit chunks

    // Perform the AVX-512 bitwise AND on 512-bit chunks
    for (size_t i = 0; i < numChunks; ++i)
    {
        __m512i a = _mm512_load_si512(&this->storage[i * 8]);  // Load 512 bits from this object
        __m512i b = _mm512_load_si512(&other.storage[i * 8]); // Load 512 bits from the other object
        __m512i result = _mm512_and_si512(a, b);           // Perform bitwise AND
        _mm512_store_si512(&this->storage[i * 8], result);     // Store the result back
    }

    // Handle the remaining elements that don't fit into a 512-bit chunk
    for (size_t i = numChunks * 8; i < this->storage_size; ++i)
    {
        this->storage[i] &= other.storage[i];
    }
#else
    // Handle the remaining elements that don't fit into a 512-bit chunk
    for (size_t i = 0; i < this->storage_size; ++i)
    {
        this->storage[i] &= other.storage[i];
    }
#endif
}

uint64_t *BitStorage::data()
{
    return this->storage;
}

size_t BitStorage::size() const
{
    return this->storage_size;
}

size_t countChunk(const uint64_t* storage, size_t numuint64_t) 
{
    size_t count = 0;

#ifdef __AVX512VPOPCNTDQ__
    size_t numChunks = numuint64_t / 8; // Number of 512-bit chunks
    size_t remainder = numuint64_t % 8; // Remaining elements after 512-bit chunks

    // Use AVX-512 to count bits in 512-bit chunks
    for (size_t i = 0; i < numChunks; ++i)
    {
        __m512i chunk = _mm512_load_si512(&storage[i * 8]);

        // Sum the population counts of each 64-bit element
        __m512i popcnt = popcount512_64(chunk);

        // Add the result to the total count
        count += _mm512_reduce_add_epi64(popcnt);
    }

    // Handle the remaining elements that don't fit into a 512-bit chunk
    for (size_t i = numChunks * 8; i < numuint64_t; ++i)
    {
        count += __builtin_popcountll(storage[i]);
    }
#else
    for (size_t i = 0; i < numuint64_t; ++i)
    {
        count += __builtin_popcountll(storage[i]);
    }
#endif

    return count;
}

size_t BitStorage::countBitsSet() const
{
    return countChunk(this->storage, this->storage_size);
}
