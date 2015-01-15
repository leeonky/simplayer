#include "ccsdl2_mock_test/ttf_test.h"
#include "ccsdl2/ttf.h"
#include "cclibc/text.h"
#include "cclibc/util/array_list.h"

using CCLIBC::array_list;
using CCSDL2::sdlttf_lib;
using CCSDL2::sdlttf_font;
using CCSDL2::sdl_surface;
using CCSDL2::sdl_color;
using CCSDL2::rich_text_renderer;
using CCSDL2::sdl_video;
using CCSDL2::sdl_window;
using CCSDL2::sdl_renderer;
using CCSDL2::sdl_texture;
using CCSDL2::sdl_rect;

void ttf_test_class::sdlttf_component_init(){
	interface_testor::clear();
	{
		sdlttf_lib TRACE_CONSTRUCT(sttf);
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_Init"));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_Quit"));
}

void ttf_test_class::sdlttf_font_init(){
	interface_testor::clear();
	void *p;
	{
		const char *fp = "font.ttf";
		sdlttf_font font(TRACE_PARAMS(fp, 66));
		p = font._font;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_Init"));
		CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_OpenFont"));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(fp));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(66));
	}
	CPPUNIT_ASSERT(interface_testor::method(2).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_outline(){
	sdlttf_font font(TRACE_PARAMS("", 66));
	interface_testor::clear();
	sdlttf_font &f = font.outline(10);
	font.outline();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_SetFontOutline"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(10));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_GetFontOutline"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(font._font));
	CPPUNIT_ASSERT(&f == &font);
}

void ttf_test_class::sdlttf_font_surface_solid(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::solid, font, c));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Solid"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_shaded(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::shaded, font, c));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Shaded"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(sdl_color(0,0,0,0)));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_blended(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::blended, font, c));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Blended"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_blended2(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4), cb(4,3,2,1);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, font, c, cb));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Shaded"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(cb));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_solid_uni(){
	void *p;
	{
		uint16_t str[10];
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::solid, font, c));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUNICODE_Solid"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_shaded_uni(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::shaded, font, c));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Shaded"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(sdl_color(0,0,0,0)));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_blended_uni(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::blended, font, c));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Blended"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdlttf_font_surface_blended2_uni(){
	void *p;
	{
		const char *str = "Hello";
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4), cb(4,3,2,1);
		interface_testor::clear();
		sdl_surface tsf(TRACE_PARAMS(str, font, c, cb));
		p = tsf._surface;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_RenderUTF8_Shaded"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(c));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(cb));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::surface_blit_to(){
	const char *str = "Hello";
	sdlttf_font font(TRACE_PARAMS("", 66));
	sdl_color c(1,2,3,4), cb(4,3,2,1);
	interface_testor::clear();
	sdl_surface tsf1(TRACE_PARAMS(str, font, c, cb));
	sdl_surface tsf2(TRACE_PARAMS(str, font, c, cb));
	interface_testor::clear();
	tsf1.blit_to(sdl_renderer::blend, tsf2);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_SetSurfaceBlendMode"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(sdl_renderer::blend));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_BlitSurface"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(tsf2._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(3).equal(NULL));
}

void ttf_test_class::surface_blit_to_sr(){
	const char *str = "Hello";
	sdlttf_font font(TRACE_PARAMS("", 66));
	sdl_color c(1,2,3,4), cb(4,3,2,1);
	interface_testor::clear();
	sdl_surface tsf1(TRACE_PARAMS(str, font, c, cb));
	sdl_surface tsf2(TRACE_PARAMS(str, font, c, cb));
	interface_testor::clear();
	sdl_rect r(1,1,3,3);
	tsf1.blit_to(sdl_renderer::blend, r, tsf2);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_SetSurfaceBlendMode"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(sdl_renderer::blend));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_BlitSurface"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(&r));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(tsf2._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(3).equal(NULL));
}

void ttf_test_class::surface_blit_to_dr(){
	const char *str = "Hello";
	sdlttf_font font(TRACE_PARAMS("", 66));
	sdl_color c(1,2,3,4), cb(4,3,2,1);
	interface_testor::clear();
	sdl_surface tsf1(TRACE_PARAMS(str, font, c, cb));
	sdl_surface tsf2(TRACE_PARAMS(str, font, c, cb));
	interface_testor::clear();
	sdl_rect r(1,1,3,3);
	tsf1.blit_to(sdl_renderer::blend, tsf2, r);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_SetSurfaceBlendMode"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(sdl_renderer::blend));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_BlitSurface"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(tsf2._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(3).equal(&r));
}

