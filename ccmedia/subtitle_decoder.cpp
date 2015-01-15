#include <cstring>
#include "ccmedia/subtitle_decoder.h"
#include "cclibc/memory.h"
#include "ccapplib/string_util.h"

using CCLIBC::object;
using CCLIBC::illegal_argument;
using CCSDL2::sdl_color;
using CCSDL2::sdl_locked_txture;
using CCAPPLIB::string_util;
using CCAPPLIB::time_bucket;
using CCFFMPEG::media_stream;
using CCFFMPEG::media_decoder;
using CCFFMPEG::media_packet;

CCMEDIA_BG

object* external_subtitle_decoder::parse_decoder(const char* sub, size_t w, size_t h, size_t sw, size_t sh, const char *f, float fs, sdl_color c, float pb, const char *s, const char *fc, bool upfc){
	if(string_util::end_with_ig(sub, ".srt")){
		return ALLOCATOR_NEW(srt_decoder, TRACE_PARAMS(f, static_cast<size_t>(w/35*fs), c, pb));
	}else if(string_util::end_with_ig(sub, ".ass")){
		return ALLOCATOR_NEW(ass_decoder, TRACE_PARAMS(w, h, sw, sh, s, fc, upfc));
	}
	THROW_NEW_EXCEPTION(illegal_argument, "Not supported subtitle", "AVStream", __PRETTY_FUNCTION__);
}

subtitle_stream* external_subtitle_decoder::parse_stream(const char *sub, const char *cs){
	if(string_util::end_with_ig(sub, ".srt")){
		return ALLOCATOR_NEW(srt_stream, TRACE_PARAMS(*static_cast<srt_decoder*>(_decoder.raw_pointer()), sub, cs));
	}else if(string_util::end_with_ig(sub, ".ass")){
		return ALLOCATOR_NEW(ass_stream, TRACE_PARAMS(*static_cast<ass_decoder*>(_decoder.raw_pointer()), sub, cs));
	}
	THROW_NEW_EXCEPTION(illegal_argument, "Not supported subtitle", "AVStream", __PRETTY_FUNCTION__);
}

external_subtitle_decoder::external_subtitle_decoder FUNCTION_PARAMETER_LIST(const char *sub, size_t w, size_t h, size_t sw, size_t sh, const char *f, float fs,
	sdl_color c, const char *cs, float pb, const char *s, const char *fc, bool upfc):
	_decoder(parse_decoder(sub, w, h, sw, sh, f, fs, c, pb, s, fc, upfc)),
	_stream(parse_stream(sub, cs)){
}

void external_subtitle_decoder::render_frame FUNCTION_PARAMETER_LIST(sdl_locked_txture &lt, size_t t){
	_stream->render_frame(lt, t);
}

object* media_subtitle_decoder::parse_decoder(size_t w, size_t h, size_t sw, size_t sh, const char *f, float fs, sdl_color c, float pb, const char *s, const char *fc, bool upfc){
	switch(_context->codec_id){
		case AV_CODEC_ID_SSA:
			return ALLOCATOR_NEW(ass_decoder, TRACE_PARAMS(w, h, sw, sh, s, fc, upfc));
		case AV_CODEC_ID_TEXT:
		case AV_CODEC_ID_SUBRIP:
			return ALLOCATOR_NEW(srt_decoder, TRACE_PARAMS(f, static_cast<size_t>(sw/35*fs), c, pb));
		default:
			THROW_NEW_EXCEPTION(illegal_argument, "Not supported subtitle", "AVStream", __PRETTY_FUNCTION__);
	}
}

subtitle_stream* media_subtitle_decoder::parse_stream(){
	switch(_context->codec_id){
		case AV_CODEC_ID_SSA:
			return ALLOCATOR_NEW(ass_stream,
				TRACE_PARAMS(*static_cast<ass_decoder*>(_decoder.raw_pointer()), reinterpret_cast<char*>(_context->extradata), _context->extradata_size));
		case AV_CODEC_ID_TEXT:
		case AV_CODEC_ID_SUBRIP:
			return ALLOCATOR_NEW(srt_stream, *static_cast<srt_decoder*>(_decoder.raw_pointer()), reinterpret_cast<char*>(_context->extradata), _context->extradata_size);
		default:
			THROW_NEW_EXCEPTION(illegal_argument, "Not supported subtitle", "AVStream", __PRETTY_FUNCTION__);
	}
}

media_subtitle_decoder::media_subtitle_decoder FUNCTION_PARAMETER_LIST(media_stream &st, size_t i, size_t w, size_t h, size_t sw, size_t sh, const char *f,
	float fs, sdl_color c, float pb, const char *s, const char *fc, bool updc): media_decoder(TRACE_PARAMS(st, i, AVMEDIA_TYPE_SUBTITLE)),
		_decoder(parse_decoder(w, h, sw, sh, f, fs, c, pb, s, fc, updc)), _stream(parse_stream()){
}

void media_subtitle_decoder::render_frame FUNCTION_PARAMETER_LIST(sdl_locked_txture &lt, size_t t){
	_stream->render_frame(lt, t);
}

void media_subtitle_decoder::decode_frame FUNCTION_PARAMETER_LIST(media_packet &pkt){
	AVPacket *p = static_cast<AVPacket*>(pkt.raw_pointer());
	int64_t pts = media_decoder::_stream->time_base.num;
			pts = 1000 * pts * p->pts/media_decoder::_stream->time_base.den;
	_stream->extend_content(reinterpret_cast<char*>(p->data),
		time_bucket(pts, p->duration));
}

CCMEDIA_END
