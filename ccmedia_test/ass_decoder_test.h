#ifndef CCMEDIA_TEST_ASS_DECODER_TEST_H
#define CCMEDIA_TEST_ASS_DECODER_TEST_H

#include "cctest/util.h"

class ass_decoder_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (ass_decoder_test_class);
	CPPUNIT_TEST(decoder_init);
	CPPUNIT_TEST_SUITE_END();

	void decoder_init();
};

#endif //CCMEDIA_TEST_ASS_DECODER_TEST_H
