#include "ccffmpeg/ffmpeg.h"
#include "cclibc/callc.h"

using CCLIBC::stack_ptr;
using CCLIBC::pool_ptr;
using CCLIBC::allocator;
using CCLIBC::exception;
using CCAPPLIB::string_array;

CCFFMPEG_BG

namespace{
	__thread char error_msg_buffer[AV_ERROR_MAX_STRING_SIZE];

	AVFormatContext* init_context_from_file(const char *f){
		AVFormatContext *res = NULL;
		INVOKE_THROW_RETURN(int, != 0, ffmpeg_error, avformat_open_input, &res, f, NULL, NULL);
		return res;
	}
}

ffmpeg_error::ffmpeg_error(const char *c, int e): exception("%s: FFmpeg error: %s(%d)", c,
	av_make_error_string(error_msg_buffer, sizeof(error_msg_buffer), e), e) {}

ffmpeg_error::ffmpeg_error(const char *c): exception("%s: FFmpeg error", c) {}

media_packet::media_packet(): _has_data(false), _stream(NULL) {
	av_init_packet(&_packet);
}

media_packet::~media_packet(){
	free_packet();
}

media_stream::media_stream FUNCTION_PARAMETER_LIST(const char *f)
	: _context(init_context_from_file(f)), _output(false){
	INVOKE_THROW_RETURN(int, != 0, ffmpeg_error, avformat_find_stream_info, _context, NULL);
}

media_stream::~media_stream(){
	avformat_close_input(&_context);
}

bool media_stream::bit_audio(size_t i) const{
	if(i <  _context->nb_streams){
		AVCodecContext *codec = _context->streams[i]->codec;
		return AVMEDIA_TYPE_AUDIO == codec->codec_type
			&& (AV_CODEC_ID_EAC3 == codec->codec_id 
				|| AV_CODEC_ID_AC3 == codec->codec_id
				|| AV_CODEC_ID_DTS == codec->codec_id
				|| AV_CODEC_ID_TRUEHD == codec->codec_id);
	}
	return false;
}

void media_stream::media_info FUNCTION_PARAMETER_LIST(string_array<> &out){
	char buf[1024];
	for(size_t i=0; i<_context->nb_streams; i++){
		if(AV_CODEC_ID_HDMV_PGS_SUBTITLE != _context->streams[i]->codec->codec_id){
			avcodec_string(buf, sizeof(buf), _context->streams[i]->codec, _output);
			pool_ptr<allocator> str_ptr(allocator::instance().allocate("%d: %s", i, buf));
			out.add(str_ptr);
		}
	}
}

bool media_stream::read_packet FUNCTION_PARAMETER_LIST(media_packet &pkt){
	pkt.free_packet();
	int r = av_read_frame(_context, &pkt._packet);
	if((pkt._has_data = (r == 0))){
		pkt._stream = _context->streams[pkt._packet.stream_index];
	}else if(AVERROR_EOF != r){
		THROW_NEW_EXCEPTION(ffmpeg_error, "av_read_frame", r);
	}
	return pkt._has_data;
}

void media_stream::seek_frame FUNCTION_PARAMETER_LIST(size_t ms){
	INVOKE_THROW_RETURN(int, < 0, ffmpeg_error, av_seek_frame, _context, -1, ms*static_cast<uint64_t>(1000), AVSEEK_FLAG_BACKWARD);
}

media_decoder::media_decoder FUNCTION_PARAMETER_LIST(media_stream &m, size_t s, AVMediaType mt, bool op)
	: _container(m), _stream(NULL), _context(NULL), _index(s), _got_frame(false), _opened(op) {
	ILLEGAL_ARGUMENT_CHECK(s<m._context->nb_streams && mt == m._context->streams[s]->codec->codec_type, stream);
	_context = m._context->streams[s]->codec;
	if(op){
		if(AVCodec *cd = avcodec_find_decoder(_context->codec_id)){
			INVOKE_THROW_RETURN(int, != 0, ffmpeg_error, avcodec_open2, _context, cd, NULL);
			_stream = m._context->streams[s];
		}else{
			THROW_NEW_EXCEPTION(exception, "No FFmpeg decoder for stream %s", s);
		}
	}
}

void media_decoder::update_presentation_time(media_frame &f){
	uint64_t pts = _stream->time_base.num;
	if (static_cast<AVFrame*>(f.raw_data())->pkt_pts != AV_NOPTS_VALUE){
		pts = 1000 * (static_cast<AVFrame*>(f.raw_data())->pkt_pts
			- _stream->start_time) * pts/_stream->time_base.den;
	}else{
		pts = 1000 * pts * (av_frame_get_best_effort_timestamp(
			static_cast<AVFrame*>(f.raw_data())) - _stream->start_time)/_stream->time_base.den;
	}
	size_t dur;
	if (static_cast<AVFrame*>(f.raw_data())->pkt_duration){
		dur = static_cast<AVFrame*>(f.raw_data())->pkt_duration;
	}
	f.presentation_time(pts);
	f.duration(dur);
}

void media_decoder::flush_buffer(){
	avcodec_flush_buffers(_context);
}

media_decoder::~media_decoder(){
	if(_opened){
		avcodec_close(_context);
	}
}

video_decoder::video_decoder FUNCTION_PARAMETER_LIST(media_stream &m, size_t s): media_decoder(TRACE_PARAMS(m, s, AVMEDIA_TYPE_VIDEO )){ }

bool video_decoder::decode_frame FUNCTION_PARAMETER_LIST(media_packet &pkt, video_frame &frm){
	int i = 0;
	int len = INVOKE_THROW_RETURN(int, < 0, ffmpeg_error, avcodec_decode_video2,
		_context, static_cast<AVFrame*>(frm.raw_data()), &i, &pkt._packet);
	if((_got_frame = i)){
		update_presentation_time(frm);
	}
	pkt._packet.data += len;
	return pkt._packet.size -= len;
}

