#include <cstring>
#include "ccsdl2/ttf.h"
#include "cclibc/memory.h"

using CCLIBC::pool_ptr;
using CCLIBC::allocator;
using CCLIBC::array_list;
using CCLIBC::map;

CCSDL2_BG

sdlttf_error::sdlttf_error(const char *call): exception("%s: SDL_ttf error: %s", call, TTF_GetError()), _call(call){}

sdlttf_lib::sdlttf_lib FUNCTION_PARAMETER_LIST(){
	VOID_INVOKE_THROW(!=0, sdl_error, TTF_Init);
}

sdlttf_font::sdlttf_font FUNCTION_PARAMETER_LIST(const char *f, size_t s): _font(NULL){
	static sdlttf_lib TRACE_CONSTRUCT(_sdlttf);
	if(!(_font = TTF_OpenFont(f, s))){
		THROW_NEW_EXCEPTION(sdlttf_error, "TTF_OpenFont");
	}
}

void sdlttf_font::text_size FUNCTION_PARAMETER_LIST(const char *t, size_t &w, size_t &h){
	int _w, _h;
	VOID_INVOKE_THROW(!=0, sdl_error, TTF_SizeUTF8, _font, t, &_w, &_h);
	w = _w;
	h = _h;
}

sdl_surface::sdl_surface FUNCTION_PARAMETER_LIST(const char *t, sdlttf_font::render_style s, sdlttf_font &f, const sdl_color &c){
	const char *method;
	switch(s){
		case sdlttf_font::solid:
			_surface = TTF_RenderUTF8_Solid(f._font, t, c);
			method = "TTF_RenderUTF8_Solid";
			break;
		case sdlttf_font::shaded:
			_surface = TTF_RenderUTF8_Shaded(f._font, t, c, sdl_color(0,0,0,0));
			method = "TTF_RenderUTF8_Shaded";
			break;
		case sdlttf_font::blended:
			_surface = TTF_RenderUTF8_Blended(f._font, t, c);
			method = "TTF_RenderUTF8_Blended";
			break;
	}
	if(!_surface){
		THROW_NEW_EXCEPTION(sdlttf_error, method);
	}
}

sdl_surface::sdl_surface FUNCTION_PARAMETER_LIST(const char *t, sdlttf_font &f, const sdl_color &c, const sdl_color &b){
	if(!(_surface = TTF_RenderUTF8_Shaded(f._font, t, c, b))){
		THROW_NEW_EXCEPTION(sdlttf_error, "TTF_RenderUTF8_Shaded");
	}
}

sdl_surface::sdl_surface FUNCTION_PARAMETER_LIST(const uint16_t *t, sdlttf_font::render_style s, sdlttf_font &f, const sdl_color &c){
	const char *method;
	switch(s){
		case sdlttf_font::solid:
			_surface = TTF_RenderUNICODE_Solid(f._font, t, c);
			method = "TTF_RenderUNICODE_Solid";
			break;
		case sdlttf_font::shaded:
			_surface = TTF_RenderUNICODE_Shaded(f._font, t, c, sdl_color(0,0,0,0));
			method = "TTF_RenderUNICODE_Shaded";
			break;
		case sdlttf_font::blended:
			_surface = TTF_RenderUNICODE_Blended(f._font, t, c);
			method = "TTF_RenderUNICODE_Blended";
			break;
	}
	if(!_surface){
		THROW_NEW_EXCEPTION(sdlttf_error, method);
	}
}

void sdl_surface::blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode mode, sdl_surface &dst){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_SetSurfaceBlendMode, _surface, static_cast<SDL_BlendMode>(mode));
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_BlitSurface, _surface, NULL, dst._surface, NULL);
}

void sdl_surface::blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode mode, const sdl_rect &s, sdl_surface &dst){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_SetSurfaceBlendMode, _surface, static_cast<SDL_BlendMode>(mode));
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_BlitSurface, _surface, &s, dst._surface, NULL);
}

