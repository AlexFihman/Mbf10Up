#ifndef fastlist
#define fastlist
#include <random>
#include "mbf16c.h"

extern std::mt19937 mt;

class TFastList{
public:
	TFastList();
	int           Count;
	int  Arr[NUM_BITS];
	int  Items[NUM_BITS];

	void  Add(int i);
	void  Delete(int i);
	void  Clear();
	int  RandItem();
	void  Assign(TFastList* lst);
};
#endif
