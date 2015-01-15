#include <SDL2/SDL_ttf.h>
#include "ccsdl2_test/ttf_test.h"
#include "ccsdl2/ttf.h"

using CCSDL2::sdlttf_lib;
using CCSDL2::sdlttf_font;
using CCSDL2::sdl_color;
using CCSDL2::rich_text_renderer;
using CCSDL2::sdl_surface;

void ttf_test_class::sdlttf_component_init(){
	{
		sdlttf_lib TRACE_CONSTRUCT(sttf);
		CPPUNIT_ASSERT(TTF_WasInit());
	}
	CPPUNIT_ASSERT(!TTF_WasInit());
}

void ttf_test_class::sdlttf_font_outline(){
	sdlttf_lib TRACE_CONSTRUCT(sttf);
	sdlttf_font font(TRACE_PARAMS("/usr/share/fonts/truetype/luxisr.ttf", 66));
	font.outline(10);
	CPPUNIT_ASSERT(font.outline() == 10);
}

void ttf_test_class::surface_size_properties(){
	sdlttf_font font(TRACE_PARAMS("/usr/share/fonts/truetype/luxisr.ttf", 66));
	sdl_surface surface(TRACE_PARAMS("hello", sdlttf_font::blended, font, sdl_color(255,255,255,255)));
	CPPUNIT_ASSERT(surface.width() == (size_t)surface._surface->w);
	CPPUNIT_ASSERT(surface.height() == (size_t)surface._surface->h);
}

void ttf_test_class::sdlttf_font_style(){
	sdlttf_font font(TRACE_PARAMS("/usr/share/fonts/truetype/luxisr.ttf", 66));
	CPPUNIT_ASSERT(font.style() == sdlttf_font::normal);
	sdlttf_font &f2 = font.style(sdlttf_font::bold|sdlttf_font::underline);
	CPPUNIT_ASSERT(&f2 == &font);
	CPPUNIT_ASSERT(font.style() & sdlttf_font::bold);
	CPPUNIT_ASSERT(font.style() & sdlttf_font::underline);
}

void ttf_test_class::rich_text_renderer_new(){
	const char *file = "/usr/share/fonts/truetype/DroidSansFallback.ttf";
	sdl_color c(1,2,3,4);
	rich_text_renderer dec(TRACE_PARAMS(file, 50, c));
	CPPUNIT_ASSERT(dec._font.raw_pointer());
	CPPUNIT_ASSERT(dec._size == 50);
	CPPUNIT_ASSERT(!memcmp(&dec._color, &c, sizeof(sdl_color)));
	CPPUNIT_ASSERT(!strcmp(static_cast<char*>(dec._font_file.raw_pointer()), file));
}

void ttf_test_class::rich_text_renderer_change_size(){
	const char *file = "/usr/share/fonts/truetype/DroidSansFallback.ttf";
	sdl_color c(1,2,3,4);
	rich_text_renderer dec(TRACE_PARAMS(file, 50, c));
	void *p1 = dec._font;
	dec.font_size(20);
	
	CPPUNIT_ASSERT(dec._font.raw_pointer());
	CPPUNIT_ASSERT(dec._font != p1);
	CPPUNIT_ASSERT(dec._size == 20);
}
