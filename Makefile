mbfup: mbfup.cpp fastlist.cpp fastlist.h mtrand.cpp mtrand.h uMbfObj.cpp uMbfObj.h mbf16c.cpp mbf16c.h time_ms.cpp time_ms.h
	g++ -O3 mbfup.cpp fastlist.cpp mtrand.cpp uMbfObj.cpp mbf16c.cpp time_ms.cpp -o mbfup
clean:
	rm -f mbfup