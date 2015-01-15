#ifndef CCFFMPEG_CCFFMPEG_H_
#define CCFFMPEG_CCFFMPEG_H_
#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

extern "C" {
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#include "ccffmpeg/env.h"
#include "cclibc/trace.h"
#include "cclibc/exception.h"
#include "cclibc/memory.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/thread/semaphore.h"
#include "cclibc/util/list.h"
#include "ccapplib/string_util.h"

CCFFMPEG_BG

class ffmpeg_error: public CCLIBC::exception{
public:
	ffmpeg_error(const char*, int);
	ffmpeg_error(const char*);
};

class format_register{
private:
	static int register_all(){
		av_register_all();
		return 0;
	}
protected:
	format_register(){
		static int i = register_all();
		(void)i;
		return;
	}
};

class media_stream;
class media_decoder;
class video_decoder;
class audio_decoder;
class media_packet: public CCLIBC::single_list<media_packet>::node{
friend class media_stream;
friend class media_decoder;
friend class video_decoder;
friend class audio_decoder;
private:
	AVPacket _packet;
	bool _has_data;
	AVStream *_stream;
public:
	media_packet();
	~media_packet();
	void free_packet(){
		if(_has_data){
			av_free_packet(&_packet);
		}
		_stream = NULL;
	}
	void* raw_pointer(){
		return &_packet;
	}
	
	size_t presentation_time(){
		if (_packet.pts != AV_NOPTS_VALUE){
			return static_cast<uint64_t>(1000) * (_packet.pts
				- _stream->start_time) * _stream->time_base.num/_stream->time_base.den;
		}else{
			return 0;
		}
	}
};

class media_decoder;
class media_stream: private format_register {
friend class media_decoder;
private:
	AVFormatContext *_context;
	bool _output;
	int find_stream(AVMediaType t, size_t start = 0){
		while(start<_context->nb_streams){
			if(t == _context->streams[start]->codec->codec_type){
				return start;
			}
			++start;
		}
		return -1;
	}
public:
	media_stream FUNCTION_PARAMETER_LIST(const char*);
	~media_stream();
	void media_info FUNCTION_PARAMETER_LIST(CCAPPLIB::string_array<>&);
	void* extradata(size_t s) const{
		return _context->streams[s]->codec->extradata;
	}

	size_t extradata_size(size_t s) const{
		return _context->streams[s]->codec->extradata_size;
	}

	size_t duration(size_t str) const{
		AVStream *_stream = _context->streams[str];
		uint64_t s = _stream->duration;
		s = s*1000*_stream->time_base.num/_stream->time_base.den;
		debug_assert(!(s>>32));
		return s;
	}
	
	size_t duration() const{
		uint64_t s = _context->duration;
		if(static_cast<uint64_t>(AV_NOPTS_VALUE) != s){
			s = s*1000/AV_TIME_BASE;
			debug_assert(!(s>>32));
			return s;
		}
		return 0;
	}
	
	size_t max_duration() const {
		size_t r = 0;
		for(size_t i=0; i<_context->nb_streams; ++i){
			CCLIBC::util::update_max(r, duration(i));
		}
		return r;
	}
	
	int find_video(size_t start = 0){
		return find_stream(AVMEDIA_TYPE_VIDEO, start);
	}
	int find_audio(size_t start = 0){
		return find_stream(AVMEDIA_TYPE_AUDIO, start);
	}
	int find_subtitle(size_t start = 0){
		while(start<_context->nb_streams){
			if(AVMEDIA_TYPE_SUBTITLE == _context->streams[start]->codec->codec_type
				&& AV_CODEC_ID_HDMV_PGS_SUBTITLE != _context->streams[start]->codec->codec_id){
				return start;
			}
			++start;
		}
		return -1;
	}

	bool bit_audio(size_t i) const;
	
	bool read_packet FUNCTION_PARAMETER_LIST(media_packet&);

	void seek_frame FUNCTION_PARAMETER_LIST(size_t);
};

class media_frame;
class media_decoder{
protected:
	media_stream &_container;
	AVStream *_stream;
	AVCodecContext *_context;
	size_t _index;
	media_decoder FUNCTION_PARAMETER_LIST(media_stream &, size_t, AVMediaType, bool op = true);
	bool _got_frame;
	bool _opened;
	void update_presentation_time(media_frame&);
public:
	virtual ~media_decoder();

	size_t duration() const{
		return _container.duration(_index);
	}

	bool contains_data(const media_packet &pkt) const{
		return pkt._has_data && pkt._packet.stream_index == static_cast<int>(_index);
	}
	
	bool got_frame() const{
		return _got_frame;
	}

	void flush_buffer();
};

class video_frame;
class soft_scaler;
class video_decoder: public media_decoder{
friend class soft_scaler;
public:
	video_decoder FUNCTION_PARAMETER_LIST(media_stream&, size_t);

	size_t width() const{
		return _context->width;
	}

	size_t height() const{
		return _context->height;
	}
	
	AVPixelFormat format() const{
		return _context->pix_fmt;
	}

	bool decode_frame FUNCTION_PARAMETER_LIST(media_packet&, video_frame&);
};

class media_frame{
protected:
	AVFrame *_frame;
	media_frame FUNCTION_PARAMETER_LIST();
	virtual ~media_frame();
	size_t _presentation_time;
	size_t _duration;
public:
	void* raw_data() const{
		return _frame;
	}
	
	size_t presentation_time() const{
		return _presentation_time;
	}
	
	size_t duration() const{
		return _duration;
	}
	
	void presentation_time(size_t pts){
		_presentation_time = pts;
	}
	
	void duration(size_t dur){
		_duration = dur;
	}
};

class video_frame: public media_frame, private CCLIBC::single_list<video_frame>::node{
friend class CCLIBC::single_list<video_frame>;
public:
	video_frame FUNCTION_PARAMETER_LIST();
	AVPixelFormat format() const{
		return static_cast<AVPixelFormat>(_frame->format);
	}
	size_t width() const{
		return _frame->width;
	}
	size_t height() const{
		return _frame->height;
	}
};

class soft_scaler {
private:
	SwsContext *_context;
	size_t _h;

public:
	soft_scaler FUNCTION_PARAMETER_LIST(video_decoder&, size_t, size_t, AVPixelFormat, int al = SWS_FAST_BILINEAR);
	soft_scaler FUNCTION_PARAMETER_LIST(size_t, size_t, AVPixelFormat, size_t, size_t, AVPixelFormat, int al = SWS_FAST_BILINEAR);
	~soft_scaler();
	void scale(const void* const[], const int*, void* const[], const int*);
	void scale(const video_frame&, void* const[], const int*);
};

class audio_frame;
class audio_converter;
class audio_decoder: public media_decoder{
friend class audio_converter;
public:
	audio_decoder FUNCTION_PARAMETER_LIST(media_stream&, size_t);
	
	size_t rate() const{
		return _context->sample_rate;
	}

	size_t channels() const{
		return _context->channels;
	}
	
	int64_t channel_layout() const{
		if(_context->channel_layout){
			return _context->channel_layout;
		}else{
			static int64_t layouts[] = {
				AV_CH_LAYOUT_MONO,
				AV_CH_LAYOUT_STEREO,
				AV_CH_LAYOUT_2POINT1,
				AV_CH_LAYOUT_4POINT0,
				AV_CH_LAYOUT_4POINT1,
				AV_CH_LAYOUT_5POINT1,
				AV_CH_LAYOUT_6POINT1,
				AV_CH_LAYOUT_7POINT1,
				};
			return layouts[_context->channels - 1];
		}
	}
	
	AVSampleFormat format() const{
		return _context->sample_fmt;
	}
	bool decode_frame FUNCTION_PARAMETER_LIST(media_packet&, audio_frame&);
};

class audio_frame: public media_frame{
private:
public:
	audio_frame FUNCTION_PARAMETER_LIST();
};

class audio_converter{
private:
	int64_t _in_cl, _out_cl;
	AVSampleFormat _in_fmt, _out_fmt;
	size_t _in_rate, _out_rate, _out_chs, _frame_size;
	SwrContext *_context;
public:
	audio_converter FUNCTION_PARAMETER_LIST(int64_t, AVSampleFormat, size_t, int64_t, AVSampleFormat, size_t);
	audio_converter FUNCTION_PARAMETER_LIST(audio_decoder&, int64_t, AVSampleFormat, size_t);
	~audio_converter();
	size_t required_buffer_size(audio_frame&);
	void convert_audio(audio_frame&, void*, size_t&);
};

#ifdef STACK_TRACE_FLAG

#define media_info(...) media_info(INVOKE_TRACE, ##__VA_ARGS__)
#define read_packet(...) read_packet(INVOKE_TRACE, ##__VA_ARGS__)
#define decode_frame(...) decode_frame(INVOKE_TRACE, ##__VA_ARGS__)
#define put_picture(...) put_picture(INVOKE_TRACE, ##__VA_ARGS__)
#define wait_picture(...) wait_picture(INVOKE_TRACE, ##__VA_ARGS__)
#define seek_frame(...) seek_frame(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCFFMPEG_END


#endif //CCFFMPEG_CCFFMPEG_H_
