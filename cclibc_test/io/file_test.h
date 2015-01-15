#ifndef FILE_TEST_H_
#define FILE_TEST_H_

#include "cctest/util.h"

class file_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (file_test_class);
	CPPUNIT_TEST(file_close);
	CPPUNIT_TEST(file_block_read_write);
	CPPUNIT_TEST(get_file_size);
	CPPUNIT_TEST(read_all_file);
	CPPUNIT_TEST_SUITE_END();

	void file_close();
	void file_block_read_write();
	void get_file_size();
	void read_all_file();
};

#endif // FILE_TEST_H_
