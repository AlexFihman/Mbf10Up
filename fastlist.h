#ifndef fastlist
#define fastlist
#include "mtrand.h"

class TFastList{
private:
	static MTRand mt;
public:
	TFastList();
	int           Count;
	int  Arr[1024];
	int  Items[1024];

	void  Add(int i);
	void  Delete(int i);
	void  Clear();
	int  RandItem();
	void  Assign(TFastList* lst);
};
#endif
