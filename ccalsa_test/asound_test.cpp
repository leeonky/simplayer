#include <cstring>
#include "ccalsa_test/asound_test.h"
#include "ccalsa/asound.h"

using CCLIBC::exception;
using CCALSA::alsa_sound;
using CCALSA::alsa_error;
using CCALSA::sound_parameters;
using CCALSA::soft_parameters;

void asound_test_class::asound_open_close(){
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback));
		CPPUNIT_ASSERT_THROW(alsa_sound(TRACE_PARAMS("xxx", alsa_sound::playback)), alsa_error*);
		CPPUNIT_ASSERT_THROW(alsa_sound(TRACE_PARAMS("hw:0", alsa_sound::playback)), alsa_error*);
	}
	CPPUNIT_ASSERT_NO_THROW(alsa_sound(TRACE_PARAMS("hw:0", alsa_sound::playback)));
}

void asound_test_class::asound_access_mode(){
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::mmap_interleaved);
		CPPUNIT_ASSERT(param.access_mode() == sound_parameters::mmap_interleaved);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		CPPUNIT_ASSERT(param.access_mode() == sound_parameters::interleaved);
	}
	/*{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::nointerleaved);
		CPPUNIT_ASSERT(param.access_mode() == sound_parameters::nointerleaved);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::mmap_nointerleaved);
		CPPUNIT_ASSERT(param.access_mode() == sound_parameters::mmap_nointerleaved);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::mmap_complex);
		CPPUNIT_ASSERT(param.access_mode() == sound_parameters::mmap_complex);
	}*/
}

void asound_test_class::asound_data_format(){
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::nonblock));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.data_format(sound_parameters::s16_le);
		CPPUNIT_ASSERT(param.data_format() == sound_parameters::s16_le);
	}
}

void asound_test_class::asound_channels(){
	{
		alsa_sound alsa(TRACE_PARAMS("hw:0", alsa_sound::playback, alsa_sound::nonblock));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.sound_channels(2);
		CPPUNIT_ASSERT(param.sound_channels() == 2);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.sound_channels(6);
		CPPUNIT_ASSERT(param.sound_channels() == 6);
	}
}

void asound_test_class::asound_rate(){
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.sample_rate(44100);
		CPPUNIT_ASSERT(param.sample_rate() == 44100);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		size_t r = -1;
		CPPUNIT_ASSERT(param.sample_rate_near(r) != r);
	}
}

void asound_test_class::asound_buffer_size_mm(){
	alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	CPPUNIT_ASSERT(param.max_buffer_size(65536) == 65536);
	CPPUNIT_ASSERT(param.max_buffer_size() == 65536);
	CPPUNIT_ASSERT(param.max_buffer_size(32768) == 32768);
	CPPUNIT_ASSERT(param.max_buffer_size() == 32768);
	
	CPPUNIT_ASSERT(param.min_buffer_size(1024) == 1024);
	CPPUNIT_ASSERT(param.min_buffer_size() == 1024);
	CPPUNIT_ASSERT(param.min_buffer_size(2048) == 2048);
	CPPUNIT_ASSERT(param.min_buffer_size() == 2048);
}

void asound_test_class::asound_buffer_size(){
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(32768);
		param.min_buffer_size(1024);
		param.buffer_size(32700);
		CPPUNIT_ASSERT(param.buffer_size() == 32700);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(32768);
		param.min_buffer_size(1024);
		param.buffer_size(4096);
		CPPUNIT_ASSERT(param.buffer_size() == 4096);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(32768);
		param.min_buffer_size(1024);
		CPPUNIT_ASSERT(param.buffer_size_near(128) == 1024);
	}
}

void asound_test_class::asound_period_size_mm(){
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(65536);
		param.min_buffer_size(1024);
		
		CPPUNIT_ASSERT(param.max_period_size(32768)==32768);
		CPPUNIT_ASSERT(param.max_period_size() == 32768);
		CPPUNIT_ASSERT(param.max_period_size(8192)==8192);
		CPPUNIT_ASSERT(param.max_period_size() == 8192);
		CPPUNIT_ASSERT(param.min_period_size(1024)==1024);
		CPPUNIT_ASSERT(param.min_period_size() == 1024);
		CPPUNIT_ASSERT(param.min_period_size(2048)==2048);
		CPPUNIT_ASSERT(param.min_period_size() == 2048);
	}
}

void asound_test_class::asound_period_size(){
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(65536);
		param.min_buffer_size(1024);
		param.max_period_size(65536);
		param.min_period_size(1024);
		param.period_size(1024);
		CPPUNIT_ASSERT(param.period_size() == 1024);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(65536);
		param.min_buffer_size(1024);
		param.max_period_size(65536);
		param.min_period_size(1024);
		param.period_size(2048);
		CPPUNIT_ASSERT(param.period_size() == 2048);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.max_buffer_size(65536);
		param.min_buffer_size(1024);
		param.max_period_size(65536);
		param.min_period_size(1024);
		CPPUNIT_ASSERT(param.period_size_near(512) == 1024);
		CPPUNIT_ASSERT(param.period_size() == 1024);
	}
}

