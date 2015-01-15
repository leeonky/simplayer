#ifndef CCALSA_ASOUND_H_
#define CCALSA_ASOUND_H_

#include <alsa/asoundlib.h>
#include "ccalsa/env.h"
#include "cclibc/trace.h"
#include "cclibc/exception.h"
#include "cclibc/callc.h"

CCALSA_BG

class alsa_error: public CCLIBC::exception{
private:
	const char *_call;
	int _code;
public:
	alsa_error(const char *call, int c): exception("%s: alsa error (%d): %s", call, c, snd_strerror(c)), _call(call), _code(c){}
	int code() const {
		return _code;
	}

	const char* call() {
		return _call;
	}
};

class sound_parameters;
class soft_parameters;
class alsa_sound{
friend class sound_parameters;
friend class soft_parameters;
private:
	snd_pcm_t *_snd_pcm;
	alsa_sound(const alsa_sound&);
	alsa_sound& operator=(const alsa_sound&);
public:
	enum stream_types{
		playback = SND_PCM_STREAM_PLAYBACK,
		capture = SND_PCM_STREAM_CAPTURE,
	};

	enum io_modes{
		nonblock = SND_PCM_NONBLOCK,
		async = SND_PCM_ASYNC
	};
	
	alsa_sound FUNCTION_PARAMETER_LIST(const char*, stream_types, io_modes = async);
	~alsa_sound();
	size_t interleaved_write FUNCTION_PARAMETER_LIST(const void*, size_t);
	void prepare_device FUNCTION_PARAMETER_LIST();
	void shutdown FUNCTION_PARAMETER_LIST();
	bool resume_device FUNCTION_PARAMETER_LIST();
	size_t available_frames FUNCTION_PARAMETER_LIST();
	size_t available_update_frames FUNCTION_PARAMETER_LIST();
	size_t rewindable_frames FUNCTION_PARAMETER_LIST();
	size_t rewind_frames FUNCTION_PARAMETER_LIST(size_t);
	size_t forwardable_frames FUNCTION_PARAMETER_LIST();
	size_t forward_frames FUNCTION_PARAMETER_LIST(size_t);
	ssize_t delayed_frames FUNCTION_PARAMETER_LIST();
	bool wait_for_available(int);
	bool is_running(){
		return snd_pcm_state(_snd_pcm) == SND_PCM_STATE_RUNNING;
	}
};

class sound_parameters{
private:
	alsa_sound &_alsa;
	snd_pcm_hw_params_t *_snd_pcm_params;
public:
	enum access_modes{
		mmap_interleaved = SND_PCM_ACCESS_MMAP_INTERLEAVED,
		mmap_nointerleaved = SND_PCM_ACCESS_MMAP_NONINTERLEAVED,
		mmap_complex = SND_PCM_ACCESS_MMAP_COMPLEX,
		interleaved = SND_PCM_ACCESS_RW_INTERLEAVED,
		nointerleaved = SND_PCM_ACCESS_RW_NONINTERLEAVED,
	};
	enum data_formats{
		s8 = SND_PCM_FORMAT_S8,
		u8 = SND_PCM_FORMAT_U8,
		s16_le = SND_PCM_FORMAT_S16_LE,
		s16_be = SND_PCM_FORMAT_S16_BE,
		u16_le = SND_PCM_FORMAT_U16_LE,
		u16_be = SND_PCM_FORMAT_U16_BE,
		s24_le = SND_PCM_FORMAT_S24_LE,
		s24_be = SND_PCM_FORMAT_S24_BE,
		u24_le = SND_PCM_FORMAT_U24_LE,
		u24_be = SND_PCM_FORMAT_U24_BE,
		s32_le = SND_PCM_FORMAT_S32_LE,
		s32_be = SND_PCM_FORMAT_S32_BE,
		u32_le = SND_PCM_FORMAT_U32_LE,
		u32_be = SND_PCM_FORMAT_U32_BE,
		f32_le = SND_PCM_FORMAT_FLOAT_LE,
		f32_be = SND_PCM_FORMAT_FLOAT_BE,
		f64_le = SND_PCM_FORMAT_FLOAT64_LE,
		f64_be = SND_PCM_FORMAT_FLOAT64_BE,
		iec95b_le = SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
		iec95b_be = SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
		mu_law = SND_PCM_FORMAT_MU_LAW,
		a_law = SND_PCM_FORMAT_A_LAW,
		ima_adpcm = SND_PCM_FORMAT_IMA_ADPCM,
		mpeg = SND_PCM_FORMAT_MPEG,
		gsm = SND_PCM_FORMAT_GSM,
		special = SND_PCM_FORMAT_SPECIAL,
		s24_3le = SND_PCM_FORMAT_S24_3LE,
		s24_3be = SND_PCM_FORMAT_S24_3BE,
		u24_3le = SND_PCM_FORMAT_U24_3LE,
		u24_3be = SND_PCM_FORMAT_U24_3BE,
		s20_3le = SND_PCM_FORMAT_S20_3LE,
		s20_3be = SND_PCM_FORMAT_S20_3BE,
		u20_3le = SND_PCM_FORMAT_U20_3LE,
		u20_3be = SND_PCM_FORMAT_U20_3BE,
		s18_3le = SND_PCM_FORMAT_S18_3LE,
		s18_3be = SND_PCM_FORMAT_S18_3BE,
		u18_3le = SND_PCM_FORMAT_U18_3LE,
		u18_3be = SND_PCM_FORMAT_U18_3BE,
		/*g723_24 = SND_PCM_FORMAT_G723_24,
		g723_24_1b = SND_PCM_FORMAT_G723_24_1B,
		g723_40 = SND_PCM_FORMAT_G723_40,
		g723_40_1b = SND_PCM_FORMAT_G723_40_1B,
		dsd_u8 = SND_PCM_FORMAT_DSD_U8,
		dsd_u16_le = SND_PCM_FORMAT_DSD_U16_LE,*/

