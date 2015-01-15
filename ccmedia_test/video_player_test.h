#ifndef CCMEDIA_TEST_VIDEO_PLAYER_TEST_H
#define CCMEDIA_TEST_VIDEO_PLAYER_TEST_H

#include "cctest/util.h"

class video_player_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (video_player_test_class);
	CPPUNIT_TEST(video_player_new_delete);
	CPPUNIT_TEST(video_player_set_scaler_match);
	CPPUNIT_TEST(video_player_set_scaler_16_9_to_4_3);
	CPPUNIT_TEST(video_player_set_scaler_4_3_to_16_9);
	CPPUNIT_TEST(video_player_set_scaler_same);
	CPPUNIT_TEST(video_player_set_scaler_v_same);
	CPPUNIT_TEST(video_player_set_scaler_h_same);
	CPPUNIT_TEST(video_player_play_video_same);
	CPPUNIT_TEST(video_player_play_video_v_same);
	CPPUNIT_TEST(video_player_play_video_v_same_not_align);
	CPPUNIT_TEST(video_player_play_video_h_same);
	CPPUNIT_TEST(video_player_play_video_same_block);
	CPPUNIT_TEST(video_player_seek_frm);
	CPPUNIT_TEST_SUITE_END();

	void video_player_new_delete();
	void video_player_set_scaler_match();
	void video_player_set_scaler_16_9_to_4_3();
	void video_player_set_scaler_4_3_to_16_9();
	void video_player_set_scaler_same();
	void video_player_set_scaler_v_same();
	void video_player_set_scaler_h_same();
	void video_player_play_video_same();
	void video_player_play_video_v_same();
	void video_player_play_video_v_same_not_align();
	void video_player_play_video_h_same();
	void video_player_play_video_same_block();
	void video_player_seek_frm();
};

#endif //CCMEDIA_TEST_VIDEO_PLAYER_TEST_H
