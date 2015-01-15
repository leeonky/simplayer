#ifndef CCMEDIA_MOCK_TEST_MEDIA_PLAYER_TEST_H
#define CCMEDIA_MOCK_TEST_MEDIA_PLAYER_TEST_H

#include "cctest/util.h"

class media_player_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (media_player_test_class);
	CPPUNIT_TEST(media_processor_read_packet);
	CPPUNIT_TEST(media_processor_read_packet_twice);
	CPPUNIT_TEST_SUITE_END();

	void media_processor_read_packet();
	void media_processor_read_packet_twice();
};

#endif //CCMEDIA_MOCK_TEST_MEDIA_PLAYER_TEST_H
