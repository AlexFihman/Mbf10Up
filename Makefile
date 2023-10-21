mbfup: mbfup.o fastlist.o uMbfObj.o mbf16c.o time_ms.o
	g++ -Wall -O3 -g mbfup.o fastlist.o uMbfObj.o mbf16c.o time_ms.o -o mbfup

mbfup.o: mbfup.cpp fastlist.h uMbfObj.h mbf16c.h time_ms.h
	g++ -Wall -O3 -g -c mbfup.cpp

fastlist.o: fastlist.cpp fastlist.h mbf16c.h
	g++ -Wall -O3 -g -c fastlist.cpp

uMbfObj.o: uMbfObj.cpp uMbfObj.h fastlist.h mbf16c.h
	g++ -Wall -O3 -g -c uMbfObj.cpp

mbf16c.o: mbf16c.cpp mbf16c.h
	g++ -Wall -O3 -g -c mbf16c.cpp

time_ms.o: time_ms.cpp time_ms.h
	g++ -Wall -O3 -g -c time_ms.cpp

clean:
	rm -f mbfup *.o
all: mbfup
