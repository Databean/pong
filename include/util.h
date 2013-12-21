#ifndef UTIL_H
#define UTIL_H

#include <sstream>
#include <string>

template<typename T>
T fromString(const std::string& str) {
	std::stringstream stream(str);
	T ret;
	str >> ret;
	return ret;
}

template<typename T>
std::string toString(const T& t) {
	std::stringstream stream;
	stream << t;
	return stream.str();
}

float signum(float x);

double getWallTime();

#endif