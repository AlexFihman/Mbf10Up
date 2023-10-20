#ifndef _mbf16cH_
#include "mbf16c.h"
#define _mbf16cH_
#endif

int     ties[d1][d2];
int     levelofitem[d1];
int     leveldepth[d3];
int     levellist[d3][d1];

long double fact[600];
long double power2[600];
long double C[463][463];

//int     arr1[d1];
//int     arr2[d1];

void start(){
  int i,j,k;

  for (i=0;i<d1;i++){
    k=1;
    for (j=0;j<d2;j++){
      ties[i][j]=i ^ k;
      k=k*2;
    }
  }

  for (i=0;i<d1;i++){
    levelofitem[i] = 0;
  }

  for (i=0;i<d1;i++){
	  for (j=0;j<d2;j++){
		  if (ties[i][j]<i)
		  {
			  levelofitem[i]++;
		  }}}

  for (i=0;i<d3;i++){
    leveldepth[i]=0;
  }
  for (i=0;i<d1;i++)
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
