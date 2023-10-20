// mbfup.cpp : Defines the entry point for the console application.
//
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "mbf16c.h"
#include "uMbfObj.h"
#include "time_ms.h"
#include "mtrand.h"

using namespace std;

int main(int argc, char** argv)
{
	int loops = 1000;
	int seed = 1;
	if (argc > 1) seed = atoi(argv[1]);
	cout << "seed =" << seed << endl;

	start();
	std::cout.precision(15);
	static int c1 = d2 >> 1;
	static int c2 = d2 - c1;
	static int c3 = leveldepth[c1];
	static TMbfObj mbfObj1;
	static TMbfObj mbfObj2;
	MTRand::Seed(seed);
	long double sum = 0;
	long double sums = 0;
	long double sums2 = 0;
	double t0 = TimeMillis();
	for (int cycle=0; cycle < loops; cycle++){
		sum = 0;
		mbfObj1.ClearLevel(c2);
		mbfObj2.ClearLevel(c1);
		for (int i=0;i<=c3;i++){
			for (int j=0;j<mbfObj1.InList[c2]->Count;j++){
				int item = mbfObj1.InList[c2]->Items[j];
				item ^= d1-1;
				mbfObj2.AddItem(item);
			}
			long double sum1 = SumOfMbf(&mbfObj1,c2+1,1);
			long double sum2 = SumOfMbf(&mbfObj2,c1+1,1);
			sum += sum1*sum2*C[c3][i];
			if (i<c3)
			{
				mbfObj1.AddRandItem(c2);
				mbfObj2.ClearLevel(c1);
			}						
		}
		sums += sum;
		sums2 += sum * sum;
		std::cout << sum << std::endl;
	}
	double t1 = TimeMillis();
	long double avg = sums/loops;
	long double stddev = sqrt(sums2/loops - avg*avg)/sqrt(loops-1);
	cout << "avg:  " << avg << endl;
	cout << "std.dev:  " << stddev << endl;
	cout << "runtime: " << (t1-t0) << endl;
	return 0;
}
