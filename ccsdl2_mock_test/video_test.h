#ifndef CCSDL2_VIDEO_TEST_H
#define CCSDL2_VIDEO_TEST_H

#include "cctest/util.h"

class video_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(video_test_class);
	CPPUNIT_TEST(video_create_des_window);
	CPPUNIT_TEST(window_size);
	CPPUNIT_TEST(window_position);
	CPPUNIT_TEST(renderer_init_quit);
	CPPUNIT_TEST(texture_init_quit);
	CPPUNIT_TEST(sdl_rect_properties);
	CPPUNIT_TEST(texture_lock_unlock);
	CPPUNIT_TEST(renderer_copy);
	CPPUNIT_TEST(renderer_present);
	CPPUNIT_TEST(sdl_color_properties);
	CPPUNIT_TEST(texture_lock_return_obj);
	CPPUNIT_TEST(texture_lock_all_return_obj);
	CPPUNIT_TEST(sdl_rect_intersect_self);
	CPPUNIT_TEST(sdl_rect_intersect_same);
	CPPUNIT_TEST(sdl_rect_contains);
	CPPUNIT_TEST(sdl_rect_intersect);
	CPPUNIT_TEST_SUITE_END();

	void video_create_des_window();
	void window_size();
	void window_position();
	void renderer_init_quit();
	void texture_init_quit();
	void sdl_rect_properties();
	void texture_lock_unlock();
	void renderer_copy();
	void renderer_present();
	void sdl_color_properties();
	void texture_lock_return_obj();
	void texture_lock_all_return_obj();
	void sdl_rect_intersect_self();
	void sdl_rect_intersect_same();
	void sdl_rect_contains();
	void sdl_rect_intersect();
};


#endif //CCSDL2_VIDEO_TEST_H

