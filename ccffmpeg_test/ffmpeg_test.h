#ifndef CCFFMPEG_TEST_FFMPEG_TEST
#define CCFFMPEG_TEST_FFMPEG_TEST

#include "cctest/util.h"

class ffmpeg_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE(ffmpeg_test_class);
	CPPUNIT_TEST(media_stream_open_dump);
	CPPUNIT_TEST(video_decoder_params);
	CPPUNIT_TEST(video_decoder_contains_data);
	CPPUNIT_TEST(audio_decoder_params);
	CPPUNIT_TEST(audio_decoder_contains_data);
	CPPUNIT_TEST(audio_converter_buffer_size);
	CPPUNIT_TEST_SUITE_END();

	void media_stream_open_dump();
	void video_decoder_params();
	void video_decoder_contains_data();
	void audio_decoder_params();
	void audio_decoder_contains_data();
	void audio_converter_buffer_size();
}; 

#endif //CCFFMPEG_TEST_FFMPEG_TEST

