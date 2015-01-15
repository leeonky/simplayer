#ifndef CCFFMPEG_MOCK_TEST_FFMPEG_TEST
#define CCFFMPEG_MOCK_TEST_FFMPEG_TEST

#include "cctest/util.h"

class ffmpeg_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE(ffmpeg_test_class);
	CPPUNIT_TEST(media_stream_open);
	CPPUNIT_TEST(media_stream_close);
	CPPUNIT_TEST(open_close_ffmpeg_video_decoder);
	CPPUNIT_TEST(video_picture_create_destroy);
	CPPUNIT_TEST(soft_scale_init_context);
	CPPUNIT_TEST(soft_scale_scale);
	CPPUNIT_TEST(soft_scale_scale_frame);
	CPPUNIT_TEST(open_close_ffmpeg_audio_decoder);
	CPPUNIT_TEST(audio_frame_create_destroy);
	CPPUNIT_TEST(audio_converter_create_delete);
	CPPUNIT_TEST(audio_converter_create_delete2);
	CPPUNIT_TEST(soft_scaler_create_by_size);
	
	//=================================
	CPPUNIT_TEST(packet_node_create);
	CPPUNIT_TEST(packet_node_destruct_nodata);
	CPPUNIT_TEST(packet_node_destruct_free);
	CPPUNIT_TEST(media_stream_read_packet);
	CPPUNIT_TEST(media_stream_free_before_read_packet);
	CPPUNIT_TEST(contains_packet);
	CPPUNIT_TEST(test_decode_video2);
	CPPUNIT_TEST(test_decode_audio2);
	
	CPPUNIT_TEST_SUITE_END();
	
	void media_stream_open();
	void media_stream_close();
	void open_close_ffmpeg_video_decoder();
	void video_picture_create_destroy();
	void soft_scale_init_context();
	void soft_scale_scale();
	void soft_scale_scale_frame();
	void open_close_ffmpeg_audio_decoder();
	void audio_frame_create_destroy();
	void audio_converter_create_delete();
	void audio_converter_create_delete2();
	void soft_scaler_create_by_size();
	
	//=================================
	void packet_node_create();
	void packet_node_destruct_nodata();
	void packet_node_destruct_free();
	void media_stream_read_packet();
	void media_stream_free_before_read_packet();
	void contains_packet();
	void test_decode_video2();
	void test_decode_audio2();
}; 

#endif //CCFFMPEG_MOCK_TEST_FFMPEG_TEST

