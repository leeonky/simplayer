#include <cstring>
#include "ccmedia/srt_decoder.h"
#include "cclibc/io/file.h"
#include "cclibc/text.h"
#include "ccapplib/string_util.h"

using CCLIBC::pool_ptr;
using CCLIBC::allocator;
using CCLIBC::file;
using CCLIBC::encoding;
using CCSDL2::sdl_color;
using CCSDL2::sdl_locked_txture;
using CCSDL2::sdl_surface;
using CCSDL2::sdl_rect;
using CCSDL2::sdl_texture;
using CCAPPLIB::time_bucket;
using CCAPPLIB::string_array;
using CCAPPLIB::string_util;

CCMEDIA_BG

srt_decoder::srt_decoder FUNCTION_PARAMETER_LIST(const char *f, size_t s, sdl_color c, float b)
	: _renderer(TRACE_PARAMS(f, s, c)), _bottom(b){
	_renderer.back_color();
}

void srt_stream::extend_content FUNCTION_PARAMETER_LIST(const char *s){
	string_array<> lines;
	string_util::split_line(s, lines);
	size_t l=0;
	while(l<lines.size()){
		int sh,eh,sm,em,ss,es,sms,ems;
		if(8 == sscanf(lines[l++], "%d:%d:%d,%d%*s%d:%d:%d,%d", &sh,&sm,&ss,&sms,&eh,&em,&es,&ems)){
			size_t st = sh*3600000 + sm*60000+ ss*1000 + sms;
			time_bucket t(st, eh*3600000 + em*60000+ es*1000 + ems - st);
			while(l<lines.size() && *lines[l]){
				extend_content(lines[l++], t);
			}
		}
	}
}

srt_stream::srt_stream FUNCTION_PARAMETER_LIST(srt_decoder &dec, const char *f, const char *cs)
	: _decoder(dec), _max_len(0), _last_event(NULL){
	size_t len;
	pool_ptr<allocator> ptr(file::read_all_text<allocator>(TRACE_PARAMS(f, len)));
	if(strcasecmp(cs, "UTF-8")){
		size_t buf_len = len*3/2+1;
		pool_ptr<allocator> buf_ptr(allocator::instance().allocate(buf_len));
		encoding::transform_format(cs, "UTF-8",
					ptr.raw_pointer(), len, buf_ptr.raw_pointer(), buf_len);
		extend_content(static_cast<char*>(buf_ptr.raw_pointer()));
	}else{
		extend_content(static_cast<char*>(ptr.raw_pointer()));
	}
}

srt_stream::srt_stream(srt_decoder &dec, char *str, size_t s): _decoder(dec), _max_len(0), _last_event(NULL){
	extend_content(str);
}

void srt_stream::extend_content FUNCTION_PARAMETER_LIST(const char *m, const time_bucket &t){
	size_t len;
	_contents[t].add(_pool.allocate(len, "%s", m));
	if(len > _max_len){
		_max_len = len;
	}
}

