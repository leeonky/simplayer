#include "ccmedia_mock_test/ass_decoder_test.h"
#include "ccmedia/ass_decoder.h"
#include "ccsdl2/video.h"

using CCMEDIA::ass_decoder;
using CCMEDIA::ass_stream;
using CCAPPLIB::time_bucket;

void ass_decoder_test_class::decoder_init_config(){
	void *p1, *p2;
	{
		interface_testor::clear();
		ass_decoder ass(TRACE_PARAMS(300, 200, 200, 100, "Sans", "Fontconfig", true));
		p1 = ass._lib;
		p2 = ass._renderer;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("ass_library_init"));
		CPPUNIT_ASSERT(interface_testor::method(1).equal("ass_renderer_init"));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(ass._lib));
		CPPUNIT_ASSERT(interface_testor::method(2).equal("ass_set_frame_size"));
		CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(ass._renderer));
		CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(300));
		CPPUNIT_ASSERT(interface_testor::method(2).parameter(2).equal(200));
		CPPUNIT_ASSERT(interface_testor::method(3).equal("ass_set_fonts"));
		CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(ass._renderer));
		CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal((char*)NULL));
		CPPUNIT_ASSERT(interface_testor::method(3).parameter(2).equal("Sans"));
		CPPUNIT_ASSERT(interface_testor::method(3).parameter(3).equal(1));
		CPPUNIT_ASSERT(interface_testor::method(3).parameter(4).equal("Fontconfig"));
		CPPUNIT_ASSERT(interface_testor::method(3).parameter(5).equal(1));
	}
	CPPUNIT_ASSERT(interface_testor::method(4).equal("ass_renderer_done"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(p2));
	CPPUNIT_ASSERT(interface_testor::method(5).equal("ass_library_done"));
	CPPUNIT_ASSERT(interface_testor::method(5).parameter(0).equal(p1));
}

void ass_decoder_test_class::ass_track_load(){
	/*
	void *p;
	{
		ass_decoder ass(TRACE_PARAMS(300, 200, 200, 100, "Sans", "Fontconfig", true));
		interface_testor::clear();
		ass_stream track(TRACE_PARAMS(ass, "/extend/rd_1/storage/users/leeonky/project/splayer_lib/test.ass", "UTF-8"));
		p = track._track;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("ass_read_file"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(ass._lib));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("/extend/rd_1/storage/users/leeonky/project/splayer_lib/test.ass"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal("UTF-8"));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("ass_free_track"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
	*/
}

void ass_decoder_test_class::ass_track_new(){
	void *p;
	{
		char buf[1024];
		ass_decoder ass(TRACE_PARAMS(300, 200, 200, 100, "Sans", "Fontconfig", true));
		interface_testor::clear();
		ass_stream track(TRACE_PARAMS(ass, buf, sizeof(buf)));
		p = track._track;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("ass_new_track"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(ass._lib));
		CPPUNIT_ASSERT(interface_testor::method(1).equal("ass_process_codec_private"));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(track._track));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(buf));
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(2).equal(sizeof(buf)));
	}
	CPPUNIT_ASSERT(interface_testor::method(2).equal("ass_free_track"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(p));
}

void ass_decoder_test_class::ass_track_append(){
	ass_decoder ass(TRACE_PARAMS(300, 200, 200, 100, "Sans", "Fontconfig", true));
	interface_testor::clear();
	ass_stream track(TRACE_PARAMS(ass, "/extend/rd_1/storage/users/leeonky/project/splayer_lib/test.ass", "UTF-8"));
	const char *buf = "subtitle";
	interface_testor::clear();
	track.extend_content(buf, time_bucket(100, 200));
	CPPUNIT_ASSERT(interface_testor::method(0).equal("ass_process_chunk"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(track._track));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(100));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(200));
}

