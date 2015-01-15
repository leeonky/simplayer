#include <SDL2/SDL.h>
#include "ccsdl2/sdl.h"

namespace{

class sdl_common_system{
public:
	sdl_common_system FUNCTION_PARAMETER_LIST(){
		VOID_INVOKE_THROW(!=0, CCSDL2::sdl_error, SDL_Init, 0);
	}
	~sdl_common_system(){
		SDL_Quit();
	}
};

}

CCSDL2_BG

sdl_error::sdl_error(const char *call): exception("%s: SDL error: %s", call, SDL_GetError()), _call(call){}

sdl_base::sdl_base FUNCTION_PARAMETER_LIST(){
	static sdl_common_system TRACE_CONSTRUCT(_com_sys);
}

CCSDL2_END
