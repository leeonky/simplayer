#ifndef CCMEDIA_TEST_DUBTITLE_DECODER_TEST_H
#define CCMEDIA_TEST_DUBTITLE_DECODER_TEST_H

#include "cctest/util.h"

class subtitle_decoder_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (subtitle_decoder_test_class);
	CPPUNIT_TEST(subtitle_decoder_srt_file);
	CPPUNIT_TEST(subtitle_decoder_ass_file);
	CPPUNIT_TEST(subtitle_decoder_srt_stream);
	CPPUNIT_TEST(subtitle_decoder_ass_stream);
	CPPUNIT_TEST_SUITE_END();

	void subtitle_decoder_srt_file();
	void subtitle_decoder_ass_file();
	void subtitle_decoder_srt_stream();
	void subtitle_decoder_ass_stream();
};

#endif //CCMEDIA_TEST_DUBTITLE_DECODER_TEST_H
