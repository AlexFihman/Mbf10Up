#include "uMbfObj.h"
#include "mbf16c.h"
#include <string.h>

TMbfObj::TMbfObj(std::mt19937* mt){
  int i,j;
  ParentObj = NULL;
  for (i=0; i<DIMENSION+1; i++){
    InList[i].Init(mt);}
  for (i=0; i<DIMENSION+1; i++){
    OutList[i].Init(mt);}

  for (i=0;i<DIMENSION+1;i++)
  {
    for (j=0;j<leveldepth[i];j++)
    {
      InList[i].Add(levellist[i][j]);
      LvlGrey[i]=0;
      lvl[i] = 0;
    }
  }
  for (i=0;i<NUM_BITS;i++){
    arr[i]= false;
  }
}

TMbfObj::~TMbfObj(){
};

long double SumOfMbf(TMbfObj* mbfobj, int CurrLvl, long double CurrSum){
  int i,j;
  int cnt;
  long double comb;
  int CalcType;
  long double result = 0;
  CalcType = 3;
  if (CurrLvl+2 > DIMENSION) {CalcType = 2;} else
    if (CurrLvl+2 > DIMENSION) {CalcType = 1;} else
      if (CurrLvl+2<= DIMENSION) {
        if (mbfobj->InList[CurrLvl+2].Count==0)
        {
           CalcType = 2;
           if (CurrLvl+1<= DIMENSION){
             if (mbfobj->InList[CurrLvl+1].Count==0){
             CalcType = 1;
             }
           }
        }
      };

  switch (CalcType) {
  case 1: result = CurrSum*power2[mbfobj->InList[CurrLvl].Count];
          break;
  case 2:
          cnt = mbfobj->InList[CurrLvl].Count;
          for (j=0;j<=cnt; j++){
            comb = C[cnt][j];
            result = result + CurrSum * comb * power2[mbfobj->InList[CurrLvl+1].Count];
            if (j<cnt){mbfobj->AddRandItem2(CurrLvl);}
          }
          mbfobj->ClearLevel(CurrLvl);
          break;
  case 3: cnt = mbfobj->InList[CurrLvl].Count;
          for (i=0;i<=cnt;i++)
          {
            comb = C[cnt][i];
            result = result + SumOfMbf(mbfobj,CurrLvl+1,CurrSum*comb);
            if (i<cnt){mbfobj->AddRandItem(CurrLvl);}
          }
          mbfobj->ClearLevel(CurrLvl);
          break;
  };
  return result;
}

void TMbfObj::AddItem(int item){
  int curr_lvl = levelofitem[item];
  int j,lv;
  InList[curr_lvl].Delete(item);
  OutList[curr_lvl].Add(item);
  arr[item] = true;
  lvl[curr_lvl]++;
  int ns = superset_count[item];
  for (j=0; j<ns; j++){
    int sup = supersets[item][j];
    if (!arr[sup]){
      lv = levelofitem[sup];
      InList[lv].Delete(sup);
      OutList[lv].Add(sup);
      LvlGrey[lv]++;
      arr[sup] = true;
    }
  }
}

void TMbfObj::AddRandItem(int curr_lvl){
  int Item;
  int j,lv;
  Item = InList[curr_lvl].RandItem();
  InList[curr_lvl].Delete(Item);
  OutList[curr_lvl].Add(Item);
  arr[Item] = true;
  lvl[curr_lvl]++;
  int ns = superset_count[Item];
  for (j=0; j<ns; j++){
    int sup = supersets[Item][j];
    if (!arr[sup]){
      lv = levelofitem[sup];
      InList[lv].Delete(sup);
      OutList[lv].Add(sup);
      LvlGrey[lv]++;
      arr[sup] = true;
    }
  }
}

void TMbfObj::AddRandItem2(int curr_lvl){
  int item, item2;
  int j,lv;

  item = InList[curr_lvl].RandItem();
  InList[curr_lvl].Delete(item);
  OutList[curr_lvl].Add(item);
  arr[item] = true;
  lvl[curr_lvl]++;

  for (j = 0;j<DIMENSION;j++){
    item2 = ties[item][j];
    if ( ((item2 & item) == item) && (!arr[item2]) )
    {
      lv = curr_lvl + 1;
      InList[lv].Delete(item2);
      OutList[lv].Add(item2);
      LvlGrey[lv]++;
      arr[item2] = true;
    }
  }
}

void TMbfObj::ClearLevel(int curr_lvl){
  int i,j,item,lv;

  for (i=curr_lvl;i<DIMENSION+1;i++){
    OutList[i].Count = 0;
    int depth = leveldepth[i];
    memcpy(InList[i].Items, levellist[i], depth * sizeof(int));
    for (j=0; j<depth; j++)
      InList[i].Arr[levellist[i][j]] = j;
    InList[i].Count = depth;
    LvlGrey[i]=0;
    lvl[i] = 0;
  }

  for (j=0;j<NUM_BITS;j++){
    if (levelofitem[j]>=curr_lvl)
      {arr[j]=false;}
  }

  if (curr_lvl>0)
  {
    for (i=0; i<OutList[curr_lvl-1].Count; i++)
    {
      item = OutList[curr_lvl-1].Items[i];
      int ns = superset_count[item];
      for (j=0; j<ns; j++){
        int sup = supersets[item][j];
        if (!arr[sup]){
          lv = levelofitem[sup];
          InList[lv].Delete(sup);
          OutList[lv].Add(sup);
          LvlGrey[lv]++;
          arr[sup] = true;
        }
      }
    }
  }
}
