//---------------------------------------------------------------------------

#ifndef mbf16cH
#define mbf16cH
//---------------------------------------------------------------------------


//constants

const int d1 =  1024;      //   {2^d2}
const int d2 =  10;      //   {d3-1}
const int d3 =  11;


extern int     ties[d1][d2];
extern int     levelofitem[d1];
extern int     leveldepth[d3];
extern int     levellist[d3][d1];

extern long double fact[600];
extern long double power2[600];
extern long double C[463][463];

void start();
#endif
