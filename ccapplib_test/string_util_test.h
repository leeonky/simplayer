#ifndef CCAPPLIB_TEST_MULTILINE_STRING
#define CCAPPLIB_TEST_MULTILINE_STRING

#include "cctest/util.h"

class string_util_test_class: public CppUnit::TestFixture{
public:
	CPPUNIT_TEST_SUITE(string_util_test_class);
	CPPUNIT_TEST(utf2uni);
	CPPUNIT_TEST(wrap_empty);
	CPPUNIT_TEST(wrap_no_wrap_1_char);
	CPPUNIT_TEST(string_clear);
	CPPUNIT_TEST(wrap_no_wrap);
	CPPUNIT_TEST(wrap_no_wrap_too_long);
	CPPUNIT_TEST(wrap_only_stopper_letter);
	CPPUNIT_TEST(wrap_stopper_cjk);
	CPPUNIT_TEST(wrap_stopper_letters);
	CPPUNIT_TEST(wrap_stopper_cjks);
	CPPUNIT_TEST(wrap_stopper_letters_too_long);
	CPPUNIT_TEST(wrap_stopper_cjks_too_long);
	CPPUNIT_TEST(wrap_stopper_last_one_stopper);
	CPPUNIT_TEST(wrap_letter_space);
	CPPUNIT_TEST(wrap_cjk_space);
	CPPUNIT_TEST(wrap_letter_n);
	CPPUNIT_TEST(splite_line_r);
	CPPUNIT_TEST(splite_line_n);
	CPPUNIT_TEST(splite_line_rn);
	CPPUNIT_TEST(string_end_with);
	CPPUNIT_TEST(string_end_with_i);
	CPPUNIT_TEST_SUITE_END();
	
	void utf2uni();
	void wrap_empty();
	void wrap_no_wrap_1_char();
	void string_clear();
	void wrap_no_wrap();
	void wrap_no_wrap_too_long();
	void wrap_only_stopper_letter();
	void wrap_stopper_cjk();
	void wrap_stopper_letters();
	void wrap_stopper_cjks();
	void wrap_stopper_letters_too_long();
	void wrap_stopper_cjks_too_long();
	void wrap_stopper_last_one_stopper();
	void wrap_letter_space();
	void wrap_cjk_space();
	void wrap_letter_n();
	void splite_line_r();
	void splite_line_n();
	void splite_line_rn();
	void string_end_with();
	void string_end_with_i();
};

#endif //CCAPPLIB_TEST_MULTILINE_STRING
