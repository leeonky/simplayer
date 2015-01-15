#include "cctest/util.h"
#include "ccsdl2/sdl.cpp"

INTERFACE_TESTABLE_FUNCTION_1(int, SDL_Init, r_non_call<int>, uint32_t)
INTERFACE_TESTABLE_FUNCTION(const char*, SDL_GetError, r_non_call<const char*>)
INTERFACE_TESTABLE_VOID_FUNCTION(SDL_Quit, non_call)
INTERFACE_TESTABLE_FUNCTION_1(int, SDL_InitSubSystem, r_non_call<int>, uint32_t)
INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_QuitSubSystem, non_call, uint32_t)
