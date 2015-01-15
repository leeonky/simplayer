#ifndef CCMEDIA_MOCK_TEST_VIDEO_PLAYER_TEST_H
#define CCMEDIA_MOCK_TEST_VIDEO_PLAYER_TEST_H

#include "cctest/util.h"

class video_player_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (video_player_test_class);
	CPPUNIT_TEST(video_player_allocate_screen_buffer);
	CPPUNIT_TEST(video_player_push_picture);
	CPPUNIT_TEST(video_player_consume);
	CPPUNIT_TEST(video_player_drain);
	CPPUNIT_TEST(video_player_clock);
	CPPUNIT_TEST(video_player_drop_frame);
	CPPUNIT_TEST(video_player_pause);
	CPPUNIT_TEST(video_player_resume);
	CPPUNIT_TEST(video_player_pause_time);
	CPPUNIT_TEST(video_player_pause_resume_time);
	CPPUNIT_TEST(video_player_change_clock);
	CPPUNIT_TEST_SUITE_END();
	
    void setUp() {
		interface_testor::record_stack = false;
    }
    
    void tearDown() {
		interface_testor::record_stack = true;
    }

	void video_player_allocate_screen_buffer();
	void video_player_push_picture();
	void video_player_consume();
	void video_player_drain();
	void video_player_clock();
	void video_player_drop_frame();
	void video_player_pause();
	void video_player_resume();
	void video_player_pause_time();
	void video_player_pause_resume_time();
	void video_player_change_clock();
};

#endif //CCMEDIA_MOCK_TEST_VIDEO_PLAYER_TEST_H
