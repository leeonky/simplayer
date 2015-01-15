#include "ccmedia/ass_decoder.h"
#include "cclibc/callc.h"
#include "cclibc/io/file.h"
#include "cclibc/text.h"
#include "ccapplib/string_util.h"

using CCLIBC::pool_ptr;
using CCLIBC::allocator;
using CCLIBC::file;
using CCLIBC::pool_ptr;
using CCLIBC::allocator;
using CCLIBC::encoding;
using CCSDL2::sdl_locked_txture;
using CCSDL2::sdl_rect;
using CCSDL2::sdl_texture;
using CCAPPLIB::time_bucket;
using CCAPPLIB::string_array;
using CCAPPLIB::string_util;

CCMEDIA_BG

ass_decoder::ass_decoder FUNCTION_PARAMETER_LIST(size_t w, size_t h, size_t sw, size_t sh, const char *s, const char *fc, bool upfc)
	: _lib(INVOKE_THROW(ASS_Library*, == NULL, ass_decoder_error, ass_library_init)),
	_renderer(INVOKE_THROW(ASS_Renderer*, == NULL, ass_decoder_error, ass_renderer_init, _lib)),
	_video_rect((sw-w)/2, (sh-h)/2, w, h){
	ass_set_frame_size(_renderer, w, h);
	ass_set_fonts(_renderer, NULL, s, 1, fc, upfc);
}

ass_decoder::~ass_decoder(){
	ass_renderer_done(_renderer);
	ass_library_done(_lib);
}

/*
ass_stream::ass_stream FUNCTION_PARAMETER_LIST(ass_decoder &a, const char *f, const char *cs)
	: _track(INVOKE_THROW(ASS_Track*, == NULL, ass_decoder_error, ass_read_file, a._lib, const_cast<char*>(f), NULL)), _decoder(a){}
*/

ass_stream::ass_stream FUNCTION_PARAMETER_LIST(ass_decoder &a, const char *f, const char *cs)
	: _track(INVOKE_THROW(ASS_Track*, == NULL, ass_decoder_error, ass_new_track, a._lib)), _decoder(a){
	size_t len;
	pool_ptr<allocator> ptr(file::read_all_text<allocator>(TRACE_PARAMS(f, len)));
	if(strcasecmp(cs, "UTF-8")){
		size_t buf_len = len*3/2+1;
		pool_ptr<allocator> buf_ptr(allocator::instance().allocate(buf_len));
		len = encoding::transform_format(cs, "UTF-8",
					ptr.raw_pointer(), len, buf_ptr.raw_pointer(), buf_len);
		ass_process_codec_private(_track, static_cast<char*>(buf_ptr.raw_pointer()), len);
	}else{
		ass_process_codec_private(_track, static_cast<char*>(ptr.raw_pointer()), file::file_size(f));
	}
}

ass_stream::ass_stream FUNCTION_PARAMETER_LIST(ass_decoder &a, char *d, size_t s)
	: _track(INVOKE_THROW(ASS_Track*, == NULL, ass_decoder_error, ass_new_track, a._lib)), _decoder(a){
	ass_process_codec_private(_track, d, s);
}

ass_stream::~ass_stream(){
	ass_free_track(_track);
}

