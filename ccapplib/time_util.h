#ifndef CCAPPLIB_TIME_UTIL_H_
#define CCAPPLIB_TIME_UTIL_H_

#include <cstdio>
#include <sys/time.h>
#include <stdint.h>
#include "ccapplib/env.h"

CCAPPLIB_BG

class time_bucket{
private:
	size_t _start, _end;
public:
	time_bucket(size_t s, size_t b): _start(s), _end(s + b){}
	size_t start() const{
		return _start;
	}
	size_t end() const{
		return _end;
	}
	bool contains(size_t t) const{
		return t>=_start && t<_end;
	}
};

class time_util{
public:
	static uint64_t to_millisecond(const timeval tm){
		return ((uint64_t)tm.tv_sec)*1000 + tm.tv_usec/1000;
	}
	static uint64_t current_millisecond(){
		timeval tm;
		gettimeofday(&tm, NULL);
		return to_millisecond(tm);
	}
};

class clock{
private:
	timeval _start;
	timeval _last;

public:
	clock(){
		gettimeofday(&_start, NULL);
		gettimeofday(&_last, NULL);
	}
	
	size_t last_millisecond(){
		uint64_t t = time_util::to_millisecond(_last);
		gettimeofday(&_last, NULL);
		return time_util::to_millisecond(_last) - t;
	}
	
	size_t total_millisecond(){
		uint64_t t = time_util::to_millisecond(_start);
		gettimeofday(&_last, NULL);
		return time_util::to_millisecond(_last) - t;
	}
};

CCAPPLIB_END

#endif //CCAPPLIB_TIME_UTIL_H_
