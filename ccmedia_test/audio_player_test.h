#ifndef CCMEDIA_MOCK_TEST_AUDIO_PLAYER_TEST_H
#define CCMEDIA_MOCK_TEST_AUDIO_PLAYER_TEST_H

#include "cctest/util.h"

class audio_player_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (audio_player_test_class);
	CPPUNIT_TEST(audio_player_params);
	CPPUNIT_TEST(audio_player_cvt_fmt);
	CPPUNIT_TEST(audio_player_cvt_rate);
	CPPUNIT_TEST_SUITE_END();
	
	void audio_player_params();
	void audio_player_cvt_fmt();
	void audio_player_cvt_rate();
};

#endif //CCMEDIA_MOCK_TEST_AUDIO_PLAYER_TEST_H
