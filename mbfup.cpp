// mbfup.cpp : Defines the entry point for the console application.
//
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <thread>
#include <math.h>
#include <random>
#include "mbf16c.h"
#include "uMbfObj.h"
#include "time_ms.h"
#include "fastlist.h"

using namespace std;

const int loops = 1000;

int c1,c2,c3;

void thread_task(int seed) {
	//cout << "seed =" << seed << endl;

	std::ofstream wf("experiment"+to_string(seed)+".dat", ios::out | ios::binary);
   	if(!wf) {
          cout << "Cannot open file!" << endl;
          return;
        }

	mt19937* mt = new mt19937();
	mt->seed(seed);


	std::cout.precision(15);
	TMbfObj mbfObj1(mt);
	TMbfObj mbfObj2(mt);

	long double sum;
	//long double sums = 0;
	//long double sums2 = 0;

	for (int cycle=0; cycle < loops; cycle++){
		sum = 0;
		mbfObj1.ClearLevel(c2);
		mbfObj2.ClearLevel(c1);
		for (int i=0;i<=c3;i++){
			for (int j=0;j<mbfObj1.InList[c2]->Count;j++){
				int item = mbfObj1.InList[c2]->Items[j];
				item ^= NUM_BITS-1;
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
		//sums += sum;
		//sums2 += sum * sum;
		//std::cout << seed << "\t" << cycle << "\t" << sum << std::endl;
                wf.write((char*)&seed, sizeof(int));
                wf.write((char*)&cycle, sizeof(int));
                wf.write((char*)&sum, sizeof(long double));
	}
        wf.close();

	//long double avg = sums/loops;
	//long double stddev = sqrt(sums2/loops - avg*avg)/sqrt(loops-1);
	//cout << "avg:  " << avg << endl;
	//cout << "std.dev:  " << stddev << endl;
}


int main(int argc, char** argv)
{
	start();
        c1 = DIMENSION >> 1;
        c2 = DIMENSION - c1;
        c3 = leveldepth[c1];

	int seed = 1;
	if (argc > 1) seed = atoi(argv[1]);

	int nthread = 10;

	double t0 = TimeMillis();
	thread** threads = new thread*[nthread];
	for (int i=0; i<nthread; i++) {
		threads[i] = new thread(thread_task, seed+i);
	}
	for (int i=0; i<nthread; i++) {
		threads[i]->join();
	}
	double t1 = TimeMillis();
	cout << "runtime: " << (t1-t0) << endl;
	return 0;
}
