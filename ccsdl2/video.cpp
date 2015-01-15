#include <SDL2/SDL.h>
#include "ccsdl2/video.h"
#include "ccsdl2/ttf.h"

using CCLIBC::util;

namespace{
class sdl_video_system{
public:
	sdl_video_system FUNCTION_PARAMETER_LIST(){
		VOID_INVOKE_THROW(!=0, CCSDL2::sdl_error, SDL_InitSubSystem, SDL_INIT_VIDEO);
	}
	~sdl_video_system(){
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}
};
}

CCSDL2_BG

sdl_video::sdl_video FUNCTION_PARAMETER_LIST() : sdl_base(TRACE_PARAMS()){
	static sdl_video_system TRACE_CONSTRUCT(_video_sys);
}

sdl_window::sdl_window FUNCTION_PARAMETER_LIST(const char *t, size_t x, size_t y, size_t w, size_t h, style s)
	:sdl_video(TRACE_PARAMS()), _wnd(NULL){
	if(!(_wnd = SDL_CreateWindow(t, x, y, w, h, static_cast<uint32_t>(s)))){
		THROW_NEW_EXCEPTION(sdl_error, "SDL_CreateWindow");
	}
}

sdl_renderer::sdl_renderer FUNCTION_PARAMETER_LIST(sdl_window &wnd, style s, int i): _renderer(NULL) {
	if(!(_renderer = SDL_CreateRenderer(wnd._wnd, i, static_cast<uint32_t>(s)))){
		THROW_NEW_EXCEPTION(sdl_error, "SDL_CreateRenderer");
	}
}

void sdl_renderer::copy_texture FUNCTION_PARAMETER_LIST(sdl_texture &t){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_RenderCopy, _renderer, t._texture, NULL, NULL);
}

void sdl_renderer::copy_texture FUNCTION_PARAMETER_LIST(const sdl_rect &d, sdl_texture &t){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_RenderCopy, _renderer, t._texture, NULL, &d);
}

void sdl_renderer::copy_texture FUNCTION_PARAMETER_LIST(sdl_texture &t, const sdl_rect &s){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_RenderCopy, _renderer, t._texture, &s, NULL);
}

void sdl_renderer::copy_texture FUNCTION_PARAMETER_LIST(const sdl_rect &d, sdl_texture &t, const sdl_rect &s){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_RenderCopy, _renderer, t._texture, &s, &d);
}

sdl_texture::sdl_texture FUNCTION_PARAMETER_LIST(sdl_renderer &rnd, format f, access a, size_t w, size_t h, sdl_renderer::blend_mode bm)
	: _texture(INVOKE_THROW(SDL_Texture*, == NULL, sdl_error, SDL_CreateTexture, rnd._renderer, static_cast<uint32_t>(f), static_cast<uint32_t>(a), w, h)){
	try{
		int i1,i2,i3;
		uint32_t i4;
		VOID_INVOKE_THROW(!=0, sdl_error, SDL_QueryTexture, _texture, &i4, &i1, &i2, &i3);
		_w = i2;
		_h = i3;
		_f = static_cast<format>(i4);
		VOID_INVOKE_THROW(!=0, sdl_error, SDL_SetTextureBlendMode, _texture, static_cast<SDL_BlendMode>(bm));
	}catch(...){
		SDL_DestroyTexture(_texture);
		throw;
	}
}

