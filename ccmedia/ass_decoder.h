#ifndef CCMEDIA_ASS_DECORDER_H_
#define CCMEDIA_ASS_DECORDER_H_

extern "C"{
	#include <ass/ass.h>
};

#include "ccmedia/env.h"
#include "cclibc/trace.h"
#include "cclibc/exception.h"
#include "cclibc/util/array_list.h"
#include "ccmedia/subtitle.h"

CCMEDIA_BG

class ass_decoder_error: public CCLIBC::exception{
public:
	ass_decoder_error(const char *c): exception("%s: ass decoder error.", c){}
};

class ass_stream;
class ass_decoder: public CCLIBC::object{
friend class ass_stream;
private:
	ASS_Library *_lib;
	ASS_Renderer *_renderer;
	CCSDL2::sdl_rect _video_rect;
public:
	ass_decoder FUNCTION_PARAMETER_LIST(size_t w, size_t h, size_t sw, size_t sh, const char *s = "Sans", const char *fc = NULL, bool upfc = true);
	~ass_decoder();
	const CCSDL2::sdl_rect& video_rect() const{
		return _video_rect;
	}
};

class ass_stream: public subtitle_stream{
private:
	ASS_Track *_track;
	ass_decoder &_decoder;
public:
	ass_stream FUNCTION_PARAMETER_LIST(ass_decoder&, const char*, const char *cs = NULL);
	ass_stream FUNCTION_PARAMETER_LIST(ass_decoder&, char*, size_t);
	~ass_stream();
	void render_frame FUNCTION_PARAMETER_LIST(CCSDL2::sdl_locked_txture&, size_t);
	void extend_content FUNCTION_PARAMETER_LIST(const char*, const CCAPPLIB::time_bucket&);
	void clear_cache(){};
};

CCMEDIA_END

#endif //CCMEDIA_ASS_DECORDER_H_
