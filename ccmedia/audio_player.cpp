#include "ccmedia/audio_player.h"
#include "ccmedia/util.h"
#include "ccalsa/asound.h"

#include <limits>  
using CCALSA::alsa_error;
using CCALSA::alsa_sound;
using CCALSA::sound_parameters;
using CCLIBC::semaphore;
using CCLIBC::thread;
using CCLIBC::exception;
using CCALSA::soft_parameters;
using CCFFMPEG::audio_converter;
using CCFFMPEG::audio_frame;

CCMEDIA_BG

namespace{
	template<typename T>
	void _remap(size_t c, void *buf, size_t s){
		if(c>=6){//FL, FR, FC, LFE, BL, BR, (SL, SR) ==> FL, FR, BL, BR, FC, LFE, (SL, SR)
			T *p = static_cast<T*>(buf);
			s/=(c*sizeof(T));
			while(s--){
				T t = p[2];	p[2] = p[4]; p[4] = t;
				t = p[3]; p[3] = p[5]; p[5] = t;
				p+=c;
			}
		}
	}

	template<typename T>
	void _aj_v_l(void *buf, size_t s, size_t vol){
		if(100 != vol){
			float v = vol/100.0;
			s/=sizeof(T);
			while(s--){
				static_cast<T*>(buf)[s] *= v; 
			}
		}
	}

	bool transform_format(AVSampleFormat &in, sound_parameters::data_formats &out){
		switch(in){
			case AV_SAMPLE_FMT_U8:
				out = sound_parameters::u8;
				break;
			case AV_SAMPLE_FMT_S16:
				out = sound_parameters::s16;
				break;
			case AV_SAMPLE_FMT_S32:
				out = sound_parameters::s32;
				break;
			case AV_SAMPLE_FMT_FLT:
				out = sound_parameters::float32;
				break;
			case AV_SAMPLE_FMT_DBL:
				out = sound_parameters::float64;
				break;
			default:
				out = sound_parameters::s16;
				in = AV_SAMPLE_FMT_S16;
				return false;
		}
		return true;
	}
}

audio_player::audio_player FUNCTION_PARAMETER_LIST(const char *dev, AVSampleFormat f,
	int64_t cl, size_t r, bool ps, size_t v, size_t l, size_t m)
	: _alsa(TRACE_PARAMS(dev, alsa_sound::playback, alsa_sound::async)), _hwparam(TRACE_PARAMS(_alsa)),
	_volume(v), _passthough(ps), _pl_mutex(TRACE_PARAMS()), _pl_cond(TRACE_PARAMS()), _buffer_pos(0), 
	_least_buffer(l), _most_buffer(m), _less(true), _more(false), 
	_syn_sem(TRACE_PARAMS(1)), _last_pts(0), _last_pts_count(0), _last_pts_pos(0){
	size_t nbc = av_get_channel_layout_nb_channels(cl);
	sound_parameters::data_formats out_f;
	AVSampleFormat out_ff = f;
	bool cvt = !transform_format(out_ff, out_f);
	sound_parameters::data_formats fms[] = {out_f, sound_parameters::s16, sound_parameters::s32};
	AVSampleFormat ffms[] = {out_ff, AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_S32};
	bool errfmt = true;
	for(size_t i=0; i<sizeof(fms)/sizeof(fms[0]); ++i){
		if( _hwparam.test_format(out_f = fms[i])){
			_hwparam.data_format(out_f);
			out_ff = ffms[i];
			errfmt = false;
			if(i>0){
				cvt = true;
			}
			break;
		}
	}
	if(errfmt){
		THROW_NEW_EXCEPTION(exception, "Unsupport audio frame format(%d)", f);
	}
	size_t out_r = _hwparam.sample_rate_near(r);
	size_t out_c = _hwparam.sound_channels_near(nbc);
	_hwparam.access_mode(sound_parameters::interleaved);
	size_t bt = out_r * 300/1000;
	if(_hwparam.max_period_size() > bt){
		_hwparam.max_period_size(bt);
		_hwparam.max_buffer_size(bt);
	}else{
		_hwparam.max_buffer_size(bt);
	}
	bt = _hwparam.buffer_size_near(bt);
	_period_frame = _hwparam.period_size_near(bt*2/3);
	_hwparam.install_to_device();
	switch(out_f){
		case sound_parameters::u8:
			_frame_size = 1;
			_adjust_volumn_fun = _aj_v_l<int8_t>;
			_remap_fun = _remap<int8_t>;
			break;
		case sound_parameters::s16:
			_frame_size = 2;
			_adjust_volumn_fun = _aj_v_l<int16_t>;
			_remap_fun = _remap<int16_t>;
			break;
		case sound_parameters::s32:
			_frame_size = 4;
			_adjust_volumn_fun = _aj_v_l<int32_t>;
			_remap_fun = _remap<int32_t>;
			break;
		case sound_parameters::float32:
			_frame_size = 4;
			_adjust_volumn_fun = _aj_v_l<float>;
			_remap_fun = _remap<float>;
			break;
		case sound_parameters::float64:
			_frame_size = 8;
			_adjust_volumn_fun = _aj_v_l<double>;
			_remap_fun = _remap<double>;
			break;
		default:
			THROW_NEW_EXCEPTION(exception, "Unsupport alsa frame format(%d)", out_f);
	}
	_frame_size *= out_c;
	_period_size = _period_frame*_frame_size;
	if(cvt || r!=out_r || nbc!=out_c){
		int64_t out_cl = nbc != out_c ? av_get_default_channel_layout(out_c) : cl;
		_cvt.reassign(STACK_NEW(_pool, audio_converter,
			TRACE_PARAMS(cl, f, r, out_cl, out_ff, out_r)));
		_cvt_buf = _pool.allocate(1024*1024*6);
	}
	soft_parameters swparam(TRACE_PARAMS(_alsa));
	swparam.waited_period(_period_frame);
	swparam.install_to_device();
	semaphore TRACE_CONSTRUCT(wait_start);
	_thd.reassign(STACK_NEW(_pool, thread, TRACE_PARAMS(*this, &audio_player::play_main, wait_start)));
	wait_start.wait();
}

