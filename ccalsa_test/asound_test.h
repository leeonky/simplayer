#ifndef CCALSA_ASOUND_TEST_H
#define CCALSA_ASOUND_TEST_H

#include "cctest/util.h"

class asound_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (asound_test_class);
	CPPUNIT_TEST(asound_open_close);
	CPPUNIT_TEST(asound_access_mode);
	CPPUNIT_TEST(asound_data_format);
	CPPUNIT_TEST(asound_channels);
	CPPUNIT_TEST(asound_rate);
	CPPUNIT_TEST(asound_buffer_size_mm);
	CPPUNIT_TEST(asound_buffer_size);
	CPPUNIT_TEST(asound_period_size_mm);
	CPPUNIT_TEST(asound_period_size);
	CPPUNIT_TEST(asound_buffer_time_mm);
	CPPUNIT_TEST(asound_buffer_time);
	CPPUNIT_TEST(asound_period_time_mm);
	CPPUNIT_TEST(asound_period_time);
	CPPUNIT_TEST(asound_praram_write);
	CPPUNIT_TEST(asound_writei);
	CPPUNIT_TEST(asound_waited_period);
	CPPUNIT_TEST_SUITE_END();

	void asound_open_close();
	void asound_access_mode();
	void asound_data_format();
	void asound_channels();
	void asound_rate();
	void asound_buffer_size_mm();
	void asound_buffer_size();
	void asound_period_size_mm();
	void asound_period_size();
	void asound_buffer_time_mm();
	void asound_buffer_time();
	void asound_period_time_mm();
	void asound_period_time();
	void asound_praram_write();
	void asound_writei();
	void asound_waited_period();
};

#endif //CCALSA_ASOUND_TEST_H
