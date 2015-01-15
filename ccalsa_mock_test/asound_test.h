#ifndef CCALSA_ASOUND_TEST_H
#define CCALSA_ASOUND_TEST_H

#include "cctest/util.h"

class asound_test_class: public CppUnit::TestFixture {
public:
	CPPUNIT_TEST_SUITE (asound_test_class);
	CPPUNIT_TEST(asound_open);
	CPPUNIT_TEST(asound_close);
	CPPUNIT_TEST(asound_parameters);
	CPPUNIT_TEST(asound_parameters_get_access_style);
	CPPUNIT_TEST(asound_parameters_set_access_style);
	CPPUNIT_TEST(asound_parameters_get_format);
	CPPUNIT_TEST(asound_parameters_set_format);
	CPPUNIT_TEST(asound_parameters_get_channels);
	CPPUNIT_TEST(asound_parameters_set_channels);
	CPPUNIT_TEST(asound_parameters_get_rate);
	CPPUNIT_TEST(asound_parameters_set_rate);
	CPPUNIT_TEST(asound_parameters_set_rate_near);
	CPPUNIT_TEST(asound_parameters_get_period);
	CPPUNIT_TEST(asound_parameters_set_period);
	CPPUNIT_TEST(asound_parameters_set_period_near);
	CPPUNIT_TEST(asound_parameters_period_max);
	CPPUNIT_TEST(asound_parameters_period_min);
	CPPUNIT_TEST(asound_parameters_period_time);
	CPPUNIT_TEST(asound_parameters_period_time_near);
	CPPUNIT_TEST(asound_parameters_period_time_max);
	CPPUNIT_TEST(asound_parameters_period_time_min);
	CPPUNIT_TEST(asound_parameters_set_channels_near);
	
	CPPUNIT_TEST(asound_parameters_install_to_device);
	CPPUNIT_TEST(asound_writei);
	CPPUNIT_TEST(asound_prepare);
	CPPUNIT_TEST(asound_shutdown);
	CPPUNIT_TEST(asound_parameters_set_buffer_size);
	CPPUNIT_TEST(asound_parameters_get_buffer_size);
	CPPUNIT_TEST(asound_parameters_set_max_buffer_size);
	CPPUNIT_TEST(asound_parameters_get_max_buffer_size);
	CPPUNIT_TEST(asound_parameters_set_buffer_size_near);
	CPPUNIT_TEST(asound_parameters_set_min_buffer_size);
	CPPUNIT_TEST(asound_parameters_get_min_buffer_size);
	CPPUNIT_TEST(asound_parameters_buffer_time);
	CPPUNIT_TEST(asound_parameters_buffer_time_near);
	CPPUNIT_TEST(asound_parameters_buffer_time_max);
	CPPUNIT_TEST(asound_parameters_buffer_time_min);
	CPPUNIT_TEST(asound_resume);
	CPPUNIT_TEST(asound_avail);
	CPPUNIT_TEST(asound_avail_update);
	CPPUNIT_TEST(asound_rewindable);
	CPPUNIT_TEST(asound_rewind);
	CPPUNIT_TEST(asound_forwardable);
	CPPUNIT_TEST(asound_forward);
	CPPUNIT_TEST(asound_delay);
	CPPUNIT_TEST(asound_wait_avail);
	CPPUNIT_TEST(asound_parameters_soft);
	CPPUNIT_TEST(asound_parameters_minimal_avail);
	CPPUNIT_TEST(asound_parameters_soft_install);
	CPPUNIT_TEST_SUITE_END();

	void asound_open();
	void asound_close();
	void asound_parameters();
	void asound_parameters_get_access_style();
	void asound_parameters_set_access_style();
	void asound_parameters_get_format();
	void asound_parameters_set_format();
	void asound_parameters_get_channels();
	void asound_parameters_set_channels();
	void asound_parameters_get_rate();
	void asound_parameters_set_rate();
	void asound_parameters_set_rate_near();
	void asound_parameters_get_period();
	void asound_parameters_set_period();
	void asound_parameters_set_period_near();
	void asound_parameters_period_max();
	void asound_parameters_period_min();
	void asound_parameters_period_time();
	void asound_parameters_period_time_near();
	void asound_parameters_period_time_max();
	void asound_parameters_period_time_min();
	void asound_parameters_set_channels_near();
	
	void asound_parameters_install_to_device();
	void asound_writei();
	void asound_prepare();
	void asound_shutdown();
	void asound_parameters_set_buffer_size();
	void asound_parameters_get_buffer_size();
	void asound_parameters_set_max_buffer_size();
	void asound_parameters_get_max_buffer_size();
	void asound_parameters_set_buffer_size_near();
	void asound_parameters_set_min_buffer_size();
	void asound_parameters_get_min_buffer_size();
	void asound_parameters_buffer_time();
	void asound_parameters_buffer_time_near();
	void asound_parameters_buffer_time_max();
	void asound_parameters_buffer_time_min();
	void asound_resume();
	void asound_avail();
	void asound_avail_update();
	void asound_rewindable();
	void asound_rewind();
	void asound_forwardable();
	void asound_forward();
	void asound_delay();
	void asound_wait_avail();
	void asound_parameters_soft();
	void asound_parameters_minimal_avail();
	void asound_parameters_soft_install();
};

#endif //CCALSA_ASOUND_TEST_H
