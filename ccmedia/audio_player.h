#ifndef CCMEDIA_AUDIO_PLAYER_H_
#define CCMEDIA_AUDIO_PLAYER_H_

#include "ccmedia/env.h"
#include "ccmedia/util.h"
#include "ccalsa/asound.h"
#include "cclibc/util/list.h"
#include "cclibc/thread/thread.h"
#include "cclibc/thread/mutex.h"
#include "cclibc/thread/condition.h"
#include "cclibc/thread/semaphore.h"
#include "cclibc/memory.h"
#include "ccffmpeg/ffmpeg.h"

CCMEDIA_BG

#ifdef STACK_TRACE_FLAG

#define push_audio(...) push_audio(INVOKE_TRACE, ##__VA_ARGS__)
#define rest_time(...) rest_time(INVOKE_TRACE, ##__VA_ARGS__)

#ifndef drop_all
#define drop_all(...) drop_all(INVOKE_TRACE, ##__VA_ARGS__)
#endif 

#endif

typedef void (*adjust_volumn_function)(void*, size_t, size_t v);

class audio_player;
typedef void (*remap_function)(size_t, void*, size_t);

class audio_player: public clock_source{
private:
	CCALSA::alsa_sound _alsa;
	CCALSA::sound_parameters _hwparam;
	size_t _period_frame;
	size_t _frame_size;
	size_t _period_size;

	size_t _volume;
	adjust_volumn_function _adjust_volumn_fun;
	remap_function _remap_fun;
	CCLIBC::stack_ptr<CCFFMPEG::audio_converter> _cvt;
	void *_cvt_buf;
	
	bool _passthough;

	class period_buffer: public CCLIBC::single_list<period_buffer>::node{
	private:
		void * const _data;
	public:
		period_buffer(void *buf): _data(buf){ }
		void* data() const{
			return _data;
		}
	};
	CCLIBC::stack_pool _pool;
	CCLIBC::single_list<period_buffer> _buffered, _released;
	CCLIBC::mutex _pl_mutex;
	CCLIBC::condition _pl_cond;
	size_t _buffer_pos;

	const size_t _least_buffer, _most_buffer;
	bool _less, _more;
	void update_state(){
		_less = _buffered.size() < _least_buffer;
		_more = _buffered.size() >= _most_buffer;
	}

	bool _running;
	CCLIBC::stack_ptr<CCLIBC::thread> _thd;
	CCLIBC::semaphore _syn_sem;
	
	size_t _last_pts;
	int _last_pts_count;
	size_t _last_pts_pos;
	
	size_t delayed_frames FUNCTION_PARAMETER_LIST(){
		return _alsa.is_running() ? _alsa.delayed_frames() : 0;
	}

	void play_main(CCLIBC::semaphore&);
		period_buffer* allocate_period_buffer(){
		return STACK_NEW(_pool, period_buffer, _pool.allocate(period_buffer_size()));
	}
	
public:
	audio_player FUNCTION_PARAMETER_LIST(const char*, AVSampleFormat, int64_t, size_t,
		bool ps = false, size_t volume = 100, size_t least = 2, size_t most = 10);

	size_t period_buffer_size() const{
		return _period_size;
	}

	void push_audio FUNCTION_PARAMETER_LIST(CCFFMPEG::audio_frame&);

	void push_audio FUNCTION_PARAMETER_LIST(const void*, size_t, size_t pts = 0);

	~audio_player();

	const CCALSA::sound_parameters& parameters() const{
		return _hwparam;
	}

	size_t volume() const{
		return _volume;
	}

	void volume(size_t v){
		_volume = v;
	}

	size_t rest_time FUNCTION_PARAMETER_LIST();
	size_t current_clock FUNCTION_PARAMETER_LIST();

	void pause() {
		_syn_sem.wait();
		_alsa.shutdown();
	}

	void resume() {
		_syn_sem.post();
	}
	void drop_all FUNCTION_PARAMETER_LIST();

	bool insufficient() const{
		return _less;
	}

	bool overflow() const{
		return _more;
	}
};


CCMEDIA_END

#endif //CCMEDIA_AUDIO_PLAYER_H_