void ass_stream::render_frame FUNCTION_PARAMETER_LIST(sdl_locked_txture &lt, size_t ns){
	ASS_Image *img = ass_render_frame(_decoder._renderer, _track, ns, NULL);
	while(img){
		sdl_rect target_rect(img->dst_x, img->dst_y, img->w, img->h);
		target_rect.intersect(lt.locked_rect());
		target_rect.intersect(_decoder.video_rect());
		if(target_rect.w && target_rect.h){
			uint8_t r = img->color >> 24;
			uint8_t g = img->color >> 16;
			uint8_t b = img->color >> 8;
			uint8_t a = img->color;
			int start_dx = target_rect.x - lt.locked_rect().x + _decoder.video_rect().x/2;
			int start_dy = target_rect.y - lt.locked_rect().y + _decoder.video_rect().y/2;
			int start_sx = target_rect.x - img->dst_x;
			int start_sy = target_rect.y - img->dst_y;	
			switch(lt.texture().pixel_format()){
			case sdl_texture::rgb24:
				for(uint8_t *img_line = img->bitmap + start_sy*img->stride,
							*last_img_line = img->bitmap + target_rect.h*img->stride,
							*buf_line = static_cast<uint8_t*>(lt.data()) + start_dy*lt.line();
					img_line<last_img_line; buf_line+=lt.line()){
					uint8_t *last_img_row = img_line+img->stride;
					for(uint8_t *img_row = img_line + start_sx,
								*buf_row = buf_line + start_dx*3; 
						img_row<last_img_row; ++img_row, buf_row+=3){
						uint32_t k = (255 - a) * (*img_row) /255;
						if(k){
							buf_row[0] = (k*r + (255-k)*buf_row[0])/255;
							buf_row[1] = (k*g + (255-k)*buf_row[1])/255;
							buf_row[2] = (k*b + (255-k)*buf_row[2])/255;
						}
					}
					img_line=last_img_row;
				}
				break;
			case sdl_texture::rgba32:
				for(uint8_t *img_line = img->bitmap + start_sy*img->stride,
							*last_img_line = img->bitmap + target_rect.h*img->stride,
							*buf_line = static_cast<uint8_t*>(lt.data()) + start_dy*lt.line();
					img_line<last_img_line; buf_line+=lt.line()){
					uint8_t *last_img_row = img_line+img->stride;
					for(uint8_t *img_row = img_line + start_sx,
								*buf_row = buf_line + start_dx*4; 
						img_row<last_img_row; ++img_row, buf_row+=4){
						uint32_t k = (255 - a) * (*img_row) /255;
						if(k){
							buf_row[3] = (k*r + (255-k)*buf_row[3])/255;
							buf_row[2] = (k*g + (255-k)*buf_row[2])/255;
							buf_row[1] = (k*b + (255-k)*buf_row[1])/255;
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
					uint8_t cy = CCSDL2::rgb_2_y(r,g,b);
					uint8_t u = CCSDL2::rgb_2_u(r,g,b);
					uint8_t v = CCSDL2::rgb_2_v(r,g,b);
					for(uint8_t *img_line = img->bitmap + start_sy*img->stride,
								*last_img_line = img->bitmap + target_rect.h*img->stride,
								*buf_y_line = static_cast<uint8_t*>(lt.data()) + start_dy*lt.line(),
								*buf_u_line = static_cast<uint8_t*>(lt.data()) + cysize + (start_dy/2 * uvline),
								marky = start_dy;
						img_line<last_img_line; buf_y_line+=lt.line(), img_line+=img->stride, ++marky){
						uint8_t *last_img_row = img_line+img->stride;
						for(uint8_t *img_row = img_line + start_sx,
									*buf_y_row = buf_y_line + start_dx,
									*buf_u_row = buf_u_line + start_dx/2,
									*buf_v_row = buf_u_row + uvsize,
									markx = start_dx;
							img_row<last_img_row; ++img_row, ++buf_y_row, ++markx){
							uint32_t k = (255 - a) * (*img_row) /255;
							if(k){
								*buf_y_row = (k*cy + (255-k)*(*buf_y_row))/255;
								*buf_u_row = (k*u + (255-k)*(*buf_u_row))/255;
								*buf_v_row = (k*v + (255-k)*(*buf_v_row))/255;
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
		img=img->next;
	}
}

void ass_stream::extend_content FUNCTION_PARAMETER_LIST(const char *s, const time_bucket &t){
	if(strstr(s, "Dialogue:") == s){
		string_array<> lines;
		string_util::split_line(s, lines);
		char buf[1024];
		for(size_t l=0; l<lines.size(); l++){
			int sh, sm, eh, em;
			float ss, es;
			char layer[128];
			if(sscanf(lines[l], "%*[^:]:%[^,],%d:%d:%f,%d:%d:%f",
				layer, &sh, &sm, &ss, &eh,&em, &es) == 7){
				size_t st = sh*3600000 + sm*60000 + static_cast<size_t>(ss*1000);
				char *content = lines[l];
				content = strstr(content, ",") + 1;
				content = strstr(content, ",") + 1;
				content = strstr(content, ",") + 1;
				size_t len = snprintf(buf, sizeof(buf), "%d,%s,%s", _track->n_events, layer, content) - 1;
				ass_process_chunk(_track, buf, len, st, eh*3600000 + em*60000 + static_cast<size_t>(es*1000) - st);
			}
		}
	}else{
		size_t len = strlen(s);
		CCLIBC::pool_ptr<CCLIBC::allocator> buf_ptr(CCLIBC::allocator::instance().allocate(len + 1));
		memcpy(buf_ptr.raw_pointer(), s, len + 1);
		ass_process_chunk(_track, static_cast<char*>(buf_ptr.raw_pointer()), len, t.start(), t.end()-t.start());
	}
}

CCMEDIA_END
