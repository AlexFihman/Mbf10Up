#pragma once

#include <random> // For std::mt19937

class ShortList
{
private:
    static const int MAX_SIZE = 2049; // Maximum size of the list
    int arr[MAX_SIZE];                // Array to store integers
    bool containsArr[MAX_SIZE];       // Array to check contains
    int indexArr[MAX_SIZE];           // Array to store the index of each integer
    int size;                         // Current size of the list

public:
    ShortList();

    bool insert(int num);

    bool remove(int num);

    bool contains(int num) const;

    int getRandomElement(std::mt19937 &rng) const;

    int getSize() const;

    int getValue(int pos) const;

    void print() const;
};
