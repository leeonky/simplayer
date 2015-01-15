#ifndef CCSDL2_VIDEO_TEST_H
#define CCSDL2_VIDEO_TEST_H

#include "cctest/util.h"

class video_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(video_test_class);
	CPPUNIT_TEST(window_init);
	CPPUNIT_TEST(window_size);
	CPPUNIT_TEST(window_postion);
	CPPUNIT_TEST(renderer_init);
	CPPUNIT_TEST(texture_init);
	CPPUNIT_TEST(texture_size);
	CPPUNIT_TEST_SUITE_END();

	void window_init();
	void window_size();
	void window_postion();
	void renderer_init();
	void texture_init();
	void texture_size();
};


#endif //CCSDL2_VIDEO_TEST_H

