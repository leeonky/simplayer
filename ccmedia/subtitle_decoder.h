#ifndef CCMEDIA_SUBTITLE_DECORDER_H_
#define CCMEDIA_SUBTITLE_DECORDER_H_

#include "ccmedia/env.h"
#include "cclibc/trace.h"
#include "cclibc/exception.h"
#include "ccmedia/srt_decoder.h"
#include "ccmedia/ass_decoder.h"
#include "ccffmpeg/ffmpeg.h"

CCMEDIA_BG

class subtitle_decoder{
public:
	virtual ~subtitle_decoder(){}
	virtual void render_frame FUNCTION_PARAMETER_LIST(CCSDL2::sdl_locked_txture&, size_t) = 0;
};

class external_subtitle_decoder: public subtitle_decoder{
private:
	CCLIBC::object_ptr<CCLIBC::object, CCLIBC::allocator> _decoder;
	CCLIBC::object_ptr<subtitle_stream, CCLIBC::allocator> _stream;
	CCLIBC::object* parse_decoder(const char*, size_t, size_t, size_t, size_t, const char*, float, CCSDL2::sdl_color, float, const char*, const char*, bool);
	subtitle_stream* parse_stream(const char*, const char*);
public:
	external_subtitle_decoder FUNCTION_PARAMETER_LIST(const char*, size_t, size_t, size_t, size_t, const char*, float,
		CCSDL2::sdl_color, const char *cs = "UTF-8", float _bottom = 0.95, const char *s = "Sans",
		const char *fc = NULL, bool upfc = true);
	void render_frame FUNCTION_PARAMETER_LIST(CCSDL2::sdl_locked_txture&, size_t);
};

class media_subtitle_decoder: public CCFFMPEG::media_decoder, public subtitle_decoder{
private:
	CCLIBC::object_ptr<CCLIBC::object, CCLIBC::allocator> _decoder;
	CCLIBC::object_ptr<subtitle_stream, CCLIBC::allocator> _stream;
	CCLIBC::object* parse_decoder(size_t, size_t, size_t, size_t, const char*, float, CCSDL2::sdl_color, float, const char*, const char*, bool);
	subtitle_stream* parse_stream();
public:
	media_subtitle_decoder FUNCTION_PARAMETER_LIST(CCFFMPEG::media_stream&, size_t, size_t, size_t, size_t, size_t, const char*, float, CCSDL2::sdl_color,
		float pb = 0.95, const char *s = "Sans", const char *fc = NULL, bool upfc = true);
	void render_frame FUNCTION_PARAMETER_LIST(CCSDL2::sdl_locked_txture&, size_t);
	void decode_frame FUNCTION_PARAMETER_LIST(CCFFMPEG::media_packet&);
	void clear_cache(){
		_stream->clear_cache();
	}
};

CCMEDIA_END

#endif //CCMEDIA_SUBTITLE_DECORDER_H_