void sdl_surface::blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode mode, sdl_surface &dst, sdl_rect &d){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_SetSurfaceBlendMode, _surface, static_cast<SDL_BlendMode>(mode));
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_BlitSurface, _surface, NULL, dst._surface, &d);
}

void sdl_surface::blit_to FUNCTION_PARAMETER_LIST(sdl_renderer::blend_mode mode, const sdl_rect &s, sdl_surface &dst, sdl_rect &d){
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_SetSurfaceBlendMode, _surface, static_cast<SDL_BlendMode>(mode));
	VOID_INVOKE_THROW(!=0, sdl_error, SDL_BlitSurface, _surface, &s, dst._surface, &d);
}

sdl_surface::sdl_surface(const sdl_surface &s):_surface(s._surface){
	s._surface = NULL;
}

rich_text_renderer::rich_text_renderer FUNCTION_PARAMETER_LIST(const char *f, size_t s, sdl_color c, sdlttf_font::render_style st):
	_font(OBJECT_NEW(sdlttf_font, TRACE_PARAMS(f, s))), _font_file(CCLIBC::allocator::instance().allocate("%s", f)),
	_size(s), _r_size(s), _color(c), _r_color(c), _style(st), _bk_width(0){
		_colors.add("aliceblue", sdl_color(0xF0, 0xF8, 0xFF));
		_colors.add("antiquewhite", sdl_color(0xFA, 0xEB, 0xD7));
		_colors.add("aqua", sdl_color(0x00, 0xFF, 0xFF));
		_colors.add("aquamarine", sdl_color(0x7F, 0xFF, 0xD4));
		_colors.add("azure", sdl_color(0xF0, 0xFF, 0xFF));
		_colors.add("beige", sdl_color(0xF5, 0xF5, 0xDC));
		_colors.add("bisque", sdl_color(0xFF, 0xE4, 0xC4));
		_colors.add("black", sdl_color(0x00, 0x00, 0x00));
		_colors.add("blanchedalmond", sdl_color(0xFF, 0xEB, 0xCD));
		_colors.add("blue", sdl_color(0x00, 0x00, 0xFF));
		_colors.add("blueviolet", sdl_color(0x8A, 0x2B, 0xE2));
		_colors.add("brown", sdl_color(0xA5, 0x2A, 0x2A));
		_colors.add("burlywood", sdl_color(0xDE, 0xB8, 0x87));
		_colors.add("cadetblue", sdl_color(0x5F, 0x9E, 0xA0));
		_colors.add("chartreuse", sdl_color(0x7F, 0xFF, 0x00));
		_colors.add("chocolate", sdl_color(0xD2, 0x69, 0x1E));
		_colors.add("coral", sdl_color(0xFF, 0x7F, 0x50));
		_colors.add("cornflowerblue", sdl_color(0x64, 0x95, 0xED));
		_colors.add("cornsilk", sdl_color(0xFF, 0xF8, 0xDC));
		_colors.add("crimson", sdl_color(0xDC, 0x14, 0x3C));
		_colors.add("cyan", sdl_color(0x00, 0xFF, 0xFF));
		_colors.add("darkblue", sdl_color(0x00, 0x00, 0x8B));
		_colors.add("darkcyan", sdl_color(0x00, 0x8B, 0x8B));
		_colors.add("darkgoldenrod", sdl_color(0xB8, 0x86, 0x0B));
		_colors.add("darkgray", sdl_color(0xA9, 0xA9, 0xA9));
		_colors.add("darkgreen", sdl_color(0x00, 0x64, 0x00));
		_colors.add("darkkhaki", sdl_color(0xBD, 0xB7, 0x6B));
		_colors.add("darkmagenta", sdl_color(0x8B, 0x00, 0x8B));
		_colors.add("darkolivegreen", sdl_color(0x55, 0x6B, 0x2F));
		_colors.add("darkorange", sdl_color(0xFF, 0x8C, 0x00));
		_colors.add("darkorchid", sdl_color(0x99, 0x32, 0xCC));
		_colors.add("darkred", sdl_color(0x8B, 0x00, 0x00));
		_colors.add("darksalmon", sdl_color(0xE9, 0x96, 0x7A));
		_colors.add("darkseagreen", sdl_color(0x8F, 0xBC, 0x8F));
		_colors.add("darkslateblue", sdl_color(0x48, 0x3D, 0x8B));
		_colors.add("darkslategray", sdl_color(0x2F, 0x4F, 0x4F));
		_colors.add("darkturquoise", sdl_color(0x00, 0xCE, 0xD1));
		_colors.add("darkviolet", sdl_color(0x94, 0x00, 0xD3));
		_colors.add("deeppink", sdl_color(0xFF, 0x14, 0x93));
		_colors.add("deepskyblue", sdl_color(0x00, 0xBF, 0xFF));
		_colors.add("dimgray", sdl_color(0x69, 0x69, 0x69));
		_colors.add("dodgerblue", sdl_color(0x1E, 0x90, 0xFF));
		_colors.add("firebrick", sdl_color(0xB2, 0x22, 0x22));
		_colors.add("floralwhite", sdl_color(0xFF, 0xFA, 0xF0));
		_colors.add("forestgreen", sdl_color(0x22, 0x8B, 0x22));
		_colors.add("fuchsia", sdl_color(0xFF, 0x00, 0xFF));
		_colors.add("gainsboro", sdl_color(0xDC, 0xDC, 0xDC));
		_colors.add("ghostwhite", sdl_color(0xF8, 0xF8, 0xFF));
		_colors.add("gold", sdl_color(0xFF, 0xD7, 0x00));
		_colors.add("goldenrod", sdl_color(0xDA, 0xA5, 0x20));
		_colors.add("gray", sdl_color(0x80, 0x80, 0x80));
		_colors.add("green", sdl_color(0x00, 0x80, 0x00));
		_colors.add("greenyellow", sdl_color(0xAD, 0xFF, 0x2F));
		_colors.add("honeydew", sdl_color(0xF0, 0xFF, 0xF0));
		_colors.add("hotpink", sdl_color(0xFF, 0x69, 0xB4));
		_colors.add("indianred ", sdl_color(0xCD, 0x5C, 0x5C));
		_colors.add("indigo ", sdl_color(0x4B, 0x00, 0x82));
		_colors.add("ivory", sdl_color(0xFF, 0xFF, 0xF0));
		_colors.add("khaki", sdl_color(0xF0, 0xE6, 0x8C));
		_colors.add("lavender", sdl_color(0xE6, 0xE6, 0xFA));
		_colors.add("lavenderblush", sdl_color(0xFF, 0xF0, 0xF5));
		_colors.add("lawngreen", sdl_color(0x7C, 0xFC, 0x00));
		_colors.add("lemonchiffon", sdl_color(0xFF, 0xFA, 0xCD));
		_colors.add("lightblue", sdl_color(0xAD, 0xD8, 0xE6));
		_colors.add("lightcoral", sdl_color(0xF0, 0x80, 0x80));
		_colors.add("lightcyan", sdl_color(0xE0, 0xFF, 0xFF));
		_colors.add("lightgoldenrodyellow", sdl_color(0xFA, 0xFA, 0xD2));
		_colors.add("lightgray", sdl_color(0xD3, 0xD3, 0xD3));
		_colors.add("lightgreen", sdl_color(0x90, 0xEE, 0x90));
		_colors.add("lightpink", sdl_color(0xFF, 0xB6, 0xC1));
		_colors.add("lightsalmon", sdl_color(0xFF, 0xA0, 0x7A));
		_colors.add("lightseagreen", sdl_color(0x20, 0xB2, 0xAA));
		_colors.add("lightskyblue", sdl_color(0x87, 0xCE, 0xFA));
		_colors.add("lightslategray", sdl_color(0x77, 0x88, 0x99));
		_colors.add("lightsteelblue", sdl_color(0xB0, 0xC4, 0xDE));
		_colors.add("lightyellow", sdl_color(0xFF, 0xFF, 0xE0));
		_colors.add("lime", sdl_color(0x00, 0xFF, 0x00));
		_colors.add("limegreen", sdl_color(0x32, 0xCD, 0x32));
		_colors.add("linen", sdl_color(0xFA, 0xF0, 0xE6));
		_colors.add("magenta", sdl_color(0xFF, 0x00, 0xFF));
		_colors.add("maroon", sdl_color(0x80, 0x00, 0x00));
		_colors.add("mediumaquamarine", sdl_color(0x66, 0xCD, 0xAA));
		_colors.add("mediumblue", sdl_color(0x00, 0x00, 0xCD));
		_colors.add("mediumorchid", sdl_color(0xBA, 0x55, 0xD3));
		_colors.add("mediumpurple", sdl_color(0x93, 0x70, 0xDB));
		_colors.add("mediumseagreen", sdl_color(0x3C, 0xB3, 0x71));
		_colors.add("mediumslateblue", sdl_color(0x7B, 0x68, 0xEE));
		_colors.add("mediumspringgreen", sdl_color(0x00, 0xFA, 0x9A));
		_colors.add("mediumturquoise", sdl_color(0x48, 0xD1, 0xCC));
		_colors.add("mediumvioletred", sdl_color(0xC7, 0x15, 0x85));
		_colors.add("midnightblue", sdl_color(0x19, 0x19, 0x70));
		_colors.add("mintcream", sdl_color(0xF5, 0xFF, 0xFA));
		_colors.add("mistyrose", sdl_color(0xFF, 0xE4, 0xE1));
		_colors.add("moccasin", sdl_color(0xFF, 0xE4, 0xB5));
		_colors.add("navajowhite", sdl_color(0xFF, 0xDE, 0xAD));
		_colors.add("navy", sdl_color(0x00, 0x00, 0x80));
		_colors.add("oldlace", sdl_color(0xFD, 0xF5, 0xE6));
		_colors.add("olive", sdl_color(0x80, 0x80, 0x00));
		_colors.add("olivedrab", sdl_color(0x6B, 0x8E, 0x23));
		_colors.add("orange", sdl_color(0xFF, 0xA5, 0x00));
		_colors.add("orangered", sdl_color(0xFF, 0x45, 0x00));
		_colors.add("orchid", sdl_color(0xDA, 0x70, 0xD6));
		_colors.add("palegoldenrod", sdl_color(0xEE, 0xE8, 0xAA));
		_colors.add("palegreen", sdl_color(0x98, 0xFB, 0x98));
		_colors.add("paleturquoise", sdl_color(0xAF, 0xEE, 0xEE));
		_colors.add("palevioletred", sdl_color(0xDB, 0x70, 0x93));
		_colors.add("papayawhip", sdl_color(0xFF, 0xEF, 0xD5));
		_colors.add("peachpuff", sdl_color(0xFF, 0xDA, 0xB9));
		_colors.add("peru", sdl_color(0xCD, 0x85, 0x3F));
		_colors.add("pink", sdl_color(0xFF, 0xC0, 0xCB));
		_colors.add("plum", sdl_color(0xDD, 0xA0, 0xDD));
		_colors.add("powderblue", sdl_color(0xB0, 0xE0, 0xE6));
		_colors.add("purple", sdl_color(0x80, 0x00, 0x80));
		_colors.add("red", sdl_color(0xFF, 0x00, 0x00));
		_colors.add("rosybrown", sdl_color(0xBC, 0x8F, 0x8F));
		_colors.add("royalblue", sdl_color(0x41, 0x69, 0xE1));
		_colors.add("saddlebrown", sdl_color(0x8B, 0x45, 0x13));
		_colors.add("salmon", sdl_color(0xFA, 0x80, 0x72));
		_colors.add("sandybrown", sdl_color(0xF4, 0xA4, 0x60));
		_colors.add("seagreen", sdl_color(0x2E, 0x8B, 0x57));
		_colors.add("seashell", sdl_color(0xFF, 0xF5, 0xEE));
		_colors.add("sienna", sdl_color(0xA0, 0x52, 0x2D));
		_colors.add("silver", sdl_color(0xC0, 0xC0, 0xC0));
		_colors.add("skyblue", sdl_color(0x87, 0xCE, 0xEB));
		_colors.add("slateblue", sdl_color(0x6A, 0x5A, 0xCD));
		_colors.add("slategray", sdl_color(0x70, 0x80, 0x90));
		_colors.add("snow", sdl_color(0xFF, 0xFA, 0xFA));
		_colors.add("springgreen", sdl_color(0x00, 0xFF, 0x7F));
		_colors.add("steelblue", sdl_color(0x46, 0x82, 0xB4));
		_colors.add("tan", sdl_color(0xD2, 0xB4, 0x8C));
		_colors.add("teal", sdl_color(0x00, 0x80, 0x80));
		_colors.add("thistle", sdl_color(0xD8, 0xBF, 0xD8));
		_colors.add("tomato", sdl_color(0xFF, 0x63, 0x47));
		_colors.add("turquoise", sdl_color(0x40, 0xE0, 0xD0));
		_colors.add("violet", sdl_color(0xEE, 0x82, 0xEE));
		_colors.add("wheat", sdl_color(0xF5, 0xDE, 0xB3));
		_colors.add("white", sdl_color(0xFF, 0xFF, 0xFF));
		_colors.add("whitesmoke", sdl_color(0xF5, 0xF5, 0xF5));
		_colors.add("yellow", sdl_color(0xFF, 0xFF, 0x00));
		_colors.add("yellowgreen", sdl_color(0x9A, 0xCD, 0x32));
}

