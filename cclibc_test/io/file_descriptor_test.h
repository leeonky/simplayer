#ifndef FILE_DESCRIPTOR_TEST_H_
#define FILE_DESCRIPTOR_TEST_H_

#include "cctest/util.h"

class file_descriptor_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (file_descriptor_test_class);
	CPPUNIT_TEST(file_descriptor_close);
	CPPUNIT_TEST(file_descriptor_block_read_write);
	CPPUNIT_TEST_SUITE_END();

	void file_descriptor_close();
	void file_descriptor_block_read_write();
};

#endif // FILE_DESCRIPTOR_TEST_H_
