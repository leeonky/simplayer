#ifndef CCSDL2_VIDEO_H_
#define CCSDL2_VIDEO_H_

#include <SDL2/SDL.h>
#include "ccsdl2/env.h"
#include "ccsdl2/sdl.h"

CCSDL2_BG

class sdl_video : private sdl_base{
protected:
	sdl_video FUNCTION_PARAMETER_LIST();
};

class sdl_renderer;
class sdl_window: private sdl_video{
friend class sdl_renderer;
private:
	SDL_Window *_wnd;
public:
	enum style{
		none = 0,
		fullscreen = SDL_WINDOW_FULLSCREEN,
		fullscreen_desktop  = SDL_WINDOW_FULLSCREEN_DESKTOP,
		openGL = SDL_WINDOW_OPENGL,
		hidden = SDL_WINDOW_HIDDEN,
		borderless = SDL_WINDOW_BORDERLESS,
		resizable = SDL_WINDOW_RESIZABLE,
		minimized = SDL_WINDOW_MINIMIZED,
		maximized = SDL_WINDOW_MAXIMIZED,
		input_grabbed = SDL_WINDOW_INPUT_GRABBED,
		highdpi = SDL_WINDOW_ALLOW_HIGHDPI,
	};
	sdl_window FUNCTION_PARAMETER_LIST(const char*, size_t, size_t, size_t, size_t, style);

	~sdl_window(){
		SDL_DestroyWindow(_wnd);
		_wnd = NULL;
	}

	size_t width(){
		int w,h;
		SDL_GetWindowSize(_wnd, &w, &h);
		return w;
	}

	size_t height(){
		int w,h;
		SDL_GetWindowSize(_wnd, &w, &h);
		return h;
	}

	void size(size_t w, size_t h){
		SDL_SetWindowSize(_wnd, w, h);
	}

	int x(){
		int _x,_y;
		SDL_GetWindowPosition(_wnd, &_x, &_y);
		return _x;
	}

	int y(){
		int _x,_y;
		SDL_GetWindowPosition(_wnd, &_x, &_y);
		return _y;
	}

	void position(int x, int y){
		SDL_SetWindowPosition(_wnd, x, y);
	}
};

class sdl_texture;
class sdl_rect;
class sdl_renderer{
friend class sdl_texture;
private:
	SDL_Renderer *_renderer;
public:
	enum style{
		none = 0,
		software = SDL_RENDERER_SOFTWARE,
		accelerated = SDL_RENDERER_ACCELERATED,
		presentvsunc = SDL_RENDERER_PRESENTVSYNC,
		targettexture = SDL_RENDERER_TARGETTEXTURE,
	};
	
	enum blend_mode{
		copy = SDL_BLENDMODE_NONE,
		blend = SDL_BLENDMODE_BLEND,
		add = SDL_BLENDMODE_ADD,
		mod = SDL_BLENDMODE_MOD,
	};
	sdl_renderer FUNCTION_PARAMETER_LIST(sdl_window&, style s = none, int i = -1);

	~sdl_renderer(){
		SDL_DestroyRenderer(_renderer);
		_renderer = NULL;
	}

	void copy_texture FUNCTION_PARAMETER_LIST(sdl_texture&);
	void copy_texture FUNCTION_PARAMETER_LIST(const sdl_rect&, sdl_texture&);
	void copy_texture FUNCTION_PARAMETER_LIST(sdl_texture&, const sdl_rect &);
	void copy_texture FUNCTION_PARAMETER_LIST(const sdl_rect&, sdl_texture&, const sdl_rect&);
	
	void present(){
		SDL_RenderPresent(_renderer);
	}
};

class sdl_rect: public SDL_Rect{
public:
	sdl_rect(int _x, int _y, size_t _w, int _h) {
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	sdl_rect() {}
	bool contains(int _x, int _y) const{
		return _x>=x && _x<x+w && _y>=y && _y<y+h;
	}
	void intersect(const sdl_rect&);
};

class sdl_surface;
class sdl_texture{
friend class sdl_renderer;
public:
	enum format{
		yv12 = SDL_PIXELFORMAT_YV12,
		rgba32 = SDL_PIXELFORMAT_RGBA8888,	/*32bit, R:0xff000000, G:0xff0000, B:0xff00, A:0xff*/
		rgb24 = SDL_PIXELFORMAT_RGB24,		/*24bit, R:0xff, G:0xff00, B:0xff0000, A:0x0*/
	};
	
	enum access{
		statical = SDL_TEXTUREACCESS_STATIC,
		streaming = SDL_TEXTUREACCESS_STREAMING,
		target = SDL_TEXTUREACCESS_TARGET,
	};

	sdl_texture FUNCTION_PARAMETER_LIST(sdl_renderer&, format, access, size_t, size_t, sdl_renderer::blend_mode b = sdl_renderer::blend);
	sdl_texture FUNCTION_PARAMETER_LIST(sdl_renderer&, sdl_surface&);

	~sdl_texture(){
		SDL_DestroyTexture(_texture);
	}

	size_t width() const{
		return _w;
	}

	size_t height() const{
		return _h;
	}
	
	format pixel_format() const{
		return _f;
	}

	void lock FUNCTION_PARAMETER_LIST(void *&, size_t&);

	void lock FUNCTION_PARAMETER_LIST(const sdl_rect&, void *&, size_t&);

	void unlock FUNCTION_PARAMETER_LIST();
	
	void update_pixel FUNCTION_PARAMETER_LIST(const void*, int);
	void update_pixel FUNCTION_PARAMETER_LIST(const sdl_rect&, const void*, int);
	void update_pixel FUNCTION_PARAMETER_LIST(const void*, int, const void*, int, const void*, int);
	void update_pixel FUNCTION_PARAMETER_LIST(const sdl_rect&, const void*, int, const void*, int, const void*, int);
private:
	SDL_Texture *_texture;
	size_t _w, _h;
	format _f;
};

class sdl_color: public SDL_Color{
public:
	sdl_color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255){
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}
	
	sdl_color& operator=(const sdl_color &c){
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;
		return *this;
	}
	
	sdl_color(){
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}
};

class sdl_locked_txture{
private:
	sdl_rect _rect;
	sdl_texture &_texture;
	void *_data;
	size_t _line;
public:
	sdl_locked_txture FUNCTION_PARAMETER_LIST(sdl_texture&, const sdl_rect&);
	sdl_locked_txture FUNCTION_PARAMETER_LIST(sdl_texture&);
	~sdl_locked_txture();

	const sdl_rect& locked_rect() const{
		return _rect;
	}

	void* data() {
		return _data;
	}

	size_t line() {
		return _line;
	}

	sdl_texture& texture() {
		return _texture;
	}
	void update_texture FUNCTION_PARAMETER_LIST();
};

#ifdef STACK_TRACE_FLAG

#ifndef lock
#define lock(...) lock(INVOKE_TRACE, ##__VA_ARGS__)
#endif
#ifndef unlock
#define unlock(...) unlock(INVOKE_TRACE, ##__VA_ARGS__)
#endif

#define copy_texture(...) copy_texture(INVOKE_TRACE, ##__VA_ARGS__)
#define update_pixel(...) update_pixel(INVOKE_TRACE, ##__VA_ARGS__)
#define update_texture(...) update_texture(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCSDL2_END

inline bool operator == (const CCSDL2::sdl_rect &r1, const CCSDL2::sdl_rect &r2) {
	return r1.x == r2.x && r1.y == r2.y && r1.w == r2.w && r1.h == r2.h;
}

#endif //CCSDL2_VIDEO_H_