void rich_text_renderer::font_size FUNCTION_PARAMETER_LIST(size_t s){
	_font.reassign(OBJECT_NEW(sdlttf_font, TRACE_PARAMS(static_cast<char*>(_font_file.raw_pointer()), s)));
	_size = s;
}

void rich_text_renderer::reset_renderer FUNCTION_PARAMETER_LIST(){
	if(_r_size != _size){
		font_size(_r_size = _size);
	}
	_font->style(sdlttf_font::normal);
	_r_color = _color;
}

#ifdef STACK_TRACE_FLAG

#define append_text_surface(...) append_text_surface(INVOKE_TRACE, ##__VA_ARGS__)

#endif
namespace{
	inline void expect_next(const char *&str, int c){
		while(*str && *str++ != c);
	}
	
	void append_text_surface FUNCTION_PARAMETER_LIST(array_list<sdl_surface> &out,
		const char *msg, sdlttf_font::render_style style, sdlttf_font &font, sdl_color c, size_t w, sdl_color bc){
		sdl_surface text(TRACE_PARAMS(msg, style, font.outline(0), c));
		if(w){
			sdl_surface bkgrd(TRACE_PARAMS(msg, style, font.outline(w), bc));
			sdl_rect dr(w, w, text.width(), text.height());
			text.blit_to(sdl_renderer::blend, bkgrd, dr);
			out.add(bkgrd);
		}else{
			out.add(text);
		}
	}
}