	#if __BYTE_ORDER == __LITTLE_ENDIAN
		s16 = SND_PCM_FORMAT_S16_LE,
		u16 = SND_PCM_FORMAT_U16_LE,
		s24 = SND_PCM_FORMAT_S24_LE,
		u24 = SND_PCM_FORMAT_U24_LE,
		s32 = SND_PCM_FORMAT_S32_LE,
		u32 = SND_PCM_FORMAT_U32_LE,
		float32 = SND_PCM_FORMAT_FLOAT_LE,
		float64 = SND_PCM_FORMAT_FLOAT64_LE,
		iec958 = SND_PCM_FORMAT_IEC958_SUBFRAME_LE
	#elif __BYTE_ORDER == __BIG_ENDIAN
		s16 = SND_PCM_FORMAT_S16_BE,
		u16 = SND_PCM_FORMAT_U16_BE,
		s24 = SND_PCM_FORMAT_S24_BE,
		u24 = SND_PCM_FORMAT_U24_BE,
		s32 = SND_PCM_FORMAT_S32_BE,
		u32 = SND_PCM_FORMAT_U32_BE,
		float32 = SND_PCM_FORMAT_FLOAT_BE,
		float64 = SND_PCM_FORMAT_FLOAT64_BE,
		iec958 = SND_PCM_FORMAT_IEC958_SUBFRAME_BE
	#endif
	};

	sound_parameters FUNCTION_PARAMETER_LIST(alsa_sound&);
	~sound_parameters();

	access_modes access_mode FUNCTION_PARAMETER_LIST() const;
	void access_mode FUNCTION_PARAMETER_LIST(access_modes);
	
	data_formats data_format FUNCTION_PARAMETER_LIST() const;
	void data_format FUNCTION_PARAMETER_LIST(data_formats);
	
	bool test_format FUNCTION_PARAMETER_LIST(data_formats);
	
	size_t sound_channels FUNCTION_PARAMETER_LIST() const;
	void sound_channels FUNCTION_PARAMETER_LIST(size_t);
	size_t sound_channels_near FUNCTION_PARAMETER_LIST(size_t);
	
	size_t sample_rate FUNCTION_PARAMETER_LIST() const;
	void sample_rate FUNCTION_PARAMETER_LIST(size_t);
	size_t sample_rate_near FUNCTION_PARAMETER_LIST(size_t);

	size_t period_size FUNCTION_PARAMETER_LIST() const;
	void period_size FUNCTION_PARAMETER_LIST(size_t);
	size_t period_size_near FUNCTION_PARAMETER_LIST(size_t);
	size_t max_period_size FUNCTION_PARAMETER_LIST(size_t);
	size_t max_period_size FUNCTION_PARAMETER_LIST() const;
	size_t min_period_size FUNCTION_PARAMETER_LIST(size_t);
	size_t min_period_size FUNCTION_PARAMETER_LIST() const;
	
	size_t period_time FUNCTION_PARAMETER_LIST() const;
	void period_time FUNCTION_PARAMETER_LIST(size_t);
	size_t period_time_near FUNCTION_PARAMETER_LIST(size_t);
	size_t max_period_time FUNCTION_PARAMETER_LIST(size_t);
	size_t max_period_time FUNCTION_PARAMETER_LIST() const;
	size_t min_period_time FUNCTION_PARAMETER_LIST(size_t);
	size_t min_period_time FUNCTION_PARAMETER_LIST() const;
	
	void install_to_device FUNCTION_PARAMETER_LIST();
	
	void buffer_size FUNCTION_PARAMETER_LIST(size_t);
	size_t buffer_size FUNCTION_PARAMETER_LIST() const;
	size_t buffer_size_near FUNCTION_PARAMETER_LIST(size_t);
	size_t max_buffer_size FUNCTION_PARAMETER_LIST(size_t);
	size_t max_buffer_size FUNCTION_PARAMETER_LIST() const;
	size_t min_buffer_size FUNCTION_PARAMETER_LIST(size_t);
	size_t min_buffer_size FUNCTION_PARAMETER_LIST() const;

