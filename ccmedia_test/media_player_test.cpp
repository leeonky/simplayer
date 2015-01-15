#include "ccmedia_test/media_player_test.h"
#include "ccmedia/media_player.h"

using CCMEDIA::media_player;
using CCMEDIA::video_player;

void media_player_test_class::media_processor_openfile(){
	media_player md(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/ass1.mkv"));
	CPPUNIT_ASSERT_EQUAL(6, (int)md._stream._context->nb_streams);
}

void media_player_test_class::media_processor_openvideo(){
	video_player vd(TRACE_PARAMS(0, 0, 300, 200));
	media_player md(TRACE_PARAMS("/extend/rd_1/storage/users/leeonky/project/splayer_lib/ass1.mkv"));
	md.open_video(0, vd);
	CPPUNIT_ASSERT(md._vdec.raw_pointer());
}
