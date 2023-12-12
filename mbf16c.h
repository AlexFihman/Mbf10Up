//---------------------------------------------------------------------------

#ifndef mbf16cH
#define mbf16cH
//---------------------------------------------------------------------------


//constants

const int DIMENSION =  10;// dimension should be between 2 and 11. For bigger dimensions we are running into arithmetic overflows.
const int NUM_BITS =  1 << DIMENSION;


extern int     ties[NUM_BITS][DIMENSION];
extern int     levelofitem[NUM_BITS];
extern int     leveldepth[DIMENSION+1];
extern int     levellist[DIMENSION+1][NUM_BITS];

//cache values of factorials, power of 2, combinatorials. Can be used for DIMENSION up to 11
extern long double fact[600]; 
extern long double power2[600];
extern long double C[463][463];

void start();
#endif
