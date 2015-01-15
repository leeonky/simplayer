#include "cctest/util.h"
#include "ccffmpeg/ffmpeg.cpp"

INTERFACE_TESTABLE_VOID_FUNCTION(av_register_all, non_call)
INTERFACE_TESTABLE_FUNCTION_3(int, av_strerror, r_non_call<int>, int, char*, size_t)

namespace{
int avformat_open_input_mock(AVFormatContext **p, ...){
	static AVFormatContext aff;
	static AVStream* strs[1];
	static AVStream str;
	strs[0] = &str;
	*p = &aff;
	aff.nb_streams = 1;
	aff.streams = strs;
	return 0;
}
}

INTERFACE_TESTABLE_FUNCTION_4(int, avformat_open_input, avformat_open_input_mock, AVFormatContext**, char const*, AVInputFormat*, AVDictionary**)
INTERFACE_TESTABLE_VOID_FUNCTION_1(avformat_close_input, non_call, AVFormatContext**)
INTERFACE_TESTABLE_FUNCTION_2(int, avformat_find_stream_info, r_non_call<int>, AVFormatContext*, AVDictionary**)

void avcodec_string(char *buf, int buf_size, AVCodecContext *enc, int encode){}
INTERFACE_TESTABLE_FUNCTION_3(int, avcodec_open2, r_non_call<int>, AVCodecContext*, const AVCodec*, AVDictionary**)
INTERFACE_TESTABLE_FUNCTION_1(AVCodec*, avcodec_find_decoder, r_non_call_value<AVCodec*>, AVCodecID);
INTERFACE_TESTABLE_FUNCTION_1(int, avcodec_close, r_non_call<int>, AVCodecContext*);
INTERFACE_TESTABLE_VOID_FUNCTION_1(av_init_packet, non_call, AVPacket*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(av_free_packet, non_call, AVPacket*)

namespace{
	int av_read_frame_mock(AVFormatContext*, AVPacket *pkt){
		pkt->stream_index = 0;
		return 0;
	}
}

INTERFACE_TESTABLE_FUNCTION_2(int, av_read_frame, av_read_frame_mock, AVFormatContext*, AVPacket*)
INTERFACE_TESTABLE_FUNCTION(AVFrame*, av_frame_alloc, r_non_call_value<AVFrame*>);
INTERFACE_TESTABLE_VOID_FUNCTION_1(av_frame_free, non_call, AVFrame**)
INTERFACE_TESTABLE_FUNCTION_4(int, avcodec_decode_video2, r_non_call<int>, AVCodecContext*, AVFrame*, int*, const AVPacket*)
INTERFACE_TESTABLE_FUNCTION_10(SwsContext*, sws_getContext, r_non_call_value<SwsContext*>, int, int, AVPixelFormat, int, int, AVPixelFormat, int, SwsFilter*, SwsFilter*, const double*)
INTERFACE_TESTABLE_VOID_FUNCTION_1(sws_freeContext, non_call, SwsContext*)
INTERFACE_TESTABLE_FUNCTION_7(int, sws_scale, r_non_call<int>, SwsContext*, const uint8_t* const*, const int*, int, int, uint8_t* const*, const int*)
INTERFACE_TESTABLE_FUNCTION_1(int64_t, av_frame_get_best_effort_timestamp, r_non_call<int64_t>, const AVFrame*);
INTERFACE_TESTABLE_FUNCTION_4(int, avcodec_decode_audio4, r_non_call<int>, AVCodecContext*, AVFrame*, int*, const AVPacket*)
INTERFACE_TESTABLE_FUNCTION_9(SwrContext*, swr_alloc_set_opts, r_non_call_value<SwrContext*>, SwrContext*, int64_t, AVSampleFormat, int, int64_t, AVSampleFormat, int, int, void*)
INTERFACE_TESTABLE_FUNCTION_1(int, swr_init, r_non_call<int>, SwrContext*);
INTERFACE_TESTABLE_VOID_FUNCTION_1(swr_free, non_call, SwrContext**)

namespace{
	int channel_layout_nb_channels(uint64_t){
		return 2;
	}
}

INTERFACE_TESTABLE_FUNCTION_1(int, av_get_channel_layout_nb_channels, channel_layout_nb_channels, uint64_t)
INTERFACE_TESTABLE_FUNCTION_5(int, av_samples_get_buffer_size, r_non_call<int>, int*, int, int, AVSampleFormat, int)
INTERFACE_TESTABLE_FUNCTION_1(int, av_get_bytes_per_sample, r_non_call<int>, AVSampleFormat)
INTERFACE_TESTABLE_FUNCTION_5(int, swr_convert, r_non_call<int>, SwrContext*, uint8_t**, int, const uint8_t**, int)
INTERFACE_TESTABLE_FUNCTION_4(int, avpicture_alloc, r_non_call<int>, AVPicture*, AVPixelFormat, int, int)
INTERFACE_TESTABLE_VOID_FUNCTION_1(avpicture_free, non_call, AVPicture*)
INTERFACE_TESTABLE_VOID_FUNCTION_5(av_picture_copy, non_call, AVPicture*, const AVPicture*, AVPixelFormat, int, int)
INTERFACE_TESTABLE_FUNCTION_1(int64_t, av_get_default_channel_layout, r_non_call<int64_t>, int);
INTERFACE_TESTABLE_FUNCTION_4(int, av_seek_frame, r_non_call<int>, AVFormatContext*, int, int64_t, int)
INTERFACE_TESTABLE_VOID_FUNCTION_1(avcodec_flush_buffers, non_call, AVCodecContext*)

