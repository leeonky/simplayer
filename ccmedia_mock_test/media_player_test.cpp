#include "ccmedia_mock_test/media_player_test.h"
#include "ccmedia/media_player.h"

using CCMEDIA::media_player;
using CCMEDIA::video_player;

void media_player_test_class::media_processor_read_packet(){
	media_player md(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/ass1.mkv"));
	interface_testor::clear();
	md.read_packet();
	CPPUNIT_ASSERT(md._pkt_list.size() == 1);
	CPPUNIT_ASSERT(interface_testor::last_method(0).equal("av_read_frame"));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(0).equal(md._stream._context));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(1).equal(&md._pkt_list.first()->_packet));
}

void media_player_test_class::media_processor_read_packet_twice(){
	media_player md(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/ass1.mkv"));
	interface_testor::clear();
	md.read_packet();
	interface_testor::clear();
	md.read_packet();
	CPPUNIT_ASSERT(md._pkt_list.size() == 1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("av_free_packet"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&md._pkt_list.first()->_packet));
	CPPUNIT_ASSERT(interface_testor::last_method(0).equal("av_read_frame"));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(0).equal(md._stream._context));
	CPPUNIT_ASSERT(interface_testor::last_method(0).parameter(1).equal(&md._pkt_list.first()->_packet));
}