	void buffer_time FUNCTION_PARAMETER_LIST(size_t);
	size_t buffer_time FUNCTION_PARAMETER_LIST() const;
	size_t buffer_time_near FUNCTION_PARAMETER_LIST(size_t);
	size_t max_buffer_time FUNCTION_PARAMETER_LIST(size_t);
	size_t max_buffer_time FUNCTION_PARAMETER_LIST() const;
	size_t min_buffer_time FUNCTION_PARAMETER_LIST(size_t);
	size_t min_buffer_time FUNCTION_PARAMETER_LIST() const;
};

class soft_parameters{
private:
	alsa_sound &_alsa;
	snd_pcm_sw_params_t *_snd_pcm_params;

public:
	soft_parameters FUNCTION_PARAMETER_LIST(alsa_sound&);
	~soft_parameters();
	
	void waited_period FUNCTION_PARAMETER_LIST(size_t f);
	size_t waited_period FUNCTION_PARAMETER_LIST() const;
	
	void install_to_device FUNCTION_PARAMETER_LIST();
};

#ifdef STACK_TRACE_FLAG

#define access_mode(...) access_mode(INVOKE_TRACE,## __VA_ARGS__)
#define data_format(...) data_format(INVOKE_TRACE,## __VA_ARGS__)
#define test_format(...) test_format(INVOKE_TRACE,## __VA_ARGS__)
#define sound_channels(...) sound_channels(INVOKE_TRACE,## __VA_ARGS__)
#define sound_channels_near(...) sound_channels_near(INVOKE_TRACE,## __VA_ARGS__)
#define sample_rate(...) sample_rate(INVOKE_TRACE,## __VA_ARGS__)
#define sample_rate_near(...) sample_rate_near(INVOKE_TRACE,## __VA_ARGS__)
#define period_size(...) period_size(INVOKE_TRACE,## __VA_ARGS__)
#define period_size_near(...) period_size_near(INVOKE_TRACE,## __VA_ARGS__)
#define max_period_size(...) max_period_size(INVOKE_TRACE,## __VA_ARGS__)
#define min_period_size(...) min_period_size(INVOKE_TRACE,## __VA_ARGS__)
#define period_time(...) period_time(INVOKE_TRACE,## __VA_ARGS__)
#define period_time_near(...) period_time_near(INVOKE_TRACE,## __VA_ARGS__)
#define max_period_time(...) max_period_time(INVOKE_TRACE,## __VA_ARGS__)
#define min_period_time(...) min_period_time(INVOKE_TRACE,## __VA_ARGS__)
#define install_to_device(...) install_to_device(INVOKE_TRACE,## __VA_ARGS__)
#define interleaved_write(...) interleaved_write(INVOKE_TRACE,## __VA_ARGS__)
#define prepare_device(...) prepare_device(INVOKE_TRACE,## __VA_ARGS__)
#define shutdown(...) shutdown(INVOKE_TRACE,## __VA_ARGS__)
#define buffer_size(...) buffer_size(INVOKE_TRACE,## __VA_ARGS__)
#define max_buffer_size(...) max_buffer_size(INVOKE_TRACE,## __VA_ARGS__)
#define min_buffer_size(...) min_buffer_size(INVOKE_TRACE,## __VA_ARGS__)
#define buffer_size_near(...) buffer_size_near(INVOKE_TRACE,## __VA_ARGS__)
#define buffer_time(...) buffer_time(INVOKE_TRACE,## __VA_ARGS__)
#define max_buffer_time(...) max_buffer_time(INVOKE_TRACE,## __VA_ARGS__)
#define min_buffer_time(...) min_buffer_time(INVOKE_TRACE,## __VA_ARGS__)
#define buffer_time_near(...) buffer_time_near(INVOKE_TRACE,## __VA_ARGS__)
#define resume_device(...) resume_device(INVOKE_TRACE,## __VA_ARGS__)
#define available_frames(...) available_frames(INVOKE_TRACE,## __VA_ARGS__)
#define available_update_frames(...) available_update_frames(INVOKE_TRACE,## __VA_ARGS__)
#define rewindable_frames(...) rewindable_frames(INVOKE_TRACE,## __VA_ARGS__)
#define rewind_frames(...) rewind_frames(INVOKE_TRACE,## __VA_ARGS__)
#define forwardable_frames(...) forwardable_frames(INVOKE_TRACE,## __VA_ARGS__)
#define forward_frames(...) forward_frames(INVOKE_TRACE,## __VA_ARGS__)
#define delayed_frames(...) delayed_frames(INVOKE_TRACE,## __VA_ARGS__)
#define waited_period(...) waited_period(INVOKE_TRACE,## __VA_ARGS__)

#endif

CCALSA_END

#endif //CCALSA_ASOUND_H_
