#pragma once

#include <bitset>
#include "ShortList.h"

struct Record
{
    uint64_t data[8];
};

class MonotoneBooleanFunction
{
private:
    const int rsize = 8;
    const int size = 512;

    int dimension; // Dimension of the boolean function
    int weight;
    bool *functionArray; // Array to store the boolean function
    std::mt19937 &rng;   // Reference to Mersenne Twister random number generator
    ShortList min_cuts;  // ShortList to store the minimum cuts
    bool checkMinCut(int index) const;
    void updateMinCuts();
    void updateMinCutsFast(int index);

public:
    MonotoneBooleanFunction(int dim, std::mt19937 &rng);

    ~MonotoneBooleanFunction();

    bool getFunctionValue(int index) const;

    void setFunctionValue(int index, bool value);

    void flip(int index);

    void flipRandom();

    void step();

    int getRandomMinCut() const;

    void printMinCuts() const;

    int getWeight() const;

    int minCutSize() const;

    void toRecord(Record &r);

    ShortList *getMinCNF();

    static int recordHammingDistance(const Record &rec1, const Record &rec2);
};
