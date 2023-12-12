#include "fastlist.h"
#include "mbf16c.h"
#include "fastlist.h"
//---------------------------------------------------------------------------

#ifndef uMbfObjH
#define uMbfObjH
//---------------------------------------------------------------------------


class TMbfObj {

public:

  TMbfObj*  ParentObj;
  bool arr[NUM_BITS];
  TFastList* InList[DIMENSION+1];
  TFastList* OutList[DIMENSION+1];
  char   lvl[DIMENSION+1];
  char   LvlGrey[DIMENSION+1];

  TMbfObj(std::mt19937* mt);
  ~TMbfObj();
  void AddItem(int item);
  void AddRandItem(int curr_lvl);
  void AddRandItem2(int curr_lvl);
  void ClearLevel(int curr_lvl);
};

long double SumOfMbf(TMbfObj* mbfobj, int CurrLvl, long double CurrSum);
//long double C(int x, int y);

#endif
