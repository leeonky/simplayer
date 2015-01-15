#include "ccffmpeg_mock_test/ffmpeg_test.h"
#include "ccffmpeg/ffmpeg.h"

using CCFFMPEG::media_packet;
using CCFFMPEG::media_stream;
using CCFFMPEG::video_decoder;
using CCFFMPEG::audio_decoder;
using CCFFMPEG::video_frame;
using CCFFMPEG::audio_frame;
using CCFFMPEG::soft_scaler;
using CCFFMPEG::audio_converter;

void ffmpeg_test_class::media_stream_open(){
	interface_testor::clear();
	const char *f = "file";
	media_stream ftxt(TRACE_PARAMS(f));
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_register_all"));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("avformat_open_input"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal((void*)f));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(3).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("avformat_find_stream_info"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(ftxt._context));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(NULL));
}

void ffmpeg_test_class::media_stream_close(){
	void *p;
	{	
		const char *f = "file";
		media_stream ftxt(TRACE_PARAMS(f));
		interface_testor::clear();
		p = &ftxt._context;
	}
	CPPUNIT_ASSERT(interface_testor::method(0).equal("avformat_close_input"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(p));
}

void ffmpeg_test_class::open_close_ffmpeg_video_decoder(){
	void *p;
	{
		const char *f = "file";
		media_stream ftxt(TRACE_PARAMS(f));
		AVCodecContext ctxt;
		ctxt.codec_type =  AVMEDIA_TYPE_VIDEO;
		ftxt._context->streams[0]->codec = &ctxt;
		p = &ctxt;
		ctxt.codec_id = AV_CODEC_ID_AC3;
		interface_testor::clear();
		video_decoder vdec(TRACE_PARAMS(ftxt, 0));
		
		CPPUNIT_ASSERT(interface_testor::method(0).equal("avcodec_find_decoder"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(AV_CODEC_ID_AC3));
		CPPUNIT_ASSERT(interface_testor::method(1).equal("avcodec_open2"));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
	}
	CPPUNIT_ASSERT(interface_testor::method(2).equal("avcodec_close"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(p));
}

void ffmpeg_test_class::video_picture_create_destroy(){
	void *p;
	{
		interface_testor::clear();
		video_frame TRACE_CONSTRUCT(frm);
		p = &frm._frame;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("av_frame_alloc"));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("av_frame_free"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ffmpeg_test_class::soft_scale_init_context(){
	const char *f = "file";
	void *p;
	media_stream ftxt(TRACE_PARAMS(f));
	AVCodecContext ctxt;
	ctxt.codec_type =  AVMEDIA_TYPE_VIDEO;
	ftxt._context->streams[0]->codec = &ctxt;
	ctxt.codec_id = AV_CODEC_ID_AC3;
	ctxt.width=200;
	ctxt.height=100;
	video_decoder vdec(TRACE_PARAMS(ftxt, 0));
	interface_testor::clear();
	{
		soft_scaler sslr(TRACE_PARAMS(vdec, 400, 300, AV_PIX_FMT_YUV420P, SWS_BICUBIC));
		p = sslr._context;
	}
	CPPUNIT_ASSERT(interface_testor::method(0).equal("sws_getContext"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(vdec.width()));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(vdec.height()));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(vdec._context->pix_fmt));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(400));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(300));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(5).equal(AV_PIX_FMT_YUV420P));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(6).equal(SWS_BICUBIC));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(7).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(8).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(9).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("sws_freeContext"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ffmpeg_test_class::soft_scale_scale(){
	const char *f = "file";
	media_stream ftxt(TRACE_PARAMS(f));
	AVCodecContext ctxt;
	ctxt.codec_type =  AVMEDIA_TYPE_VIDEO;
	ftxt._context->streams[0]->codec = &ctxt;
	ctxt.codec_id = AV_CODEC_ID_AC3;
	ctxt.height = 100;
	video_decoder vdec(TRACE_PARAMS(ftxt, 0));
	soft_scaler sslr(TRACE_PARAMS(vdec, 400, 300, AV_PIX_FMT_YUV420P, SWS_BICUBIC));
	uint8_t pix_fuf[128];
	const void * const sbufs[]={pix_fuf};
	void * const dbufs[]={pix_fuf};
	int l;
	interface_testor::clear();
	sslr.scale(sbufs, &l, dbufs, &l);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("sws_scale"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(sslr._context));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(sbufs));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(0));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(vdec.height()));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(5).equal(dbufs));
}

