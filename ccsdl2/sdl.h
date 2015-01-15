#ifndef CCSDL2_SDL_H_
#define CCSDL2_SDL_H_

#include "ccsdl2/env.h"
#include "cclibc/exception.h"
#include "cclibc/trace.h"
#include "cclibc/callc.h"

CCSDL2_BG

class sdl_error: public CCLIBC::exception{
private:
	const char *_call;
public:
	sdl_error(const char*);
};

class sdl_base{
protected:
	sdl_base FUNCTION_PARAMETER_LIST();
};

inline uint8_t rgb_2_y(uint8_t r, uint8_t g, uint8_t b){
	double tv = 0.299 * r + 0.587 * g + 0.114 * b;
	return tv > 255 ? 255 : (tv < 0 ? 0 : tv);
}

inline uint8_t rgb_2_u(uint8_t r, uint8_t g, uint8_t b){
	double tv = (0.500 * r - 0.419 * g - 0.081 * b) + 127.5;
	return tv > 255 ? 255 : (tv < 0 ? 0 : tv);
}

inline uint8_t rgb_2_v(uint8_t r, uint8_t g, uint8_t b){
	double tv = (-0.169 * r - 0.331 * g + 0.500 * b) + 127.5;
	return tv > 255 ? 255 : (tv < 0 ? 0 : tv);
}

CCSDL2_END

#endif //CCSDL2_SDL_H_
