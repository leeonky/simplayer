#include <cstring>
#include "ccalsa_mock_test/asound_test.h"
#include "ccalsa/asound.h"

using CCALSA::alsa_sound;
using CCALSA::sound_parameters;
using CCALSA::soft_parameters;

void asound_test_class::asound_open(){
	interface_testor::clear();
	alsa_sound alsa1(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa1._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	interface_testor::clear();
	alsa_sound alsa2(TRACE_PARAMS("sound dev2", alsa_sound::capture, alsa_sound::nonblock));
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa2._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev2"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_CAPTURE));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_NONBLOCK));
}

void asound_test_class::asound_close(){
	void *p1;
	{
		alsa_sound alsa1(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
		p1 = alsa1._snd_pcm;
		interface_testor::clear();
	}
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_close"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(p1));
}

void asound_test_class::asound_parameters(){
	void *p1, *p2, *p3;
	{
		alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
		interface_testor::clear();
		sound_parameters param(TRACE_PARAMS(alsa));
		p1 = alsa._snd_pcm;
		p2 = &param._snd_pcm_params;
		p3 = param._snd_pcm_params;
	}
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(p2));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p1));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(p3));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_free"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(p3));
}

void asound_test_class::asound_parameters_get_access_style(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.access_mode();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_get_access"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_access_style(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.access_mode(sound_parameters::interleaved);
	param.access_mode();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_access"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(2).equal(SND_PCM_ACCESS_RW_INTERLEAVED));
	
	CPPUNIT_ASSERT(interface_testor::method(4).equal("snd_pcm_hw_params_get_access"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_get_format(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.data_format();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_get_format"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_format(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.data_format(sound_parameters::s16_le);
	param.data_format();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_format"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(2).equal(SND_PCM_FORMAT_S16_LE));
	
	CPPUNIT_ASSERT(interface_testor::method(4).equal("snd_pcm_hw_params_get_format"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_get_channels(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.sound_channels();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_get_channels"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_channels(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.sound_channels(4);
	param.sound_channels();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_channels"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(2).equal(4));
	
	CPPUNIT_ASSERT(interface_testor::method(4).equal("snd_pcm_hw_params_get_channels"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_get_rate(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.sample_rate();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_get_rate"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_rate(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.sample_rate(44100);
	param.sample_rate();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_rate"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(2).equal(44100));
	
	CPPUNIT_ASSERT(interface_testor::method(4).equal("snd_pcm_hw_params_get_rate"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_rate_near(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	size_t r = 44100;
	param.sample_rate_near(r);

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_rate_near"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_get_period(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.period_size();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_get_period_size"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_period(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.period_size(128);
	param.period_size();

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_period_size"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(2).equal(128));
	
	CPPUNIT_ASSERT(interface_testor::method(4).equal("snd_pcm_hw_params_get_period_size"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_period_near(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	size_t r = 0x1000000;
	param.period_size_near(r);

	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_period_size_near"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_period_max(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.max_period_size(128);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_period_size_max"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_period_min(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.min_period_size(128);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_period_size_min"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_period_time(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.period_time(10);
	param.period_time();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_period_time"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_get_period_time"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_period_time_near(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.period_time_near(10);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_period_time_near"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_period_time_max(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.max_period_time(10);
	param.max_period_time();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_period_time_max"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_get_period_time_max"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_period_time_min(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.min_period_time(10);
	param.min_period_time();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_period_time_min"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_get_period_time_min"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_channels_near(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	param.sound_channels_near(4);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_channels_near"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_install_to_device(){
	interface_testor::clear();
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	size_t r = 44100;
	param.sample_rate_near(r);
	param.install_to_device();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_open"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(&alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal("sound dev"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SND_PCM_STREAM_PLAYBACK));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(SND_PCM_ASYNC));
	
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(&param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_hw_params_any"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(param._snd_pcm_params));
	
	CPPUNIT_ASSERT(interface_testor::method(3).equal("snd_pcm_hw_params_set_rate_near"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(1).equal(param._snd_pcm_params));

	CPPUNIT_ASSERT(interface_testor::method(4).equal("snd_pcm_hw_params"));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(4).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_writei(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	char buf[] = "Hello world and alsa.";
	alsa.interleaved_write(buf, 1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_writei"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal((void*)buf));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(1));
}

void asound_test_class::asound_prepare(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.prepare_device();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_prepare"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_shutdown(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.shutdown();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_drop"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_parameters_set_buffer_size(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.buffer_size(256);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_size"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(256));
}

void asound_test_class::asound_parameters_get_buffer_size(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.buffer_size();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_get_buffer_size"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_max_buffer_size(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	size_t s = 256;
	param.max_buffer_size(s);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_size_max"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_get_max_buffer_size(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.max_buffer_size();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_get_buffer_size_max"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_buffer_size_near(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	size_t s = 256;
	param.buffer_size_near(s);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_size_near"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_set_min_buffer_size(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	size_t s = 256;
	param.min_buffer_size(s);
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_size_min"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_get_min_buffer_size(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.min_buffer_size();
	
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_get_buffer_size_min"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_buffer_time(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.buffer_time(10);
	param.buffer_time();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_time"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_get_buffer_time"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_buffer_time_near(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.buffer_time_near(10);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_time_near"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_buffer_time_max(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.max_buffer_time(10);
	param.max_buffer_time();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_time_max"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_get_buffer_time_max"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_buffer_time_min(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	sound_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.min_buffer_time(10);
	param.min_buffer_time();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_hw_params_set_buffer_time_min"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_hw_params_get_buffer_time_min"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}


void asound_test_class::asound_resume(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.resume_device();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_resume"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_avail(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.available_frames();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_avail"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_avail_update(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.available_update_frames();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_avail_update"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_rewindable(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.rewindable_frames();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_rewindable"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_rewind(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.rewind_frames(128);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_rewind"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(128));
}

void asound_test_class::asound_forwardable(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.forwardable_frames();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_forwardable"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_forward(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.forward_frames(128);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_forward"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(128));
}

void asound_test_class::asound_delay(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.delayed_frames();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_delay"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
}

void asound_test_class::asound_wait_avail(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	alsa.wait_for_available(-1);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_wait"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(-1));
}

void asound_test_class::asound_parameters_soft(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	interface_testor::clear();
	void *p1, *p2;
	{
		soft_parameters param(TRACE_PARAMS(alsa));
		p1 = &param._snd_pcm_params;
		p2 = param._snd_pcm_params;
	}
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_sw_params_malloc"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(p1));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_sw_params_current"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(1).equal(p2));
	CPPUNIT_ASSERT(interface_testor::method(2).equal("snd_pcm_sw_params_free"));
	CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(p2));
}

void asound_test_class::asound_parameters_minimal_avail(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	soft_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.waited_period(128);
	param.waited_period();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_sw_params_set_avail_min"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(128));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("snd_pcm_sw_params_get_avail_min"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(param._snd_pcm_params));
}

void asound_test_class::asound_parameters_soft_install(){
	alsa_sound alsa(TRACE_PARAMS("sound dev", alsa_sound::playback, alsa_sound::async));
	soft_parameters param(TRACE_PARAMS(alsa));
	interface_testor::clear();
	param.install_to_device();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("snd_pcm_sw_params"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(alsa._snd_pcm));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(param._snd_pcm_params));
}
