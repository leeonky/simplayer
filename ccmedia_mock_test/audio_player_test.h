#ifndef CCMEDIA_MOCK_TEST_AUDIO_PLAYER_TEST_H
#define CCMEDIA_MOCK_TEST_AUDIO_PLAYER_TEST_H

#include "cctest/util.h"

class audio_player_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (audio_player_test_class);
	CPPUNIT_TEST(audio_player_put_little);
	CPPUNIT_TEST(audio_player_put_little_and_little);
	CPPUNIT_TEST(audio_player_put_rest);
	CPPUNIT_TEST(audio_player_put_more_than_one);
	CPPUNIT_TEST(audio_player_put_more_than_two);
	CPPUNIT_TEST(audio_player_consume);
	CPPUNIT_TEST(audio_player_hold);
	CPPUNIT_TEST(audio_player_put_after_release);
	CPPUNIT_TEST(audio_player_hold_consume);
	CPPUNIT_TEST_SUITE_END();
	
    void setUp() {
		interface_testor::record_stack = false;
    }
    
    void tearDown() {
		interface_testor::record_stack = true;
    }

	void audio_player_put_little();
	void audio_player_put_little_and_little();
	void audio_player_put_rest();
	void audio_player_put_more_than_one();
	void audio_player_put_more_than_two();
	void audio_player_consume();
	void audio_player_hold();
	void audio_player_put_after_release();
	void audio_player_hold_consume();
};

#endif //CCMEDIA_MOCK_TEST_AUDIO_PLAYER_TEST_H
