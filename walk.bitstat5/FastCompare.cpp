
#include "FastCompare.h"

FastCompare::FastCompare(std::vector<MonotoneBooleanFunction>& mbfList, size_t list_size) {
    endPoints.reserve(list_size);
    for (size_t ls = 0; ls < list_size; ls++)
    {
        ShortList sl = mbfList[ls].getMinCNF();

        for(size_t i = 0; i < sl.getSize(); i++) {
            leftIndices.push_back(sl.getValue(i));
        }
        endPoints.push_back(leftIndices.size());
    }

    std::cout << "endPoints.size(): " << endPoints.size() << "   leftIndices.size(): " << leftIndices.size() << std::endl;
}