void sdl_rect::intersect(const sdl_rect &r){
#if 0
	if(this != &r){
		int tx, ty;
		if(contains(r.x, r.y)){
			w=std::min(r.w, w-(r.x-x));
			h=std::min(r.h, h-(r.y-y));
			x=r.x;
			y=r.y;
		}else if(contains(tx=(r.x+r.w-1), r.y)){
			x=std::max(r.x, x);
			h=std::min(r.h, h-(r.y-y));
			y=r.y;
			w=tx-x+1;
		}else if(contains(r.x, ty=r.y+r.h-1)){
			w=std::min(r.w, w-(r.x-x));
			x=r.x;
			y=std::max(r.y, y);
			h=ty-y+1;
		}else if(contains(tx=(r.x+r.w-1), ty=r.y+r.h-1)){
			x=std::max(r.x, x);
			y=std::max(r.y, y);
			w=tx-x+1;
			h=ty-y+1;
		}else if(!r.contains(x,y)){
			w=h=0;
		}
	}
#endif 
	if(this != &r){
		if(r.x >= x){
			w = x+w - r.x;
			if(w>0){
				x=r.x;
				util::update_min(w, r.w);
				if(r.y >= y){
					h = std::min(y+h - r.y, r.h);
					if(h >0){
						y=r.y;
						return;
					}
				}else{
					util::update_min(h, r.y+r.h-y);
					if(h >0){
						return;
					}
				}
			}
		}else{
			util::update_min(w, r.x+r.w - x);
			if(w>0){
				if(r.y >= y){
					h = std::min(y+h - r.y, r.h);
					if(h>0){
						y=r.y;
						return;
					}
				}else{
					util::update_min(h, r.y+r.h-y);
					if(h>0){
						return;
					}
				}
			}
		}
		w=0;
		h=0;
	}
}

sdl_texture::sdl_texture FUNCTION_PARAMETER_LIST(sdl_renderer &rnd, sdl_surface &s)
	: _texture(INVOKE_THROW(SDL_Texture*, == NULL, sdl_error, SDL_CreateTextureFromSurface, rnd._renderer, s._surface)){
	try{
		int i1,i2,i3;
		uint32_t i4;
		VOID_INVOKE_THROW(!=0, sdl_error, SDL_QueryTexture, _texture, &i4, &i1, &i2, &i3);
		_w = i2;
		_h = i3;
		_f = static_cast<format>(i4);
	}catch(...){
		SDL_DestroyTexture(_texture);
		throw;
	}
}

void sdl_texture::lock FUNCTION_PARAMETER_LIST(void *&p, size_t &l){
	int ti;
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_LockTexture, _texture, NULL, &p, &ti);
	l = ti;
}

void sdl_texture::lock FUNCTION_PARAMETER_LIST(const sdl_rect &r, void *&p, size_t &l){
	int ti;
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_LockTexture, _texture, &r, &p, &ti);
	l = ti;
}

void sdl_texture::unlock FUNCTION_PARAMETER_LIST(){
	SDL_UnlockTexture(_texture);
}

void sdl_texture::update_pixel FUNCTION_PARAMETER_LIST(const void *p, int l){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_UpdateTexture, _texture, NULL, p, l);
}
void sdl_texture::update_pixel FUNCTION_PARAMETER_LIST(const sdl_rect &r, const void *p, int l){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_UpdateTexture, _texture, &r, p, l);
}
void sdl_texture::update_pixel FUNCTION_PARAMETER_LIST(const void *y, int ly,
	const void *u, int lu, const void *v, int lv){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_UpdateYUVTexture, _texture, NULL,
		static_cast<const uint8_t*>(y), ly, static_cast<const uint8_t*>(u), lu, static_cast<const uint8_t*>(v), lv);
}
void sdl_texture::update_pixel FUNCTION_PARAMETER_LIST(const sdl_rect &r,
	const void *y, int ly, const void *u, int lu, const void *v, int lv){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_UpdateYUVTexture, _texture, &r,
		static_cast<const uint8_t*>(y), ly, static_cast<const uint8_t*>(u), lu, static_cast<const uint8_t*>(v), lv);
}

sdl_locked_txture::sdl_locked_txture FUNCTION_PARAMETER_LIST(sdl_texture &t, const sdl_rect &r): _rect(r), _texture(t){
	_texture.lock(_rect, _data, _line);
}

sdl_locked_txture::sdl_locked_txture FUNCTION_PARAMETER_LIST(sdl_texture &t): _rect(0, 0, t.width(), t.height()), _texture(t){
	_texture.lock(_rect, _data, _line);
}

sdl_locked_txture::~sdl_locked_txture(){
	_texture.unlock();
}

void sdl_locked_txture::update_texture FUNCTION_PARAMETER_LIST(){
	_texture.unlock();
	_texture.lock(_rect, _data, _line);
}

CCSDL2_END

