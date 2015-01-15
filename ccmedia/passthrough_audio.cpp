#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif
extern "C" {
#include <libavutil/opt.h>
}
#include "ccmedia/passthrough_audio.h"

using CCFFMPEG::media_stream;
using CCFFMPEG::ffmpeg_error;

CCMEDIA_BG

passthrough_audio::passthrough_audio FUNCTION_PARAMETER_LIST(media_stream &stm, int s, const char *dev, size_t l, size_t m)
	:media_decoder(TRACE_PARAMS(stm, s, AVMEDIA_TYPE_AUDIO )){
	size_t pa_rate;
	int64_t pa_chanel;
	bool isdtshd_ma = false;
	switch(_context->codec_id){
		case AV_CODEC_ID_EAC3:
			pa_chanel = AV_CH_LAYOUT_7POINT1;
			pa_rate = _context->sample_rate;
			break;
		case AV_CODEC_ID_DTS:
			if (FF_PROFILE_DTS_HD_MA == _context->profile) {
				pa_chanel = AV_CH_LAYOUT_7POINT1;
				pa_rate = 192000;
				isdtshd_ma = 1;
				break;
			}
		case AV_CODEC_ID_AC3:
			pa_chanel = AV_CH_LAYOUT_STEREO;
			pa_rate = _context->sample_rate;
			break;
		case AV_CODEC_ID_TRUEHD:
			pa_chanel = AV_CH_LAYOUT_7POINT1;
			pa_rate = 192000;
			break;
		default:
			pa_chanel = _context->channel_layout;
			pa_rate = _context->sample_rate;
			break;
	}
	_aplayer.reassign(STACK_NEW(_pool, audio_player, TRACE_PARAMS(dev,
		AV_SAMPLE_FMT_S16, pa_chanel, pa_rate, true, 100, l, m)));
	INVOKE_THROW_RETURN(int, < 0, ffmpeg_error, avformat_alloc_output_context2, &_ps_context, NULL, "spdif", "");
	_ps_context->pb = INVOKE_THROW(AVIOContext*, == NULL, ffmpeg_error, avio_alloc_context,
		static_cast<uint8_t*>(_pool.allocate(_aplayer->period_buffer_size())),
		_aplayer->period_buffer_size(), 1, this, NULL, write_packet, NULL);
	if (isdtshd_ma) {
		INVOKE_THROW_RETURN(int, < 0, ffmpeg_error, av_opt_set_int, _ps_context->priv_data, "dtshd_rate", 768000, 0);
	}
	outstream[0] = _stream;
	_ps_context->streams = outstream;
	INVOKE_THROW_RETURN(int, < 0, ffmpeg_error, _ps_context->oformat->write_header, _ps_context);
}

void passthrough_audio::push_bit_audio FUNCTION_PARAMETER_LIST(CCFFMPEG::media_packet &pkt){
	AVPacket *p = static_cast<AVPacket*>(pkt.raw_pointer());
	uint64_t pts = _stream->time_base.num;
	_last_pts = 1000 * (p->pts - _stream->start_time) * pts/_stream->time_base.den;
	_ps_context->oformat->write_packet(_ps_context, p);
}

CCMEDIA_END
