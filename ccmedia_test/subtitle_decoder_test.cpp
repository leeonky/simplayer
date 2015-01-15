#include "ccmedia_test/subtitle_decoder_test.h"
#include "ccmedia/subtitle_decoder.h"

using CCMEDIA::external_subtitle_decoder;
using CCMEDIA::media_subtitle_decoder;
using CCMEDIA::srt_decoder;
using CCMEDIA::srt_stream;
using CCMEDIA::ass_decoder;
using CCMEDIA::ass_stream;
using CCSDL2::sdl_color;
using CCFFMPEG::media_stream;

void subtitle_decoder_test_class::subtitle_decoder_srt_file(){
	external_subtitle_decoder sdec(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/test.srt", 800, 400, 800, 400, 
		"/usr/share/fonts/truetype/DroidSansFallback.ttf", 20, sdl_color(255, 255, 255)));
	CPPUNIT_ASSERT(typeid(srt_decoder) == typeid(*sdec._decoder.raw_pointer()));
	CPPUNIT_ASSERT(typeid(srt_stream) == typeid(*sdec._stream.raw_pointer()));
}

void subtitle_decoder_test_class::subtitle_decoder_ass_file(){
	external_subtitle_decoder sdec(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/test.ass",
		800, 600, 800, 600, "/usr/share/fonts/truetype/DroidSansFallback.ttf", 20, sdl_color(255, 255, 255)));
	CPPUNIT_ASSERT(typeid(ass_decoder) == typeid(*sdec._decoder.raw_pointer()));
	CPPUNIT_ASSERT(typeid(ass_stream) == typeid(*sdec._stream.raw_pointer()));
}

void subtitle_decoder_test_class::subtitle_decoder_srt_stream(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/multiple_sub_text.mkv"));
	media_subtitle_decoder sdec(TRACE_PARAMS(mc, 3, 800, 400, 800, 400,
		"/usr/share/fonts/truetype/DroidSansFallback.ttf", 1.0, sdl_color(255, 255, 255)));
	CPPUNIT_ASSERT(typeid(srt_decoder) == typeid(*sdec._decoder.raw_pointer()));
	CPPUNIT_ASSERT(typeid(srt_stream) == typeid(*sdec._stream.raw_pointer()));
}

void subtitle_decoder_test_class::subtitle_decoder_ass_stream(){
	media_stream mc(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/ass1.mkv"));
	media_subtitle_decoder sdec(TRACE_PARAMS(mc, 2, 800, 400, 800, 400,
		"/usr/share/fonts/truetype/DroidSansFallback.ttf", 1.0, sdl_color(255, 255, 255)));
	CPPUNIT_ASSERT(typeid(ass_decoder) == typeid(*sdec._decoder.raw_pointer()));
	CPPUNIT_ASSERT(typeid(ass_stream) == typeid(*sdec._stream.raw_pointer()));
}