void rich_text_renderer::back_color(float w, sdl_color c){
	_bk_width = w;
	_bk_color = c;
}

void rich_text_renderer::render_text FUNCTION_PARAMETER_LIST(const char *str, array_list<sdl_surface> &out){
	map<const char*, sdl_color>::entry_type *color_entry;
	if(str){
		size_t len = strlen(str);
		pool_ptr<CCLIBC::allocator> buf_ptr(CCLIBC::allocator::instance().allocate(len+1));
		char *msg_buf = static_cast<char*>(buf_ptr.raw_pointer());
		char *des = msg_buf;
		char c; bool content = true;
		//int tem_size;
		sdlttf_font::font_style tem_style;
		while(content){
			switch(c = *str++){
				case '{':
					if(!strncmp(str, "\\1c&", 4)){
						int tem_color;
						sscanf(str, "%*[^Hh]%*[Hh]%6x", &tem_color);
						_r_color.r=tem_color;
						_r_color.g=tem_color>>8;
						_r_color.b=tem_color>>16;
						expect_next(str, '}');
					}else if(!strncmp(str, "\\fs", 3)){
						//sscanf(str+3, "%d", &tem_size);
						//font_size(tem_size);
						expect_next(str, '}');
					}
					break;
				case '<':
					--str;
					if(!strncmp(str, "<b>", 3)){
						_font->style(_font->style() | sdlttf_font::bold);
						str +=3;
					}else if(!strncmp(str, "<i>", 3)){
						_font->style(_font->style() | sdlttf_font::italic);
						str +=3;
					}else if(!strncmp(str, "<u>", 3)){
						_font->style(_font->style() | sdlttf_font::underline);
						str +=3;
					}else if(!strncmp(str, "<font", 5)){
						const char *p = str+5;
						expect_next(str, '>');
						char font_info[256], color_name[128];
						size_t len = str-p;
						memcpy(font_info, p, len);
						font_info[len] = '\0';
						int r,g,b;
						if(3==sscanf(font_info, "%*[^#]%*[#]%2x%2x%2x", &r, &g, &b)){
							_r_color.r=r;
							_r_color.g=g;
							_r_color.b=b;
						}else if(1==sscanf(font_info, "%*[^\"]%*[\"]%[^\"]", color_name)){
							if((color_entry = _colors.get_entry(color_name))){
								_r_color = color_entry->value();
							}
						}else if(1==sscanf(font_info, "%*[^']%*[']%[^']", color_name)){
							if((color_entry = _colors.get_entry(color_name))){
								_r_color = color_entry->value();
							}
						}
					}else if(((!strncmp(str, "</b>", 3)) && ((tem_style=static_cast<sdlttf_font::font_style>(_font->style() & (~sdlttf_font::bold))), true))
								|| ((!strncmp(str, "</i>", 3)) && ((tem_style=static_cast<sdlttf_font::font_style>(_font->style() & (~sdlttf_font::italic))), true))
								|| ((!strncmp(str, "</u>", 3)) && ((tem_style=static_cast<sdlttf_font::font_style>(_font->style() & (~sdlttf_font::underline))), true))){
						str += 4;
						if(des!=msg_buf && *msg_buf){
							*des = '\0';
							append_text_surface(out, msg_buf, _style, *_font, _r_color, _bk_width*_r_size, _bk_color);
							des = msg_buf;
						}
						_font->style(static_cast<sdlttf_font::font_style>(tem_style));
					}else if(!strncmp(str, "<br>", 4)){
						str += 4;
						if(des!=msg_buf && *msg_buf){
							*des = '\0';
							append_text_surface(out, msg_buf, _style, *_font, _r_color, _bk_width*_r_size, _bk_color);
							des = msg_buf;
						}
					}else if(!strncmp(str, "</font>", 7)){
						str += 7;
						if(des!=msg_buf && *msg_buf){
							*des = '\0';
							append_text_surface(out, msg_buf, _style, *_font, _r_color, _bk_width*_r_size, _bk_color);
							des = msg_buf;
						}
						_r_color = _color;
					}else{
						expect_next(str, '>');
					}
					break;
				case '\0':
					content = false;
				default:
					*des++ = c;
					break;
			}
		}
		if(des!=msg_buf && *msg_buf){
			append_text_surface(out, msg_buf, _style, *_font, _r_color, _bk_width*_r_size, _bk_color);
		}
		//if(-1 != tem_size){
		//	font_size(_r_size);
		//}
	}
}

CCSDL2_END

