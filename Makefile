CXX=g++
CXXFLAGS=-Wall -O3 -march=native -g -pthread

mbfup: mbfup.o fastlist.o uMbfObj.o mbf16c.o time_ms.o
	$(CXX) $(CXXFLAGS) mbfup.o fastlist.o uMbfObj.o mbf16c.o time_ms.o -o mbfup

mbfup.o: mbfup.cpp fastlist.h uMbfObj.h mbf16c.h time_ms.h
	$(CXX) $(CXXFLAGS) -c mbfup.cpp

fastlist.o: fastlist.cpp fastlist.h mbf16c.h
	$(CXX) $(CXXFLAGS) -c fastlist.cpp

uMbfObj.o: uMbfObj.cpp uMbfObj.h fastlist.h mbf16c.h
	$(CXX) $(CXXFLAGS) -c uMbfObj.cpp

mbf16c.o: mbf16c.cpp mbf16c.h
	$(CXX) $(CXXFLAGS) -c mbf16c.cpp

time_ms.o: time_ms.cpp time_ms.h
	$(CXX) $(CXXFLAGS) -c time_ms.cpp

clean:
	rm -f mbfup *.o

all: mbfup
