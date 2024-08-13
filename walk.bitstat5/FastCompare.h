
#include <vector>
#include <stdint.h>
#include "MonotoneBooleanFunction.h"
#include "BitStorage.h"
#include <immintrin.h> // For AVX-512 intrinsics
#include <iostream>
#include <chrono>

class FastCompare {
    std::vector<int16_t> leftIndices;
    std::vector<size_t> endPoints;

public:
    FastCompare(std::vector<MonotoneBooleanFunction>& mbfList, size_t list_size);

    template<size_t ChunkSize>
    void countCompareTo(std::vector<BitStorage>& bitStorage) const {
        if(BitStorage::ALLOC_SIZE_ALIGN % (ChunkSize*sizeof(uint64_t)) != 0) {throw "Invalid List Size";}

        auto start_at = std::chrono::high_resolution_clock::now();

        int total = 0;

        size_t storage_size = bitStorage[0].storage_size;
        for (size_t chunkStart = 0; chunkStart < storage_size; chunkStart += ChunkSize)
        {
            size_t curIndex = 0;
            for(size_t endPoint : endPoints) {
                #ifdef __AVX512F__
                if constexpr(ChunkSize >= 8) {
                    alignas(64) __m512i chunkTMP[ChunkSize / 8];

                    const BitStorage& firstBitset = bitStorage[leftIndices[curIndex]];
                    for(size_t i = 0; i < ChunkSize / 8; i++) {
                        chunkTMP[i] = _mm512_load_epi64(&firstBitset.storage[chunkStart + 8*i]);
                    }
                    while(++curIndex < endPoint) {
                        const BitStorage& selectedBitset = bitStorage[leftIndices[curIndex]];
                        for(size_t i = 0; i < ChunkSize / 8; i++) {
                            chunkTMP[i] = _mm512_and_si512(chunkTMP[i], _mm512_load_epi64(&selectedBitset.storage[chunkStart + 8*i]));
                        }
                    };

                    __m512i totals = popcount512_64(chunkTMP[0]);
                    for(size_t i = 1; i < ChunkSize / 8; i++) {
                        __m512i subTotal = popcount512_64(chunkTMP[i]);
                        totals = _mm512_add_epi64(totals, subTotal);
                    }
                    total += _mm512_reduce_add_epi64(totals);
                    continue;
                }
                #elif defined(__AVX2__)
                if constexpr(ChunkSize >= 4) {
                    alignas(64) __m256i chunkTMP[ChunkSize / 4];

                    const BitStorage& firstBitset = bitStorage[leftIndices[curIndex]];
                    for(size_t i = 0; i < ChunkSize / 4; i++) {
                        chunkTMP[i] = _mm256_load_si256((const __m256i*) &firstBitset.storage[chunkStart + 4*i]);
                    }
                    while(++curIndex < endPoint) {
                        const BitStorage& selectedBitset = bitStorage[leftIndices[curIndex]];
                        for(size_t i = 0; i < ChunkSize / 4; i++) {
                            chunkTMP[i] = _mm256_and_si256(chunkTMP[i], _mm256_load_si256((const __m256i*) &selectedBitset.storage[chunkStart + 4*i]));
                        }
                    };

                    alignas(64) uint64_t elems[4];
                    for(size_t i = 0; i < ChunkSize / 4; i++) {
                        _mm256_store_si256((__m256i*) elems, chunkTMP[i]);

                        for(uint64_t& elem : elems) {
                            total += __builtin_popcountll(elem);
                        }
                    }
                    continue;
                }
                #endif
                alignas(64) uint64_t chunkTMP[ChunkSize];

                const BitStorage& firstBitset = bitStorage[leftIndices[curIndex]];
                for(size_t i = 0; i < ChunkSize; i++) {
                    chunkTMP[i] = firstBitset.storage[chunkStart + i];
                }
                while(++curIndex < endPoint) {
                    const BitStorage& selectedBitset = bitStorage[leftIndices[curIndex]];
                    for(size_t i = 0; i < ChunkSize; i++) {
                        chunkTMP[i] &= selectedBitset.storage[chunkStart + i];
                    }
                };

                for(uint64_t& v : chunkTMP) {
                    total += __builtin_popcountll(v);
                }
            }
        }
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_at).count();
        std::cout << "ChunkSize: " << ChunkSize << "  total: " << total << "  time: " << millis << std::endl;
    }
};

