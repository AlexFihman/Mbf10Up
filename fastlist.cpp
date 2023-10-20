#include "fastlist.h"
#include "string.h"
#include "mtrand.h"
#include "assert.h"

TFastList::TFastList(){
   Count = 0;
};

void TFastList::Add(int i){
     Items[Count] = i;
     Arr[i] = Count;
     Count++;
}

void TFastList::Delete(int i){
     Count--;
     int pos = Arr[i];
     assert(pos >=0);
     int item;
     Arr[i] = -1;
     if (pos < Count){
       item = Items[Count];
       Items[pos] = item;
       Arr[item] = pos;
     }
}

void TFastList::Clear(){
     //memset(&Arr,0xFF,sizeof(Arr));//comment if you don't need lookup
     Count = 0;
}

int TFastList::RandItem(){
     //return Items[MRandom(Count)];
     assert (Count > 0);
     int it = mt.Rand()%Count;
     return Items[it];
}


void  TFastList::Assign(TFastList* lst){
}
