#ifndef fastlist
#define fastlist
#include <random>
#include "mbf16c.h"

using namespace std;

class TFastList{
private:
	mt19937* mt;
public:
	TFastList(mt19937* mt1);
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
