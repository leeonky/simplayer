#ifndef CCMEDIA_PASSTHROUGH_AUDIO_H_
#define CCMEDIA_PASSTHROUGH_AUDIO_H_

#include "ccmedia/env.h"
#include "ccmedia/audio_player.h"
#include "cclibc/memory.h"
#include "ccffmpeg/ffmpeg.h"

CCMEDIA_BG

#ifdef STACK_TRACE_FLAG

#define push_bit_audio(...) push_bit_audio(INVOKE_TRACE, ##__VA_ARGS__)

#endif

class passthrough_audio: public CCFFMPEG::media_decoder{
private:
	AVFormatContext *_ps_context;
	CCLIBC::stack_pool _pool;
	CCLIBC::stack_ptr<audio_player> _aplayer;
	uint8_t *_outbuf;
	AVStream *outstream[1];
	size_t _last_pts;

	static int write_packet(void *opaque, uint8_t *buf, int buf_size) {
		passthrough_audio &ps = *static_cast<passthrough_audio*>(opaque);
		ps._aplayer->push_audio(buf, buf_size, ps._last_pts);
		ps._last_pts = 0;
		return buf_size;
	}
public:
	passthrough_audio FUNCTION_PARAMETER_LIST(CCFFMPEG::media_stream&, int,
		const char *dev = "hdmi:AES0=0x04", size_t least = 2, size_t most = 10);
	~passthrough_audio(){
		av_free(_ps_context->pb);
		_ps_context->pb = NULL;
		_ps_context->streams = NULL;
		avformat_free_context(_ps_context);
	}
	void push_bit_audio FUNCTION_PARAMETER_LIST(CCFFMPEG::media_packet&);
	void push_trailer(){
		_ps_context->oformat->write_trailer(_ps_context);
	}
	audio_player& player(){
		return *_aplayer;
	}
};

CCMEDIA_END

#endif //CCMEDIA_PASSTHROUGH_AUDIO_H_