void srt_stream::render_frame FUNCTION_PARAMETER_LIST(sdl_locked_txture &lt, size_t t){
	if(!(_last_event && _last_event->key().contains(t))){
		if(_last_event){
			_last_imgs.clear();
		}
		if((_last_event = _contents.get_entry(time_bucket(t, 0)))){
			_decoder._renderer.reset_renderer();
			for(size_t i=0; i<_last_event->value().size(); i++){
				_decoder._renderer.render_text(_last_event->value()[i], _last_imgs);
			}
		}
	}
	int posy = lt.texture().height() * _decoder._bottom;
	for(int i=_last_imgs.size()-1; i>=0; --i){
		sdl_surface &sf = _last_imgs[i];
		posy-=sf.height();
		int posx = sf.width()> lt.texture().width() ? 0 : (lt.texture().width()-sf.width())/2;
		sdl_rect target_rect(posx, posy, sf.width(), sf.height());
		target_rect.intersect(lt.locked_rect());

		int start_dx = target_rect.x - lt.locked_rect().x;
		int start_dy = target_rect.y - lt.locked_rect().y;
		int start_sx = target_rect.x - posx;
		int start_sy = target_rect.y - posy;
		
		switch(lt.texture().pixel_format()){
		case sdl_texture::rgb24:
			for(uint8_t *img_line = static_cast<uint8_t*>(sf.pixel_data()) + start_sy*sf.line(),
						*last_img_line = static_cast<uint8_t*>(sf.pixel_data()) + target_rect.h*sf.line(),
						*buf_line = static_cast<uint8_t*>(lt.data()) + start_dy*lt.line();
				img_line<last_img_line; buf_line+=lt.line()){
				uint8_t *last_img_row = img_line+sf.line();
				for(uint8_t *img_row = img_line + start_sx*4,
							*buf_row = buf_line + start_dx*3; 
					img_row<last_img_row; img_row+=4, buf_row+=3){
					uint32_t k = img_row[3];
					if(k){
						buf_row[0] = (k*img_row[2] + (255-k)*buf_row[0])/255;
						buf_row[1] = (k*img_row[1] + (255-k)*buf_row[1])/255;
						buf_row[2] = (k*img_row[0] + (255-k)*buf_row[2])/255;
					}
				}
				img_line=last_img_row;
			}
			break;
		case sdl_texture::rgba32:
			for(uint8_t *img_line = static_cast<uint8_t*>(sf.pixel_data()) + start_sy*sf.line(),
						*last_img_line = static_cast<uint8_t*>(sf.pixel_data()) + target_rect.h*sf.line(),
						*buf_line = static_cast<uint8_t*>(lt.data()) + start_dy*lt.line();
				img_line<last_img_line; buf_line+=lt.line()){
				uint8_t *last_img_row = img_line+sf.line();
				for(uint8_t *img_row = img_line + start_sx*4,
							*buf_row = buf_line + start_dx*4; 
					img_row<last_img_row; img_row+=4, buf_row+=4){
					uint32_t k = img_row[3];
					if(k){
						buf_row[3] = (k*img_row[2] + (255-k)*buf_row[3])/255;
						buf_row[2] = (k*img_row[1] + (255-k)*buf_row[2])/255;
						buf_row[1] = (k*img_row[0] + (255-k)*buf_row[1])/255;
						buf_row[0] = k + (255-k)*buf_row[0];
					}
				}
				img_line=last_img_row;
			}
			break;
		case sdl_texture::yv12:
			{
				size_t cysize = lt.line() * lt.locked_rect().h;
				size_t uvsize = cysize/4;
				size_t uvline = lt.line()/2;
				for(uint8_t *img_line = static_cast<uint8_t*>(sf.pixel_data()) + start_sy*sf.line(),
							*last_img_line = static_cast<uint8_t*>(sf.pixel_data()) + target_rect.h*sf.line(),
							*buf_y_line = static_cast<uint8_t*>(lt.data()) + start_dy*lt.line(),
							*buf_u_line = static_cast<uint8_t*>(lt.data()) + cysize + (start_dy/2 * uvline),
							marky = start_dy;
					img_line<last_img_line; buf_y_line+=lt.line(), img_line+=sf.line(), ++marky){
					uint8_t *last_img_row = img_line+sf.line();
					for(uint8_t *img_row = img_line + start_sx*4,
								*buf_y_row = buf_y_line + start_dx,
								*buf_u_row = buf_u_line + start_dx/2,
								*buf_v_row = buf_u_row + uvsize,
								markx = start_dx;
						img_row<last_img_row; img_row+=4, ++buf_y_row, ++markx){
						uint32_t k = img_row[3];
						if(k){
							uint8_t r = img_row[2];
							uint8_t g = img_row[1];
							uint8_t b = img_row[0];
							*buf_y_row = (k*CCSDL2::rgb_2_y(r,g,b) + (255-k)*(*buf_y_row))/255;
							*buf_u_row = (k*CCSDL2::rgb_2_u(r,g,b) + (255-k)*(*buf_u_row))/255;
							*buf_v_row = (k*CCSDL2::rgb_2_v(r,g,b) + (255-k)*(*buf_v_row))/255;
						}
						if(markx&1){
							++buf_u_row;
							++buf_v_row;
						}
					}
					if(marky&1){
						buf_u_line+=uvline;
					}
				}
			}
			break;
		}
	}
}

CCMEDIA_END
