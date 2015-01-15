#include "cclibc/thread/thread.h"

CCLIBC_BG

bool thread::sleep_nano FUNCTION_PARAMETER_LIST(uint64_t &ns){
	timespec s, s2;
	s.tv_sec = ns / 1000000000;
	s.tv_nsec = (ns % 1000000000);
	if(nanosleep(&s, &s2)){
		if(EINTR == errno){
			ns = s2.tv_sec;
			ns = ns * 1000000000 + s2.tv_nsec;
			return true;
		}
		THROW_NEW_EXCEPTION(posix_error, "nonosleep");
	}
	return false;
}

void thread::sleep_nano FUNCTION_PARAMETER_LIST(const uint64_t &ns){
	timespec s, s2;
	s.tv_sec = ns / 1000000000;
	s.tv_nsec = (ns % 1000000000);
	INVOKE_THROW(int, != 0, posix_error, nanosleep, &s, &s2);
}

CCLIBC_END
