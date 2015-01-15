#ifndef CCMEDIA_TEST_MEDIA_PLAYER_TEST_H
#define CCMEDIA_TEST_MEDIA_PLAYER_TEST_H

#include "cctest/util.h"

class media_player_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (media_player_test_class);
	CPPUNIT_TEST(media_processor_openfile);
	CPPUNIT_TEST(media_processor_openvideo);
	CPPUNIT_TEST_SUITE_END();

	void media_processor_openfile();
	void media_processor_openvideo();
};

#endif //CCMEDIA_TEST_MEDIA_PLAYER_TEST_H
