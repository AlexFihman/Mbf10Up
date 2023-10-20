mbfup: mbfup.o fastlist.o mtrand.o uMbfObj.o mbf16c.o time_ms.o
	g++ -O3 mbfup.o fastlist.o mtrand.o uMbfObj.o mbf16c.o time_ms.o -o mbfup

mbfup.o: mbfup.cpp
	g++ -O3 -g -c mbfup.cpp

fastlist.o: fastlist.cpp fastlist.h
	g++ -O3 -g -c fastlist.cpp

mtrand.o: mtrand.cpp mtrand.h
	g++ -O3 -g -c mtrand.cpp

uMbfObj.o: uMbfObj.cpp uMbfObj.h
	g++ -O3 -g -c uMbfObj.cpp

mbf16c.o: mbf16c.cpp mbf16c.h
	g++ -O3 -g -c mbf16c.cpp

time_ms.o: time_ms.cpp time_ms.h
	g++ -O3 -g -c time_ms.cpp

clean:
	rm -f mbfup *.o