void audio_player::play_main(semaphore &start){
	_running = true;
	start.post();
	_alsa.prepare_device();
	bool got_data = false;
	while(_running){
		if(!got_data){
			GUARD_LOCK(_pl_mutex);
			got_data = _buffered.size() ||
				(_pl_cond.timed_wait(_pl_mutex, 10*1000*1000) && _buffered.size());
		}
		if(got_data){
			if(_syn_sem.timed_wait(10*1000*1000)){
				_syn_sem.post();
				got_data = false;
				if(_alsa.wait_for_available(10)){
					GUARD_LOCK(_pl_mutex);
					if(_buffered.first()){
						if(!_passthough){
							_remap_fun(_hwparam.sound_channels(), _buffered.first()->data(), _period_size);
							_adjust_volumn_fun(_buffered.first()->data(), _period_size, _volume);
						}
						try{
							if(!_alsa.is_running()){
								_alsa.prepare_device();
							}
							_alsa.interleaved_write(_buffered.first()->data(), _period_frame);
						}catch(const alsa_error *e){
							fprintf(stderr, "Replay audio\n");
							switch(e->code()){
								case -EPIPE:
								case -EBADFD:
									_alsa.prepare_device();
									break;
								case -ESTRPIPE:
									for(;;){
										try{
											_alsa.resume_device();
										}catch(const alsa_error *e2){
											if(e2->code() == -EAGAIN){
												thread::sleep_nano(1000*1000*1000);
											}else{
												_alsa.prepare_device();
											}
										}
									}
									break;
								default:
									fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
									#ifdef STACK_TRACE_FLAG
									exception::last_stack().print_stack(stderr);
									#endif
									break;
							}
						}
						_released.add(_buffered.remove_first());
						update_state();
						--_last_pts_count;
					}
				}
			}
		}
	}
}

audio_player::~audio_player(){
	_running = false;
	_thd->join();
}

size_t audio_player::current_clock FUNCTION_PARAMETER_LIST(){
	GUARD_LOCK(_pl_mutex);
	return static_cast<size_t>(static_cast<int64_t>(_last_pts) - 
		((_last_pts_count * static_cast<int64_t>(period_buffer_size()) 
			+ static_cast<int>(_last_pts_pos))
			/ static_cast<int>(_frame_size)
			+ static_cast<int>(delayed_frames()))	* 1000
		/ static_cast<int>(_hwparam.sample_rate()));
}

size_t audio_player::rest_time FUNCTION_PARAMETER_LIST(){
	GUARD_LOCK(_pl_mutex);
	int64_t fs = static_cast<int64_t>(period_buffer_size()*_buffered.size()+
		+ static_cast<int>(delayed_frames()));
	if(fs > 0){
		return fs / _frame_size * 1000 / _hwparam.sample_rate();
	}else{
		return 0;
	}
}

void audio_player::push_audio FUNCTION_PARAMETER_LIST(audio_frame &f){
	if(_cvt.raw_pointer()){
		size_t len;
		_cvt->convert_audio(f, _cvt_buf, len);
		push_audio(_cvt_buf, len, f.presentation_time());
	}else{
		AVFrame &af = *static_cast<AVFrame*>(f.raw_data());
		push_audio(af.extended_data[0], af.nb_samples*_frame_size, f.presentation_time());
	}
}

void audio_player::push_audio FUNCTION_PARAMETER_LIST(const void *data, size_t len, size_t pts){
	GUARD_LOCK(_pl_mutex);
	const uint8_t* src = static_cast<const uint8_t*>(data);
	if(pts){
		_last_pts_count = _buffered.size();
		_last_pts_pos = _buffer_pos;
		_last_pts = pts;
	}
	do{
		if(!_released.size()){
			_released.add(allocate_period_buffer());
		}
		size_t count = std::min(len, period_buffer_size()-_buffer_pos);
		memcpy(static_cast<uint8_t*>(_released.first()->data()) + _buffer_pos, src, count);
		if(period_buffer_size() == (_buffer_pos+=count)){
			_buffered.add(_released.remove(_released.first()));
			_buffer_pos = 0;
		}
		src+=count;
		len-=count;
	}while(len);
	update_state();
	_pl_cond.signal();
}

void audio_player::drop_all FUNCTION_PARAMETER_LIST(){
	GUARD_LOCK(_pl_mutex);
	_alsa.shutdown();
	while(_buffered.size()){
		_released.add(_buffered.remove_first());
	}
	update_state();
}

CCMEDIA_END
