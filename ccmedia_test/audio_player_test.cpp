#include "ccmedia_test/audio_player_test.h"
#include "ccmedia/audio_player.h"
#include "cclibc/thread/thread.h"
#include "ccapplib/time_util.h"

using CCALSA::sound_parameters;
using CCMEDIA::audio_player;
using CCLIBC::thread;
using CCLIBC::thread_attribute;

void audio_player_test_class::audio_player_params(){
	unsigned channels = 2;
	unsigned smp_rate = 44100;
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, smp_rate));
	CPPUNIT_ASSERT(!strcmp(snd_pcm_name(apl._alsa._snd_pcm), "default"));
	
	snd_pcm_hw_params_t *param;
	snd_pcm_hw_params_alloca(&param);
	
	snd_pcm_hw_params_current(apl._alsa._snd_pcm, param);
	
	snd_pcm_format_t format;
	snd_pcm_hw_params_get_format(param, &format);
	CPPUNIT_ASSERT_EQUAL(format, (snd_pcm_format_t)sound_parameters::s16_le);
	
	unsigned c;
	snd_pcm_hw_params_get_channels (param, &c);
	CPPUNIT_ASSERT_EQUAL(c, channels);
	
	unsigned r;
	snd_pcm_hw_params_get_rate(param, &r, NULL);
	CPPUNIT_ASSERT_EQUAL(r, smp_rate);
	
	snd_pcm_uframes_t  _period_size;
	snd_pcm_hw_params_get_period_size(param, &_period_size, NULL);
	CPPUNIT_ASSERT_EQUAL(apl.period_buffer_size(), (size_t)(4 * _period_size));
	CPPUNIT_ASSERT(!apl._cvt.raw_pointer());
}

void audio_player_test_class::audio_player_cvt_fmt(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_DBL, AV_CH_LAYOUT_STEREO, 44100));
	CPPUNIT_ASSERT(apl._cvt.raw_pointer());
	CPPUNIT_ASSERT_EQUAL(apl.parameters().data_format(), sound_parameters::s16);
}

void audio_player_test_class::audio_player_cvt_rate(){
	audio_player apl(TRACE_PARAMS("default", AV_SAMPLE_FMT_S16, AV_CH_LAYOUT_STEREO, 192000*2));
	CPPUNIT_ASSERT(apl._cvt.raw_pointer());
	CPPUNIT_ASSERT_EQUAL(apl.parameters().sample_rate(), (size_t)192000);
}
