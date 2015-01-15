#include "ccsdl2_test/video_test.h"
#include "ccsdl2/video.h"
#include <SDL2/SDL.h>

using CCSDL2::sdl_window;
using CCSDL2::sdl_renderer;
using CCSDL2::sdl_texture;
using CCSDL2::sdl_color;
using CCSDL2::sdl_rect;
using CCSDL2::sdl_locked_txture;

void video_test_class::window_init(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	CPPUNIT_ASSERT(wnd._wnd);
	CPPUNIT_ASSERT(SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO);
}

void video_test_class::window_size(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	CPPUNIT_ASSERT(wnd.width() == 300);
	CPPUNIT_ASSERT(wnd.height() == 400);
	wnd.size(200,100);
	CPPUNIT_ASSERT(wnd.width() == 200);
	CPPUNIT_ASSERT(wnd.height() == 100);
}

void video_test_class::window_postion(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	CPPUNIT_ASSERT(wnd.x() == 0);
	CPPUNIT_ASSERT(wnd.y() == 0);
	wnd.position(30,40);
	CPPUNIT_ASSERT(wnd.x() == 30);
	CPPUNIT_ASSERT(wnd.y() == 40);
}

void video_test_class::renderer_init(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	CPPUNIT_ASSERT(rnder._renderer);
}

void video_test_class::texture_init(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	CPPUNIT_ASSERT(txture._texture);
}

void video_test_class::texture_size(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 300, 400, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	CPPUNIT_ASSERT(txture.width() == 100);
	CPPUNIT_ASSERT(txture.height() == 200);
}
