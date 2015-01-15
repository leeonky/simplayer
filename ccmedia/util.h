#ifndef CCMEDIA_UTIL_H
#define CCMEDIA_UTIL_H

#include "ccmedia/env.h"
#include "cclibc/exception.h"
#include "ccapplib/condition_var.h"
#include "cclibc/thread/mutex.h"

CCMEDIA_BG

class alsa_player;
class player_parameters{
friend class alsa_player;
private:
	enum command{
		nothing = 0, pause, resume, stop
	};
	
public:
	enum states{
		running, pausing, stopped, unknown = -1
	};
};

class clock_source{
public:
	virtual ~clock_source(){}
	virtual size_t current_clock FUNCTION_PARAMETER_LIST() = 0;
};

#ifdef STACK_TRACE_FLAG

#define current_clock(...) current_clock(INVOKE_TRACE, ##__VA_ARGS__)

#endif


CCMEDIA_END

#endif //CCMEDIA_UTIL_H
