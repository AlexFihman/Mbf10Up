#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <iomanip>
#include <chrono>
#include "ShortList.h"
#include "MonotoneBooleanFunction.h"
#include "BitStorage.h"

bool cmp(uint64_t x1, uint64_t x2)
{
    return (x1 & x2) == x2;
}

bool cmpRecord(Record &r1, Record &r2)
{
    for (int i = 0; i < 8; i++)
    {
        if (!cmp(r1.data[i], r2.data[i]))
            return false;
    }
    return true;
}

int64_t time_ms()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int main()
{
    std::random_device rd;
    std::mt19937 rng(rd());
    const int dim = 9;

    int LIST_SIZE = 512 * 100;
    std::vector<MonotoneBooleanFunction> mbfList;
    mbfList.reserve(LIST_SIZE * 2);

    for (int i = 0; i < LIST_SIZE * 2; ++i)
    {
        mbfList.emplace_back(dim, rng);
    }

    for (int ls = 0; ls < LIST_SIZE * 2; ls++)
    {
        for (int i = 0; i < 12000; i++)
        {
            mbfList[ls].flipRandom();
        }
    }

    std::vector<BitStorage> bitStorage;
    bitStorage.reserve(512);

    for (int i = 0; i < 512; ++i)
    {
        bitStorage.emplace_back(LIST_SIZE);
    }

    for (int loop = 0; loop < 1000; loop++)
    {

        int64_t t0 = time_ms();
        for (int ls = 0; ls < LIST_SIZE * 2; ls++)
        {
            mbfList[ls].step();
        }

        // compare two buffers in the old way
        int64_t t1 = time_ms();
        Record *buf1 = new Record[LIST_SIZE];
        Record *buf2 = new Record[LIST_SIZE];

        for (int ls = 0; ls < LIST_SIZE; ls++)
        {
            mbfList[ls].toRecord(buf1[ls]);
            mbfList[ls + LIST_SIZE].toRecord(buf2[ls]);
        }

        int total1 = 0;
        for (int i = 0; i < LIST_SIZE; i++)
        {
            for (int j = 0; j < LIST_SIZE; j++)
                if (cmpRecord(buf2[i], buf1[j]))
                    total1++;
        }

        // compare two buffers in the new way, using AVX-512
        int64_t t2 = time_ms();
        for (int ls = 0; ls < LIST_SIZE; ls++)
        {
            for (int j = 0; j < 512; j++)
            {
                if (mbfList[LIST_SIZE + ls].getFunctionValue(j))
                    bitStorage[j].setBit(ls);
                else
                    bitStorage[j].clearBit(ls);
            }
        }

        int total = 0;
        for (int ls = 0; ls < LIST_SIZE; ls++)
        {
            ShortList *sl = mbfList[ls].getMinCNF();
            if (sl->getSize() > 0)
            {
                BitStorage result = bitStorage[sl->getValue(0)].clone();
                for (int i = 1; i < sl->getSize(); i++)
                    result.bitwiseAnd(bitStorage[sl->getValue(i)]);
                total += result.countBitsSet();
            }
            delete sl;
        }

        int64_t t3 = time_ms();

        std::cout << "total: " << total1 << " " << total << " gen.time: " << (t1 - t0) << /*" cmp1 time: " << (t2 - t1) << */ " cmp2 time: " << (t3 - t2) << std::endl;
        // std::cout << loop << "\t" << total << "\t" << (t1 - t0) << "\t" << (t3 - t2) << std::endl;
    }

    return 0;
}
