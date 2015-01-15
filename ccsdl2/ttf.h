#ifndef CCSDL2_TTF_H_
#define CCSDL2_TTF_H_

#include <SDL2/SDL_ttf.h>
#include "ccsdl2/env.h"
#include "ccsdl2/video.h"
#include "cclibc/util/array_list.h"
#include "cclibc/util/map.h"

CCSDL2_BG

class sdlttf_error: public CCLIBC::exception{
private:
	const char *_call;
public:
	sdlttf_error(const char*);
};

class sdlttf_lib{
public:
	sdlttf_lib FUNCTION_PARAMETER_LIST();
	~sdlttf_lib(){
		TTF_Quit();
	}
};

class sdl_surface;
class sdlttf_font{
friend class sdl_surface;
private:
	TTF_Font *_font;
public:

	enum render_style{
		solid,
		shaded,
		blended,
	};
	
	enum font_style{
		normal = TTF_STYLE_NORMAL,
		bold = TTF_STYLE_BOLD,
		italic = TTF_STYLE_ITALIC,
		underline = TTF_STYLE_UNDERLINE,
		strikethrough = TTF_STYLE_STRIKETHROUGH,
	};

	sdlttf_font FUNCTION_PARAMETER_LIST(const char*, size_t);
	~sdlttf_font(){
		TTF_CloseFont(_font);
		_font = NULL;
	}
	sdlttf_font& outline(size_t l){
		TTF_SetFontOutline(_font, l);
		return *this;
	}

	size_t outline() const{
		return TTF_GetFontOutline(_font);
	}
	
	void text_size FUNCTION_PARAMETER_LIST(const char*, size_t&, size_t&);
	
	size_t skip_line_height() const{
		return TTF_FontLineSkip(_font);
	}
	
	font_style style(){
		return static_cast<font_style>(TTF_GetFontStyle(_font));
	}
	
	sdlttf_font& style(font_style s){
		TTF_SetFontStyle(_font, s);
		return *this;
	}
};

inline sdlttf_font::font_style operator| (sdlttf_font::font_style s1, sdlttf_font::font_style s2){
		return static_cast<sdlttf_font::font_style>(static_cast<int>(s1)|static_cast<int>(s2));
}

/* Image format: SDL_PIXELFORMAT_ARGB8888, 32bit, R:0xff0000, G:0xff00, B:0xff, A:0xff000000 */
class sdl_surface{
friend class sdl_texture;
private:
	mutable SDL_Surface *_surface;
public:
	sdl_surface FUNCTION_PARAMETER_LIST(const char*, sdlttf_font::render_style, sdlttf_font&, const sdl_color&);
	sdl_surface FUNCTION_PARAMETER_LIST(const char*, sdlttf_font&, const sdl_color&, const sdl_color&);
	sdl_surface FUNCTION_PARAMETER_LIST(const uint16_t*, sdlttf_font::render_style, sdlttf_font&, const sdl_color&);
	~sdl_surface(){
		if(_surface){
			SDL_FreeSurface(_surface);
			_surface = NULL;
		}
	}
	void blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode, sdl_surface&);
	void blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode, const sdl_rect&, sdl_surface&);
	void blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode, sdl_surface&, sdl_rect&);
	void blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode, const sdl_rect&, sdl_surface&, sdl_rect&);
	
	size_t width(){
		return _surface->w;
	}

	size_t height(){
		return _surface->h;
	}

	sdl_surface(const sdl_surface&);
	
	void* pixel_data(){
		return _surface->pixels;
	}
	
	size_t line(){
		return _surface->pitch;
	}
};

class rich_text_renderer{
private:
	CCLIBC::object_ptr<sdlttf_font> _font;
	CCLIBC::pool_ptr<CCLIBC::allocator> _font_file;
	size_t _size, _r_size;
	sdl_color _color, _r_color;
	sdlttf_font::render_style _style;
	sdl_color _bk_color;
	float _bk_width;
	CCLIBC::map<const char*, sdl_color> _colors;
public:
	rich_text_renderer FUNCTION_PARAMETER_LIST(const char*, size_t, sdl_color, sdlttf_font::render_style st= sdlttf_font::blended);
	void font_size FUNCTION_PARAMETER_LIST(size_t);
	void render_text FUNCTION_PARAMETER_LIST(const char*, CCLIBC::array_list<sdl_surface>&);
	void reset_renderer FUNCTION_PARAMETER_LIST();
	void back_color(float w = 0.1, sdl_color c = sdl_color(20, 20, 20));
};

#ifdef STACK_TRACE_FLAG

#define blit_to(...) blit_to(INVOKE_TRACE, ##__VA_ARGS__)
#define text_size(...) text_size(INVOKE_TRACE, ##__VA_ARGS__)
#define font_size(...) font_size(INVOKE_TRACE, ##__VA_ARGS__)
#define render_text(...) render_text(INVOKE_TRACE, ##__VA_ARGS__)
#define reset_renderer(...) reset_renderer(INVOKE_TRACE, ##__VA_ARGS__)


#endif

CCSDL2_END

#endif //CCSDL2_TTF_H_
