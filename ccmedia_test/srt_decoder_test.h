#ifndef CCMEDIA_TEST_SRT_DECODER_TEST_H
#define CCMEDIA_TEST_SRT_DECODER_TEST_H

#include "cctest/util.h"

class srt_decoder_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (srt_decoder_test_class);
	CPPUNIT_TEST(srt_decoder_new_empty);
	CPPUNIT_TEST(srt_decoder_extend_1);
	CPPUNIT_TEST(srt_decoder_extend_same);
	CPPUNIT_TEST(srt_decoder_new_with_file);
	CPPUNIT_TEST(srt_decoder_first_render_nothing);
	CPPUNIT_TEST(srt_decoder_first_render);
	CPPUNIT_TEST(srt_decoder_first_render_gbk);
	CPPUNIT_TEST(srt_decoder_render_nothing);
	CPPUNIT_TEST(srt_decoder_render_again);
	CPPUNIT_TEST(srt_decoder_render_new);
	CPPUNIT_TEST_SUITE_END();
	
	void srt_decoder_new_empty();
	void srt_decoder_extend_1();
	void srt_decoder_extend_same();
	void srt_decoder_new_with_file();
	void srt_decoder_first_render_nothing();
	void srt_decoder_first_render();
	void srt_decoder_first_render_gbk();
	void srt_decoder_render_nothing();
	void srt_decoder_render_again();
	void srt_decoder_render_new();
};

#endif //CCMEDIA_TEST_SRT_DECODER_TEST_H