void asound_test_class::asound_buffer_time_mm(){
	alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.access_mode(sound_parameters::interleaved);
	param.data_format(sound_parameters::u8);
	param.sound_channels(1);
	param.sample_rate(8000);
	
	CPPUNIT_ASSERT(param.max_buffer_time(2000*1000) == 2000*1000);
	CPPUNIT_ASSERT(param.max_buffer_size() == 16000);
	
	CPPUNIT_ASSERT(param.max_buffer_time(1000*1000) == 1000*1000);
	CPPUNIT_ASSERT(param.max_buffer_size() == 8000);
	
	CPPUNIT_ASSERT(param.min_buffer_time(250*1000) == 250*1000);
	CPPUNIT_ASSERT(param.min_buffer_size() == 2000);
	
	CPPUNIT_ASSERT(param.min_buffer_time(500*1000) == 500*1000);
	CPPUNIT_ASSERT(param.min_buffer_size() == 4000);
}

void asound_test_class::asound_buffer_time(){
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		param.data_format(sound_parameters::u8);
		param.sound_channels(1);
		param.sample_rate(8000);
		param.max_buffer_time(2000*1000);
		param.min_buffer_time(250*1000);
		
		param.buffer_time(500*1000);
		CPPUNIT_ASSERT(param.buffer_time() == 500*1000);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		param.data_format(sound_parameters::u8);
		param.sound_channels(1);
		param.sample_rate(8000);
		param.max_buffer_time(2000*1000);
		param.min_buffer_time(250*1000);
		
		param.buffer_time(1000*1000);
		CPPUNIT_ASSERT(param.buffer_time() == 1000*1000);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		param.data_format(sound_parameters::u8);
		param.sound_channels(1);
		param.sample_rate(8000);
		param.max_buffer_time(2000*1000);
		param.min_buffer_time(250*1000);
		
		CPPUNIT_ASSERT(param.buffer_time_near(1000) == 250*1000);
		CPPUNIT_ASSERT(param.buffer_time() == 250*1000);
	}
}

void asound_test_class::asound_period_time_mm(){
	alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.access_mode(sound_parameters::interleaved);
	param.data_format(sound_parameters::u8);
	param.sound_channels(1);
	param.sample_rate(8000);
	param.max_buffer_time(2000*1000);
	param.min_buffer_time(25*1000);
	
	CPPUNIT_ASSERT(param.min_period_time(25*1000)==25*1000);
	CPPUNIT_ASSERT(param.min_period_time() == 25*1000);
	CPPUNIT_ASSERT(param.min_period_time(50*1000)==50*1000);
	CPPUNIT_ASSERT(param.min_period_time() == 50*1000);
	CPPUNIT_ASSERT(param.max_period_time(180*1000)==180*1000);
	CPPUNIT_ASSERT(param.max_period_time() == 180*1000);
	CPPUNIT_ASSERT(param.max_period_time(100*1000)==100*1000);
	CPPUNIT_ASSERT(param.max_period_time() == 100*1000);
}

void asound_test_class::asound_period_time(){
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		param.data_format(sound_parameters::u8);
		param.sound_channels(1);
		param.sample_rate(8000);
		param.max_buffer_time(2000*1000);
		param.min_buffer_time(25*1000);
		param.max_period_time(180*1000);
		param.min_period_time(25*1000);
		
		param.period_time(100*1000);
		CPPUNIT_ASSERT(param.period_time() == 100*1000);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		param.data_format(sound_parameters::u8);
		param.sound_channels(1);
		param.sample_rate(8000);
		param.max_buffer_time(2000*1000);
		param.min_buffer_time(25*1000);
		param.max_period_time(180*1000);
		param.min_period_time(25*1000);
		
		param.period_time(80*1000);
		CPPUNIT_ASSERT(param.period_time() == 80*1000);
	}
	{
		alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
		sound_parameters param(TRACE_PARAMS(alsa));
		param.access_mode(sound_parameters::interleaved);
		param.data_format(sound_parameters::u8);
		param.sound_channels(1);
		param.sample_rate(8000);
		param.max_buffer_time(2000*1000);
		param.min_buffer_time(25*1000);
		param.max_period_time(180*1000);
		param.min_period_time(25*1000);
		
		CPPUNIT_ASSERT(param.period_time_near(1000) == 25*1000);
		CPPUNIT_ASSERT(param.period_time() == 25*1000);
	}
}

void asound_test_class::asound_praram_write(){
	alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.sample_rate(44100);
	param.install_to_device();
}

void asound_test_class::asound_writei(){
	alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.access_mode(sound_parameters::interleaved);
	param.data_format(sound_parameters::s16_le);
	param.sound_channels(2);
	param.sample_rate(44100);
	param.install_to_device();
	size_t t = param.period_size();
	char *buf = new char[t*2*2];
	CPPUNIT_ASSERT(alsa.interleaved_write(buf, t) == t);
	delete[] buf;
}

void asound_test_class::asound_waited_period(){
	alsa_sound alsa(TRACE_PARAMS("default", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.access_mode(sound_parameters::interleaved);
	param.data_format(sound_parameters::s16_le);
	param.sound_channels(2);
	param.sample_rate(44100);
	param.install_to_device();
	
	soft_parameters sp(TRACE_PARAMS(alsa));
	sp.waited_period(2048);
	CPPUNIT_ASSERT(sp.waited_period() == 2048);
}
