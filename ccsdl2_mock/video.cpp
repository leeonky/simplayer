#include "cctest/util.h"
#include "ccsdl2/video.cpp"

namespace{
	SDL_Window* sdl_createwindow(const char*, int, int, int, int, uint32_t){
		return (SDL_Window*)1;
	}
	SDL_Renderer* sdl_createrenderer(SDL_Window*, int, uint32_t){
		return (SDL_Renderer*)1;
	}
	SDL_Texture* sdl_createtexture(SDL_Renderer*, uint32_t, int, int, int){
		return (SDL_Texture*)malloc(1920*1080*4);
	}
	int lock_texture(SDL_Texture *t, const SDL_Rect*, void **p, int *l){
		*p = t;
		*l = 1;
		return 0;
	}
	void sdl_destroytexture(SDL_Texture *t){
		free(t);
	}
	void sdl_getwindowsize(SDL_Window*, int *w, int *h){
		*w = 50;
		*h = 20;
	}
}

INTERFACE_TESTABLE_FUNCTION_6(SDL_Window*, SDL_CreateWindow, sdl_createwindow, const char*, int, int, int, int, uint32_t)
INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_DestroyWindow, non_call, SDL_Window*)
INTERFACE_TESTABLE_VOID_FUNCTION_3(SDL_GetWindowSize, sdl_getwindowsize, SDL_Window*, int*, int*)
INTERFACE_TESTABLE_VOID_FUNCTION_3(SDL_SetWindowSize, non_call, SDL_Window*, int, int)
INTERFACE_TESTABLE_VOID_FUNCTION_3(SDL_GetWindowPosition, non_call, SDL_Window*, int*, int*)
INTERFACE_TESTABLE_VOID_FUNCTION_3(SDL_SetWindowPosition, non_call, SDL_Window*, int, int)
INTERFACE_TESTABLE_FUNCTION_3(SDL_Renderer*, SDL_CreateRenderer, sdl_createrenderer, SDL_Window*, int, uint32_t)
INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_DestroyRenderer, non_call, SDL_Renderer*)
INTERFACE_TESTABLE_FUNCTION_5(SDL_Texture*, SDL_CreateTexture, sdl_createtexture, SDL_Renderer*, uint32_t, int, int, int)
INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_DestroyTexture, sdl_destroytexture, SDL_Texture*)
INTERFACE_TESTABLE_FUNCTION_5(int, SDL_QueryTexture, r_non_call<int>, SDL_Texture*, uint32_t*, int*, int*, int*)
INTERFACE_TESTABLE_FUNCTION_2(int, SDL_SetTextureBlendMode, r_non_call<int>, SDL_Texture*, SDL_BlendMode)
INTERFACE_TESTABLE_FUNCTION_4(int, SDL_LockTexture, lock_texture, SDL_Texture*, const SDL_Rect*, void**, int*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_UnlockTexture, non_call, SDL_Texture*)
INTERFACE_TESTABLE_FUNCTION_4(int, SDL_RenderCopy, r_non_call<int>, SDL_Renderer*, SDL_Texture*, const SDL_Rect*, const SDL_Rect*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(SDL_RenderPresent, non_call, SDL_Renderer*)
INTERFACE_TESTABLE_FUNCTION_4(int, SDL_UpdateTexture, r_non_call<int>, SDL_Texture*, const SDL_Rect*, const void*, int)
INTERFACE_TESTABLE_FUNCTION_8(int, SDL_UpdateYUVTexture, r_non_call<int>, SDL_Texture*, const SDL_Rect*, const uint8_t*, int, const uint8_t*, int, const uint8_t*, int)
