#include "ShortList.h"
#include <algorithm> // For std::find
#include <iostream>
#include <assert.h>

ShortList::ShortList() : size(0)
{
    // Initialize containsArr and indexArr
    std::fill(containsArr, containsArr + MAX_SIZE, false);
    std::fill(indexArr, indexArr + MAX_SIZE, -1); // Invalid index
}

void ShortList::insert(int num)
{
    assert(size < MAX_SIZE);
    assert(!contains(num));
    
    arr[size] = num;
    containsArr[num] = true;
    indexArr[num] = size;
    ++size;
}

void ShortList::remove(int num)
{
    assert(contains(num));
    int idx = indexArr[num];
    arr[idx] = arr[--size];       // Replace the element with the last element and decrement size
    containsArr[arr[idx]] = true; // Update containsArr for the moved element
    containsArr[num] = false;
    indexArr[arr[idx]] = idx; // Update indexArr for the moved element
    indexArr[num] = -1;       // Invalidate the index of the removed element
}
bool ShortList::remove_if_exists(int num)
{
    if(contains(num)) {
        int idx = indexArr[num];
        arr[idx] = arr[--size];       // Replace the element with the last element and decrement size
        containsArr[arr[idx]] = true; // Update containsArr for the moved element
        containsArr[num] = false;
        indexArr[arr[idx]] = idx; // Update indexArr for the moved element
        indexArr[num] = -1;       // Invalidate the index of the removed element
        return true;
    }
    return false;
}

bool ShortList::contains(int num) const
{
    return num >= 0 && num < MAX_SIZE && containsArr[num];
}

int ShortList::getRandomElement(std::mt19937 &rng) const
{
    assert(size > 0);
    return arr[rng() % size];
}

int ShortList::getSize() const
{
    return size;
}

int ShortList::getValue(int pos) const
{
    assert(pos >= 0 && pos < size);
    
    return arr[pos];
}

void ShortList::print() const
{
    std::cout << "List: ";
    for (int i = 0; i < size; ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