void ttf_test_class::surface_blit_to_sr_dr(){
	const char *str = "Hello";
	sdlttf_font font(TRACE_PARAMS("", 66));
	sdl_color c(1,2,3,4), cb(4,3,2,1);
	interface_testor::clear();
	sdl_surface tsf1(TRACE_PARAMS(str, font, c, cb));
	sdl_surface tsf2(TRACE_PARAMS(str, font, c, cb));
	interface_testor::clear();
	sdl_rect r1(1,1,3,3), r2(2,2,10,10);
	tsf1.blit_to(sdl_renderer::blend, r1, tsf2, r2);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_SetSurfaceBlendMode"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(sdl_renderer::blend));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_BlitSurface"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(tsf1._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(&r1));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(tsf2._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(3).equal(&r2));
}

void ttf_test_class::sdlttf_font_size_of(){
	const char *str = "Hello";
	size_t w, h;
	sdlttf_font font(TRACE_PARAMS("", 66));
	interface_testor::clear();
	font.text_size(str, w, h);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_SizeUTF8"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(str));
}

void ttf_test_class::sdlttf_font_line_height(){
	sdlttf_font font(TRACE_PARAMS("", 66));
	interface_testor::clear();
	font.skip_line_height();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_FontLineSkip"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
}

void ttf_test_class::texture_init_from_surface(){
	const char *str = "Hello";
	sdlttf_font font(TRACE_PARAMS("", 66));
	sdl_color c(1,2,3,4);
	sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::solid, font, c));
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
	interface_testor::clear();
	sdl_texture txture(TRACE_PARAMS(rnder, tsf));

	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_CreateTextureFromSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(tsf._surface));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_QueryTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(txture._texture));
}

void ttf_test_class::sdlttf_change_style(){
	sdlttf_font font(TRACE_PARAMS("", 66));
	interface_testor::clear();
	font.style(sdlttf_font::bold);
	font.style();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_SetFontStyle")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(font._font));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(TTF_STYLE_BOLD));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_GetFontStyle")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(font._font));
}

void ttf_test_class::rich_text_renderer_new_font_mgt(){
	void *p;
	const char *file = "/usr/share/fonts/truetype/DroidSansFallback.ttf";
	{
		sdl_color c(1,2,3,4);
		interface_testor::clear();
		rich_text_renderer dec(TRACE_PARAMS(file, 50, c));
		p = dec._font->_font;
	}
	CPPUNIT_ASSERT(interface_testor::last_method(1).equal("TTF_OpenFont"));
	CPPUNIT_ASSERT(interface_testor::last_method(1).parameter(0).equal(file));
	CPPUNIT_ASSERT(interface_testor::last_method(1).parameter(1).equal(50));
	CPPUNIT_ASSERT(interface_testor::last_method(0).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(0).equal(p));
}

void ttf_test_class::rich_text_renderer_change_size(){
	const char *file = "/usr/share/fonts/truetype/DroidSansFallback.ttf";
	sdl_color c(1,2,3,4);
	rich_text_renderer dec(TRACE_PARAMS(file, 50, c));
	void *p = dec._font->_font;
	interface_testor::clear();
	dec.font_size(20);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_OpenFont"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(file));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(20));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ttf_test_class::sdl_surface_copy(){
	array_list<sdl_surface> sfs;
	const char *str = "Hello";
	void *p;
	{
		sdlttf_font font(TRACE_PARAMS("", 66));
		sdl_color c(1,2,3,4);
		sdl_surface tsf(TRACE_PARAMS(str, sdlttf_font::solid, font, c));
		p = tsf._surface;
		sfs.add(tsf);
		CPPUNIT_ASSERT(!tsf._surface);
	}
	interface_testor::clear();
	sfs.clear();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_FreeSurface")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(p));
}

void ttf_test_class::rich_text_renderer_ass_change_color(){
	array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(255,255,255)));
	rnd.render_text("{\\1c&H00FFFC&}", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	interface_testor::clear();
	rnd.render_text("{\\1c&H00FFFC&}a", sfs);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(sdl_color(0xfc,0xff,0)));
	CPPUNIT_ASSERT(sfs.size()==1);
}

