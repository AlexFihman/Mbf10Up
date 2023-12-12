#ifndef _mbf16cH_
#include "mbf16c.h"
#define _mbf16cH_
#endif

int     ties[NUM_BITS][DIMENSION];
int     levelofitem[NUM_BITS];
int     leveldepth[DIMENSION+1];
int     levellist[DIMENSION+1][NUM_BITS];

double fact[600];
double power2[600];
double C[463][463];

//int     arr1[d1];
//int     arr2[d1];

void start(){
  int i,j,k;

  for (i=0;i<NUM_BITS;i++){
    k=1;
    for (j=0;j<DIMENSION;j++){
      ties[i][j]=i ^ k;
      k=k*2;
    }
  }

  for (i=0;i<NUM_BITS;i++){
    levelofitem[i] = 0;
  }

  for (i=0;i<NUM_BITS;i++){
	  for (j=0;j<DIMENSION;j++){
		  if (ties[i][j]<i)
		  {
			  levelofitem[i]++;
		  }}}

  for (i=0;i<DIMENSION+1;i++){
    leveldepth[i]=0;
  }
  for (i=0;i<NUM_BITS;i++)
  {
    int lo = levelofitem[i];
    int ld = leveldepth[lo];
    levellist[lo][ld]=i;
    leveldepth[lo]++;
  }

  fact[0] = 1.0;
  for (i=1; i<600; i++){fact[i] = fact[i-1]*i;}
  power2[0] = 1;
  for (i=1; i<600; i++){power2[i] = power2[i-1]*2;}

  for (i=0;i<463;i++)
  for (j=0;j<463;j++)
  C[i][j]=0;

  C[0][0]= 1;
  for (i=1;i<463;i++)
  for (j=0;j<=i;j++)
  {
	  C[i][j] = C[i-1][j];
	  if (j>0) C[i][j] += C[i-1][j-1]; 
  }

}