media_frame::media_frame FUNCTION_PARAMETER_LIST(): _frame(INVOKE_THROW(AVFrame*, == NULL, ffmpeg_error, av_frame_alloc)), _presentation_time(0) {}

media_frame::~media_frame(){
	av_frame_free(&_frame);
}

video_frame::video_frame FUNCTION_PARAMETER_LIST(): media_frame(TRACE_PARAMS()){}

soft_scaler::soft_scaler FUNCTION_PARAMETER_LIST(video_decoder &dec,size_t w, size_t h, AVPixelFormat f, int al)
	: _context(INVOKE_THROW(SwsContext*, == NULL, ffmpeg_error,
		sws_getContext, dec.width(), dec.height(), dec._context->pix_fmt,
		w, h, f, al, NULL, NULL, NULL)), _h(dec.height()) {}
		
soft_scaler::soft_scaler FUNCTION_PARAMETER_LIST(size_t sw, size_t sh, AVPixelFormat sf,
	size_t dw, size_t dh, AVPixelFormat df, int al)
	: _context(INVOKE_THROW(SwsContext*, == NULL, ffmpeg_error,
		sws_getContext, sw, sh, sf,	dw, dh, df, al, NULL, NULL, NULL)), _h(sh){}

soft_scaler::~soft_scaler(){
	sws_freeContext(_context);
}

void soft_scaler::scale(const void* const src[], const int *sl, void* const des[], const int *dl){
	sws_scale(_context, reinterpret_cast<const uint8_t* const*>(src), sl, 0, _h, reinterpret_cast<uint8_t* const*>(des), dl);
}

void soft_scaler::scale(const video_frame &f, void* const des[], const int *dl){
	sws_scale(_context, static_cast<AVFrame*>(f.raw_data())->data,
		static_cast<AVFrame*>(f.raw_data())->linesize, 0, _h, reinterpret_cast<uint8_t* const*>(des), dl);
}

audio_decoder::audio_decoder FUNCTION_PARAMETER_LIST(media_stream &m, size_t s): media_decoder(TRACE_PARAMS(m, s, AVMEDIA_TYPE_AUDIO )){ }

bool audio_decoder::decode_frame FUNCTION_PARAMETER_LIST(media_packet &pkt, audio_frame &frm){
	int i = 0;
	int len = INVOKE_THROW_RETURN(int, < 0, ffmpeg_error, avcodec_decode_audio4,
		_context, static_cast<AVFrame*>(frm.raw_data()), &i, &pkt._packet);
	if((_got_frame = i)){
		update_presentation_time(frm);
	}
	pkt._packet.data += len;
	return pkt._packet.size -= len;
}

audio_frame::audio_frame FUNCTION_PARAMETER_LIST(): media_frame(TRACE_PARAMS()){}

audio_converter::audio_converter FUNCTION_PARAMETER_LIST(int64_t in_cl, AVSampleFormat in_f,
	size_t in_r, int64_t out_cl, AVSampleFormat out_f, size_t out_r)
	:_in_cl(in_cl), _out_cl(out_cl), _in_fmt(in_f), _out_fmt(out_f), _in_rate(in_r),
	_out_rate(out_r), _out_chs(av_get_channel_layout_nb_channels(out_cl)),
	_frame_size(av_get_bytes_per_sample(_out_fmt)*_out_chs),
	_context(INVOKE_THROW(SwrContext*, == NULL, ffmpeg_error, swr_alloc_set_opts, NULL, _out_cl, _out_fmt, _out_rate,
	_in_cl, _in_fmt, _in_rate, 0, NULL)){
	INVOKE_THROW_RETURN(int, != 0, ffmpeg_error, swr_init, _context);
}

audio_converter::audio_converter FUNCTION_PARAMETER_LIST(audio_decoder &dec, int64_t cl, AVSampleFormat f, size_t r)
	:_in_cl(dec._context->channel_layout), _out_cl(cl), _in_fmt(dec._context->sample_fmt), _out_fmt(f),
	_in_rate(dec._context->sample_rate), _out_rate(r), _out_chs(av_get_channel_layout_nb_channels(cl)),
	_frame_size(av_get_bytes_per_sample(_out_fmt)*_out_chs),
	_context(INVOKE_THROW(SwrContext*, == NULL, ffmpeg_error, swr_alloc_set_opts, NULL, _out_cl, _out_fmt, _out_rate,
	_in_cl, _in_fmt, _in_rate, 0, NULL)){
	INVOKE_THROW_RETURN(int, != 0, ffmpeg_error, swr_init, _context);
}

audio_converter::~audio_converter(){
	swr_free(&_context);
}

size_t audio_converter::required_buffer_size(audio_frame &f){
	uint64_t s = static_cast<AVFrame*>(f.raw_data())->nb_samples;
	s = s *  _out_rate / _in_rate;
	return av_samples_get_buffer_size(NULL, _out_chs, s, _out_fmt, 0);
}

void audio_converter::convert_audio(audio_frame &f, void *buf, size_t &len){
	uint64_t s = static_cast<AVFrame*>(f.raw_data())->nb_samples;
	s = s *  _out_rate / _in_rate;
	int c = INVOKE_THROW(int, < 0, ffmpeg_error, swr_convert,_context,
		reinterpret_cast<uint8_t**>(&buf), s, (const uint8_t**)static_cast<AVFrame*>(f.raw_data())->extended_data,
		static_cast<AVFrame*>(f.raw_data())->nb_samples);
	len = c * _frame_size;
}

CCFFMPEG_END
