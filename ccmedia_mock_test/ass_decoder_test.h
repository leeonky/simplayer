#ifndef CCMEDIA_MOCK_TEST_ASS_DECODER_TEST_H
#define CCMEDIA_MOCK_TEST_ASS_DECODER_TEST_H

#include "cctest/util.h"

class ass_decoder_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (ass_decoder_test_class);
	CPPUNIT_TEST(decoder_init_config);
	CPPUNIT_TEST(ass_track_load);
	CPPUNIT_TEST(ass_track_new);
	CPPUNIT_TEST(ass_track_append);
	CPPUNIT_TEST_SUITE_END();

	void decoder_init_config();
	void ass_track_load();
	void ass_track_new();
	void ass_track_append();
};

#endif //CCMEDIA_MOCK_TEST_ASS_DECODER_TEST_H
