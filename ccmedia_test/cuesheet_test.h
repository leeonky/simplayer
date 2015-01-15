#ifndef CCMEDIA_TEST_CUSSHEET_TEST_H
#define CCMEDIA_TEST_CUSSHEET_TEST_H

#include "cctest/util.h"

class cuesheet_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (cuesheet_test_class);
	CPPUNIT_TEST(parse_no_PERFORMER);
	CPPUNIT_TEST(parse_empty_PERFORMER);
	CPPUNIT_TEST(parse_PERFORMER);
	CPPUNIT_TEST(parse_no_TITLE);
	CPPUNIT_TEST(parse_empty_TITLE);
	CPPUNIT_TEST(parse_TITLE);
	CPPUNIT_TEST(parse_FILE);
	CPPUNIT_TEST(parse_FILE_EXCEPTION);
	CPPUNIT_TEST(parse_TRACK1);
	CPPUNIT_TEST(parse_TRACK1_PERFORMER_TITLE);
	CPPUNIT_TEST(parse_TRACK1_TRACK2);
	CPPUNIT_TEST(parse_TRACK1_TRACK2_only_INDEX01);
	CPPUNIT_TEST_SUITE_END();

	void parse_no_PERFORMER();
	void parse_empty_PERFORMER();
	void parse_PERFORMER();
	
	void parse_no_TITLE();
	void parse_empty_TITLE();
	void parse_TITLE();
	void parse_FILE();
	void parse_FILE_EXCEPTION();
	void parse_TRACK1();
	void parse_TRACK1_PERFORMER_TITLE();
	void parse_TRACK1_TRACK2();
	void parse_TRACK1_TRACK2_only_INDEX01();
};

#endif //CCMEDIA_TEST_CUSSHEET_TEST_H
