#include "util.h"

#include <sys/time.h>

float signum(float x) {
	if(x < 0) {
		return -1;
	} else if(x == 0) {
		return 0;
	} else {
		return 1;
	}
}

//  Windows
#ifdef _WIN32
#include <Windows.h>
double getWallTime(){
	LARGE_INTEGER time,freq;
	if (!QueryPerformanceFrequency(&freq)){
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time)){
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}

//  Posix/Linux
#else
#include <sys/time.h>
#include <cstdlib>
double getWallTime(){
	struct timeval time;
	if (gettimeofday(&time,NULL)){
		//  Handle error
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
#endif