#include "ccmedia_mock_test/audio_player_test.h"
#include "ccmedia/audio_player.h"
#include "cclibc/thread/thread.h"
#include "ccapplib/time_util.h"

using CCMEDIA::audio_player;
using CCALSA::sound_parameters;
using CCLIBC::thread;
using CCLIBC::thread_attribute;
using CCLIBC::mutex;

void audio_player_test_class::audio_player_put_little(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	const char *msg = "Hello";
	size_t len = strlen(msg);
	apl.push_audio(msg, len);
	CPPUNIT_ASSERT_EQUAL((size_t)0, apl._buffered.size());
	CPPUNIT_ASSERT(!memcmp(apl._released.first()->data(), "Hello", len));
}

void audio_player_test_class::audio_player_put_little_and_little(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	const char *msg = "Hello";
	size_t len = strlen(msg);
	apl.push_audio(msg, len);
	apl.push_audio(msg, len);
	CPPUNIT_ASSERT_EQUAL((size_t)0, apl._buffered.size());
	CPPUNIT_ASSERT(!memcmp(apl._released.first()->data(), "HelloHello", len*2));
}

void audio_player_test_class::audio_player_put_rest(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	const char *msg = "Hello";
	size_t len = strlen(msg);
	apl.push_audio(msg, len);
	char buf[apl.period_buffer_size()];
	buf[0] = '\0';
	apl.push_audio(buf, apl.period_buffer_size() - len);
	CPPUNIT_ASSERT_EQUAL((size_t)1, apl._buffered.size());
	CPPUNIT_ASSERT(!memcmp(apl._buffered.first()->data(), "Hello", len));
}

void audio_player_test_class::audio_player_put_more_than_one(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	const char *msg = "Hello";
	size_t len = strlen(msg);
	apl.push_audio(msg, len);
	char buf[apl.period_buffer_size()];
	memcpy(buf+apl.period_buffer_size()-len, "World", len);
	apl.push_audio(buf, apl.period_buffer_size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, apl._buffered.size());
	CPPUNIT_ASSERT(!memcmp(apl._buffered.first()->data(), "Hello", len));
	CPPUNIT_ASSERT(!memcmp(apl._released.first()->data(), "World", len));
}

void audio_player_test_class::audio_player_put_more_than_two(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	const char *msg = "Hello";
	size_t len = strlen(msg);
	apl.push_audio(msg, len);
	char buf[apl.period_buffer_size()*2];
	memcpy(buf+apl.period_buffer_size()-len, "World", len);
	memcpy(buf+2*apl.period_buffer_size()-len, "12345", len);
	apl.push_audio(buf, apl.period_buffer_size()*2);
	CPPUNIT_ASSERT_EQUAL((size_t)2, apl._buffered.size());
	CPPUNIT_ASSERT(!memcmp(apl._buffered.first()->data(), "Hello", len));
	CPPUNIT_ASSERT(!memcmp(apl._buffered.last()->data(), "World", len));
	CPPUNIT_ASSERT(!memcmp(apl._released.first()->data(), "12345", len));	
}

void audio_player_test_class::audio_player_consume(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	const char *msg = "Hello";
	size_t len = strlen(msg);
	char buf[apl.period_buffer_size()];
	strcpy(buf, msg);
	apl.push_audio(buf, apl.period_buffer_size());
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)0, apl._buffered.size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, apl._released.size());
	CPPUNIT_ASSERT(!memcmp(apl._released.first()->data(), "Hello", len));
}

void audio_player_test_class::audio_player_hold(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	char buf[apl.period_buffer_size()*2];
	apl.push_audio(buf, apl.period_buffer_size()*2);
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)2, apl._buffered.size());
}

void audio_player_test_class::audio_player_put_after_release(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	const char *msg = "Hello";
	size_t len = strlen(msg);
	char buf[apl.period_buffer_size()];
	strcpy(buf, msg);
	apl.push_audio(buf, apl.period_buffer_size());
	thread::sleep_nano(100*1000*1000);
	strcpy(buf, "World");
	apl.push_audio(buf, apl.period_buffer_size());
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)0, apl._buffered.size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, apl._released.size());
	CPPUNIT_ASSERT(!memcmp(apl._released.first()->data(), "World", len));
}

void audio_player_test_class::audio_player_hold_consume(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 48000));
	apl.pause();
	char buf[apl.period_buffer_size()];
	apl.push_audio(buf, apl.period_buffer_size());
	apl.resume();
	thread::sleep_nano(100*1000*1000);
	CPPUNIT_ASSERT_EQUAL((size_t)0, apl._buffered.size());
	CPPUNIT_ASSERT_EQUAL((size_t)1, apl._released.size());
}
