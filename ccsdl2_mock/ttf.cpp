#include <alloca.h>
#include "cctest/util.h"
#include "ccsdl2/ttf.cpp"
#include "cclibc/text.h"

namespace{
	SDL_Texture* sdl_createtexturefromsurface(SDL_Renderer*, SDL_Surface*){
		SDL_Texture*r = (SDL_Texture*)malloc(1920*1080*4);
		printf("A:%p\n", r);
		return r;
	}
}

INTERFACE_TESTABLE_FUNCTION_2(SDL_Texture*, SDL_CreateTextureFromSurface, sdl_createtexturefromsurface, SDL_Renderer*, SDL_Surface*)
INTERFACE_TESTABLE_FUNCTION(int, TTF_Init, r_non_call<int>)
INTERFACE_TESTABLE_VOID_FUNCTION(TTF_Quit, non_call)

INTERFACE_TESTABLE_FUNCTION_2(TTF_Font*, TTF_OpenFont, r_non_call_value<TTF_Font*>, const char*, int)
INTERFACE_TESTABLE_VOID_FUNCTION_1(TTF_CloseFont, non_call, TTF_Font*)
INTERFACE_TESTABLE_VOID_FUNCTION_2(TTF_SetFontOutline, non_call, TTF_Font*, int)
INTERFACE_TESTABLE_FUNCTION_1(int, TTF_GetFontOutline, r_non_call<int>, const TTF_Font*)
INTERFACE_TESTABLE_VOID_FUNCTION_2(TTF_SetFontStyle, non_call, TTF_Font*, int)
INTERFACE_TESTABLE_FUNCTION_1(int, TTF_GetFontStyle, r_non_call<int>, const TTF_Font*)

INTERFACE_TESTABLE_FUNCTION_4(int, TTF_SizeUTF8, r_non_call<int>, TTF_Font*, const char*, int*, int*)
INTERFACE_TESTABLE_FUNCTION_1(int, TTF_FontLineSkip, r_non_call<int>, const TTF_Font*)

INTERFACE_TESTABLE_FUNCTION_3(SDL_Surface*, TTF_RenderUTF8_Solid, r_non_call_value<SDL_Surface*>, TTF_Font*, const char*, SDL_Color)
INTERFACE_TESTABLE_FUNCTION_4(SDL_Surface*, TTF_RenderUTF8_Shaded, r_non_call_value<SDL_Surface*>, TTF_Font*, const char*, SDL_Color, SDL_Color)
INTERFACE_TESTABLE_FUNCTION_3(SDL_Surface*, TTF_RenderUTF8_Blended, r_non_call_value<SDL_Surface*>, TTF_Font*, const char*, SDL_Color)

INTERFACE_TESTABLE_FUNCTION_3(SDL_Surface*, TTF_RenderUNICODE_Solid, r_non_call_value<SDL_Surface*>, TTF_Font*, const uint16_t*, SDL_Color)
INTERFACE_TESTABLE_FUNCTION_4(SDL_Surface*, TTF_RenderUNICODE_Shaded, r_non_call_value<SDL_Surface*>, TTF_Font*, const uint16_t*, SDL_Color, SDL_Color)
INTERFACE_TESTABLE_FUNCTION_3(SDL_Surface*, TTF_RenderUNICODE_Blended, r_non_call_value<SDL_Surface*>, TTF_Font*, const uint16_t*, SDL_Color)

INTERFACE_TESTABLE_FUNCTION_2(int, SDL_SetSurfaceBlendMode, r_non_call<int>, SDL_Surface*, SDL_BlendMode)
INTERFACE_TESTABLE_FUNCTION_4(int, SDL_BlitSurface, r_non_call<int>, SDL_Surface*, const SDL_Rect*, SDL_Surface*, SDL_Rect*)

INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_FreeSurface, non_call, SDL_Surface*)