void ttf_test_class::rich_text_renderer_ass_change_size(){
	/*array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(255,255,255)));
	void *p = rnd._font->_font;
	interface_testor::clear();
	rnd.render_text("{\\fs16}", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_OpenFont"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(16));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("TTF_OpenFont"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(20));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(interface_testor::method(0).get_return()));
	
	interface_testor::clear();
	p = rnd._font->_font;
	rnd.render_text("{\\fs16}a", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_OpenFont"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(16));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("TTF_OpenFont"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(20));
	CPPUNIT_ASSERT(interface_testor::method(4).equal("TTF_CloseFont"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(interface_testor::method(0).get_return()));*/
}

void ttf_test_class::rich_text_renderer_srt_bold(){
	array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(255,255,255)));
	interface_testor::clear();
	rnd.render_text("<b>", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	sdlttf_font::font_style s;
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::bold));
	
	interface_testor::clear();
	rnd.render_text("<b>a", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::bold));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal("a"));
	
	sfs.clear();
	interface_testor::clear();
	rnd.render_text("<b>a</b>", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::bold));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(rnd._font->_font));
	interface_testor::method(2).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(4).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(5).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(1).equal(s&~sdlttf_font::bold));	
}

void ttf_test_class::rich_text_renderer_srt_italic(){
	array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(255,255,255)));
	interface_testor::clear();
	rnd.render_text("<i>", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	sdlttf_font::font_style s;
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::italic));
	
	interface_testor::clear();
	rnd.render_text("<i>a", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::italic));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal("a"));
	
	sfs.clear();
	interface_testor::clear();
	rnd.render_text("<i>a</i>", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::italic));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(rnd._font->_font));
	interface_testor::method(2).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(4).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(5).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(1).equal(s&~sdlttf_font::italic));
}

void ttf_test_class::rich_text_renderer_srt_underline(){
	array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(255,255,255)));
	interface_testor::clear();
	rnd.render_text("<u>", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	sdlttf_font::font_style s;
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::underline));
	
	interface_testor::clear();
	rnd.render_text("<u>a", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::underline));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal("a"));
	
	sfs.clear();
	interface_testor::clear();
	rnd.render_text("<u>a</u>", sfs);
	CPPUNIT_ASSERT(sfs.size()==1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnd._font->_font));
	interface_testor::method(0).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(s|sdlttf_font::underline));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("TTF_GetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(rnd._font->_font));
	interface_testor::method(2).get_return().get_value(s);
	CPPUNIT_ASSERT(interface_testor::method(4).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(5).equal("TTF_SetFontStyle"));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(1).equal(s&~sdlttf_font::underline));
}

void ttf_test_class::rich_text_renderer_srt_font_color(){
	array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(0,0,0)));
	rnd.render_text("<font color=\"#ff00ee\">", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(rnd._r_color.r==0xff);
	CPPUNIT_ASSERT(rnd._r_color.g==0);
	CPPUNIT_ASSERT(rnd._r_color.b==0xee);
	rnd.render_text("<font color='#112233'>", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(rnd._r_color.r==0x11);
	CPPUNIT_ASSERT(rnd._r_color.g==0x22);
	CPPUNIT_ASSERT(rnd._r_color.b==0x33);
	rnd.render_text("<font color=\"red\">", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(rnd._r_color.r==0xff);
	CPPUNIT_ASSERT(rnd._r_color.g==0);
	CPPUNIT_ASSERT(rnd._r_color.b==0);
	rnd.render_text("<font color='blue'>", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(rnd._r_color.r==0);
	CPPUNIT_ASSERT(rnd._r_color.g==0);
	CPPUNIT_ASSERT(rnd._r_color.b==0xff);
	rnd.render_text("</font>", sfs);
	CPPUNIT_ASSERT(sfs.size()==0);
	CPPUNIT_ASSERT(rnd._r_color.r==0);
	CPPUNIT_ASSERT(rnd._r_color.g==0);
	CPPUNIT_ASSERT(rnd._r_color.b==0);
	
	interface_testor::clear();
	rnd.render_text("<font color='blue'>a</font>", sfs);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(sdl_color(0,0,255)));
}

void ttf_test_class::rich_text_renderer_srt_br(){
	array_list<sdl_surface> sfs;
	rich_text_renderer rnd(TRACE_PARAMS("", 20, sdl_color(0,0,0)));
	interface_testor::clear();
	rnd.render_text("a<br>b", sfs);
	CPPUNIT_ASSERT(sfs.size()==2);
	CPPUNIT_ASSERT(interface_testor::method(1).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal("a"));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("TTF_RenderUTF8_Blended"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(rnd._font->_font));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal("b"));
}