void ffmpeg_test_class::soft_scale_scale_frame(){
	const char *f = "file";
	media_stream ftxt(TRACE_PARAMS(f));
	AVCodecContext ctxt;
	ctxt.codec_type =  AVMEDIA_TYPE_VIDEO;
	ftxt._context->streams[0]->codec = &ctxt;
	ctxt.codec_id = AV_CODEC_ID_AC3;
	ctxt.height = 100;
	video_decoder vdec(TRACE_PARAMS(ftxt, 0));
	soft_scaler sslr(TRACE_PARAMS(vdec, 400, 300, AV_PIX_FMT_YUV420P, SWS_BICUBIC));
	uint8_t pix_fuf[128];
	void * const dbufs[]={pix_fuf};
	int l;
	video_frame TRACE_CONSTRUCT(frm);
	interface_testor::clear();
	sslr.scale(frm, dbufs, &l);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("sws_scale"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(sslr._context));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(0));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(vdec.height()));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(5).equal(dbufs));
}

void ffmpeg_test_class::open_close_ffmpeg_audio_decoder(){
	void *p;
	{
		const char *f = "file";
		media_stream ftxt(TRACE_PARAMS(f));
		AVCodecContext ctxt;
		ctxt.codec_type =  AVMEDIA_TYPE_AUDIO;
		ftxt._context->streams[0]->codec = &ctxt;
		p = &ctxt;
		ctxt.codec_id = AV_CODEC_ID_AC3;
		interface_testor::clear();
		audio_decoder adec(TRACE_PARAMS(ftxt, 0));
		
		CPPUNIT_ASSERT(interface_testor::method(0).equal("avcodec_find_decoder"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(AV_CODEC_ID_AC3));
		CPPUNIT_ASSERT(interface_testor::method(1).equal("avcodec_open2"));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
	}
	CPPUNIT_ASSERT(interface_testor::method(2).equal("avcodec_close"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(p));
}

void ffmpeg_test_class::audio_frame_create_destroy(){
	void *p;
	{
		interface_testor::clear();
		audio_frame TRACE_CONSTRUCT(frm);
		p = &frm._frame;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("av_frame_alloc"));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("av_frame_free"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void ffmpeg_test_class::audio_converter_create_delete(){
	void *p1,*p2;
	{
		const char *f = "file";
		media_stream ftxt(TRACE_PARAMS(f));
		AVCodecContext ctxt;
		ctxt.codec_type =  AVMEDIA_TYPE_AUDIO;
		ftxt._context->streams[0]->codec = &ctxt;
		ctxt.codec_id = AV_CODEC_ID_AC3;
		audio_decoder adec(TRACE_PARAMS(ftxt, 0));
		adec._context->sample_rate = 48000;
		adec._context->channel_layout = AV_CH_LAYOUT_5POINT1;
		adec._context->sample_fmt = AV_SAMPLE_FMT_S16P;
		interface_testor::clear();
		audio_converter acvt(TRACE_PARAMS(adec, AV_CH_LAYOUT_7POINT1, AV_SAMPLE_FMT_S16, 192000));
		p1 = acvt._context;
		p2 = &acvt._context;
	}
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_get_channel_layout_nb_channels"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(AV_CH_LAYOUT_7POINT1));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("av_get_bytes_per_sample"));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("swr_alloc_set_opts"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(AV_CH_LAYOUT_7POINT1));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(2).equal(AV_SAMPLE_FMT_S16));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(3).equal(192000));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(4).equal(AV_CH_LAYOUT_5POINT1));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(5).equal(AV_SAMPLE_FMT_S16P));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(6).equal(48000));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(7).equal(0));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(8).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("swr_init"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(p1));
	CPPUNIT_ASSERT(interface_testor::method(4).equal("swr_free"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(p2));
}

void ffmpeg_test_class::audio_converter_create_delete2(){
	interface_testor::clear();
	audio_converter acvt(TRACE_PARAMS(AV_CH_LAYOUT_5POINT1, AV_SAMPLE_FMT_U8,
		44100, AV_CH_LAYOUT_7POINT1, AV_SAMPLE_FMT_S16, 192000));
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_get_channel_layout_nb_channels"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(AV_CH_LAYOUT_7POINT1));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("av_get_bytes_per_sample"));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("swr_alloc_set_opts"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(AV_CH_LAYOUT_7POINT1));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(2).equal(AV_SAMPLE_FMT_S16));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(3).equal(192000));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(4).equal(AV_CH_LAYOUT_5POINT1));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(5).equal(AV_SAMPLE_FMT_U8));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(6).equal(44100));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(7).equal(0));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(8).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(3).equal("swr_init"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(acvt._context));
}

void ffmpeg_test_class::soft_scaler_create_by_size(){
	interface_testor::clear();
	soft_scaler sslr(TRACE_PARAMS(10, 20, AV_PIX_FMT_RGB24, 400, 300, AV_PIX_FMT_YUV420P, SWS_BICUBIC));
	CPPUNIT_ASSERT(interface_testor::method(0).equal("sws_getContext"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(10));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(20));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(AV_PIX_FMT_RGB24));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(400));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(300));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(5).equal(AV_PIX_FMT_YUV420P));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(6).equal(SWS_BICUBIC));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(7).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(8).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(9).equal(NULL));
}

