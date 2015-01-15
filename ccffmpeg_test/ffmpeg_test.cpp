#include "ccffmpeg_test/ffmpeg_test.h"
#include "ccffmpeg/ffmpeg.h"

using CCAPPLIB::string_array;
using namespace CCFFMPEG;

void ffmpeg_test_class::media_stream_open_dump(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v"));
	string_array<> strs;
	mc.media_info(strs);
	CPPUNIT_ASSERT(strs.size() == 2);
	for(size_t i=0; i<strs.size(); i++){
		printf("%s\n", strs[i]);
	}
	CPPUNIT_ASSERT(strstr(strs[0], "0: Video") == strs[0]);
	CPPUNIT_ASSERT(strstr(strs[1], "1: Audio") == strs[1]);
}

void ffmpeg_test_class::video_decoder_params(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v"));
	video_decoder dec(TRACE_PARAMS(mc, 0));
	CPPUNIT_ASSERT(dec.width() == 192);
	CPPUNIT_ASSERT(dec.height() == 240);
	CPPUNIT_ASSERT(dec.duration() == 4900);
}

void ffmpeg_test_class::video_decoder_contains_data(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v"));
	video_decoder dec(TRACE_PARAMS(mc, 0));
	media_packet pkt;
	CPPUNIT_ASSERT(mc.read_packet(pkt));
	CPPUNIT_ASSERT(dec.contains_data(pkt));
}

void ffmpeg_test_class::audio_decoder_params(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v"));
	audio_decoder dec(TRACE_PARAMS(mc, 1));
	CPPUNIT_ASSERT(dec.rate() == 48000);
	CPPUNIT_ASSERT(dec.channels() == 2);
}

void ffmpeg_test_class::audio_decoder_contains_data(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v"));
	audio_decoder dec(TRACE_PARAMS(mc, 1));
	media_packet pkt;
	while(mc.read_packet(pkt)){
		if(pkt._packet.stream_index == 1){
			break;
		}
	}
	CPPUNIT_ASSERT(dec.contains_data(pkt));
}

void ffmpeg_test_class::audio_converter_buffer_size(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/sample_mpeg2.m2v"));
	audio_decoder dec(TRACE_PARAMS(mc, 1));
	media_packet pkt;
	while(mc.read_packet(pkt)){
		if(pkt._packet.stream_index == 1){
			break;
		}
	}
	audio_frame TRACE_CONSTRUCT(af);
	dec.decode_frame(pkt, af);
	audio_converter acvt(TRACE_PARAMS(dec, AV_CH_LAYOUT_7POINT1, AV_SAMPLE_FMT_S16, 192000));
	CPPUNIT_ASSERT_EQUAL(acvt.required_buffer_size(af), (size_t)(af._frame->nb_samples*4*2*8));
}
