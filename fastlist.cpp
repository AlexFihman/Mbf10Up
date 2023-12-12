#include "fastlist.h"
#include "assert.h"

uint32_t max_value = std::numeric_limits<uint32_t>::max();
std::uniform_int_distribution<uint32_t> dist(0, max_value);

TFastList::TFastList(std::mt19937* mt1){
   Count = 0;
   mt = mt1;
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
     int it = dist(*mt)%Count;
     return Items[it];
}


void  TFastList::Assign(TFastList* lst){
}