//=================================
void ffmpeg_test_class::packet_node_create(){
	interface_testor::clear();
	media_packet pkt;
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_init_packet"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&pkt._packet));
}

void ffmpeg_test_class::packet_node_destruct_nodata(){
	interface_testor::clear();
	void *p;
	{
		media_packet pkt;
		p = &pkt._packet;
	}
	CPPUNIT_ASSERT(interface_testor::last_method(0).equal("av_init_packet"));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(0).equal(p));
}

void ffmpeg_test_class::packet_node_destruct_free(){
	interface_testor::clear();
	void *p;
	{
		media_packet pkt;
		pkt._has_data = true;
	}
	CPPUNIT_ASSERT(interface_testor::last_method(0).equal("av_free_packet"));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(0).equal(p));
}

void ffmpeg_test_class::media_stream_read_packet(){
	media_stream ftxt(TRACE_PARAMS("file"));
	media_packet pkt;
	interface_testor::clear();
	ftxt.read_packet(pkt);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_read_frame"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(ftxt._context));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(&pkt._packet));
}

void ffmpeg_test_class::media_stream_free_before_read_packet(){
	media_stream ftxt(TRACE_PARAMS("file"));
	media_packet pkt;
	ftxt.read_packet(pkt);
	interface_testor::clear();
	ftxt.read_packet(pkt);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_free_packet"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&pkt._packet));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("av_read_frame"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(ftxt._context));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(&pkt._packet));
}

void ffmpeg_test_class::contains_packet(){
	media_stream stm(TRACE_PARAMS("file"));
	AVCodecContext ctxt;
	ctxt.codec_type =  AVMEDIA_TYPE_VIDEO;
	stm._context->streams[0]->codec = &ctxt;
	video_decoder vdec(TRACE_PARAMS(stm, 0));
	media_packet pkt;
	pkt._packet.stream_index = 1;
	CPPUNIT_ASSERT(!vdec.contains_data(pkt));
	pkt._has_data = true;
	pkt._packet.stream_index = 0;
	CPPUNIT_ASSERT(vdec.contains_data(pkt));
}

void ffmpeg_test_class::test_decode_video2(){
	media_stream stm(TRACE_PARAMS("file"));
	AVCodecContext ctxt;
	ctxt.codec_type =  AVMEDIA_TYPE_VIDEO;
	stm._context->streams[0]->codec = &ctxt;
	video_decoder vdec(TRACE_PARAMS(stm, 0));
	media_packet pkt;
	video_frame TRACE_CONSTRUCT(frm);
	interface_testor::clear();
	vdec.decode_frame(pkt, frm);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("avcodec_decode_video2"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(vdec._context));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(frm._frame));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(&pkt._packet));
}

void ffmpeg_test_class::test_decode_audio2(){
	media_stream stm(TRACE_PARAMS("file"));
	AVCodecContext ctxt;
	ctxt.codec_type =  AVMEDIA_TYPE_AUDIO;
	stm._context->streams[0]->codec = &ctxt;
	audio_decoder adec(TRACE_PARAMS(stm, 0));
	media_packet pkt;
	audio_frame TRACE_CONSTRUCT(frm);
	interface_testor::clear();
	adec.decode_frame(pkt, frm);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("avcodec_decode_audio4"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(adec._context));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(frm._frame));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(&pkt._packet));
}
