#include "ccmedia/media_player.h"

using CCFFMPEG::video_decoder;
using CCFFMPEG::audio_decoder;
using CCFFMPEG::media_packet;
using CCFFMPEG::video_frame;
using CCFFMPEG::audio_frame;
using CCLIBC::thread;
using CCLIBC::stack_ptr;
using CCLIBC::semaphore;
using CCLIBC::exception;
using CCLIBC::util;

CCMEDIA_BG

media_player::media_player FUNCTION_PARAMETER_LIST(const char *f, size_t st, size_t du)
	: _stream(TRACE_PARAMS(f)), _running(true), _started(false), _processing(true),
	_pkt_mtx(TRACE_PARAMS()),
	_vpl(NULL), _vpkt_mtx(TRACE_PARAMS()), _vdec_mtx(TRACE_PARAMS()),
	_vpkt_cond(TRACE_PARAMS()), _most_buffer(50), _more(false),
	_play_start(st), _play_end(du){
}

media_player::~media_player(){
	_running = false;
	if(_vdec_thd.raw_pointer()){
		_vdec_thd->join();
		_vpl->change_clock(NULL);
	}
	destruct_single_list_elements(_pkt_list);
	destruct_single_list_elements(_vpkt_list);
}

bool media_player::read_packet FUNCTION_PARAMETER_LIST(){
	media_packet *pkt;
	if((pkt = _pkt_list.first())){
		pkt->free_packet();
	}else{
		GUARD_LOCK(_pkt_mtx);
		_pkt_list.add(pkt = STACK_NEW(_pool, media_packet));
	}
	return _stream.read_packet(*pkt) && (_play_end ? pkt->presentation_time()<_play_end : true);
}

void media_player::start(){
	if(!_started){
		_started = true;
		if(_vdec.raw_pointer()){
			_vpl->resume();
			if(_apl.raw_pointer()){
				_vpl->synchronize_clock(_apl->current_clock());
			}else if(_pass_audio.raw_pointer()){
				_vpl->synchronize_clock(_pass_audio->player().current_clock());
			}
		}
		if(_apl.raw_pointer()){
			_apl->resume();
		}
		if(_pass_audio.raw_pointer()){
			_pass_audio->player().resume();
		}
	}
}

bool media_player::process FUNCTION_PARAMETER_LIST(){
	bool ovf = overflow();
	if(_processing && ovf){
		start();
	}
	if(!ovf){
		audio_frame TRACE_CONSTRUCT(afrm);
		while(read_packet()){
			media_packet *pkt = _pkt_list.first();
			if(pkt->presentation_time() && _play_start > pkt->presentation_time()){
				seek_to(_play_start);
				continue;
			}
			if(_vdec.raw_pointer()
				&& _vdec->contains_data(*pkt)){
				GUARD_LOCK(_vpkt_mtx);
				GUARD_LOCK(_pkt_mtx);
				_vpkt_list.add(_pkt_list.remove_first());
				_vpkt_cond.signal();
				update_state();
				return true;
			}else if(_adec.raw_pointer()
				&& _adec->contains_data(*pkt)){
				bool more_data;
				do{
					more_data = _adec->decode_frame(*pkt, afrm);
					if(_adec->got_frame()){
						_apl->push_audio(afrm);
					}
				}while(more_data);
				return true;
			}else if(_pass_audio.raw_pointer()
				&& _pass_audio->contains_data(*pkt)){
				_pass_audio->push_bit_audio(*pkt);
				return true;
			}else if(_media_subtitle.raw_pointer()
				&& _media_subtitle->contains_data(*pkt)){
				_media_subtitle->decode_frame(*pkt);
			}
		}
	}else{
		thread::sleep_nano(1000 * 1000);
		return true;
	}
	if(_processing){
		start();
	}
	return false;
}

void media_player::video_main(semaphore &st){
	st.post();
	video_frame TRACE_CONSTRUCT(frm);
	while(_running){
		media_packet *pkt = NULL;
		{
			GUARD_LOCK(_vpkt_mtx);
			if(_vpkt_list.size() ||
				(_vpkt_cond.timed_wait(_vpkt_mtx, 10*1000*1000) && _vpkt_list.size())){
				_vdec_mtx.lock();
				pkt = _vpkt_list.remove_first();
				update_state();
			}
		}
		if(pkt){
			try{
				bool more_data;
				do{
					if(!(more_data = _vdec->decode_frame(*pkt, frm))){
						GUARD_LOCK(_pkt_mtx);
						_pkt_list.add(pkt);
						pkt = NULL;
					}
					if(_vdec->got_frame()){
						while(_running && !_vpl->push_video_noblock(frm));
					}
				}while(more_data && _running);
			}catch(const exception *e){
				fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
				#ifdef STACK_TRACE_FLAG
				exception::last_stack().print_stack(stderr);
				#endif
			}
			_vdec_mtx.unlock();
		}
	}
	if(_media_subtitle.raw_pointer()){
		_media_subtitle->clear_cache();
	}
}

void media_player::open_video FUNCTION_PARAMETER_LIST(size_t stm, video_player &vpl){
	_vdec.reassign(STACK_NEW(_pool, video_decoder, TRACE_PARAMS(_stream, stm)));
	vpl.change_scaler(_vdec->width(), _vdec->height(), _vdec->format());
	vpl.pause();
	vpl.synchronize_clock(0);
	_vpl = &vpl;
	semaphore st(TRACE_PARAMS(0));
	_vdec_thd.reassign(STACK_NEW(_pool, thread, TRACE_PARAMS(*this, &media_player::video_main, st)));
	st.wait();
}

void media_player::open_audio FUNCTION_PARAMETER_LIST(size_t s, const char *dev, size_t l, size_t m){
	_adec.reassign(STACK_NEW(_pool, audio_decoder, TRACE_PARAMS(_stream, s)));
	char devbuf[128];
	if(!strcmp(dev, "hdmi")){
		sprintf(devbuf, "hdmi:AES0=0x04");
	}else{
		snprintf(devbuf, sizeof(devbuf), "%s", dev);
	}
	_apl.reassign(STACK_NEW(_pool, audio_player, TRACE_PARAMS(devbuf, _adec->format(),
		_adec->channel_layout(), _adec->rate(), false, 100, l, m)));
	_apl->pause();
}

void media_player::open_passthrough_audio FUNCTION_PARAMETER_LIST(size_t s, const char *dev, size_t l, size_t m){
	char devbuf[128];
	if(!strcmp(dev, "hdmi")){
		sprintf(devbuf, "hdmi:AES0=0x06");
	}else{
		snprintf(devbuf, sizeof(devbuf), "%s", dev);
	}
	_pass_audio.reassign(STACK_NEW(_pool, passthrough_audio, TRACE_PARAMS(_stream, s, devbuf)));
	_pass_audio->player().pause();
}

void media_player::open_subtitle FUNCTION_PARAMETER_LIST(size_t s, const char *f, float fs, CCSDL2::sdl_color c,
	float pb, const char *st, const char *fc, bool upfc){
	if(_vdec.raw_pointer()){
		_media_subtitle.reassign(STACK_NEW(_pool, media_subtitle_decoder,
			TRACE_PARAMS(_stream, s, _vpl->picture_width(), _vpl->picture_height(),
				_vpl->width(), _vpl->height(), f, fs, c, pb, st, fc, upfc)));
		_vpl->change_subtitle(_media_subtitle.raw_pointer());
	}
}

void media_player::load_subtitle FUNCTION_PARAMETER_LIST(const char *sub, const char *f, float fs, CCSDL2::sdl_color c,
	const char *cs, float pb, const char *st, const char *fc, bool upfc){
	if(_vdec.raw_pointer()){
		_ext_subtitle.reassign(STACK_NEW(_pool, external_subtitle_decoder,
			TRACE_PARAMS(sub, _vpl->picture_width(), _vpl->picture_height(),
				_vpl->width(), _vpl->height(), f, fs, c, cs, pb, st, fc, upfc)));
		_vpl->change_subtitle(_ext_subtitle.raw_pointer());
	}
}

size_t media_player::rest_time FUNCTION_PARAMETER_LIST(){
	size_t rest = 0;
	if(_started){
		//some video has no duration info
		if(_vdec.raw_pointer() && _vdec->duration()){
			util::update_max(rest, _vdec->duration() - _vpl->current_clock());
		}
		if(_apl.raw_pointer()){
			util::update_max(rest, _apl->rest_time());
		}
		if(_pass_audio.raw_pointer()){
			util::update_max(rest, _pass_audio->player().rest_time());
		}
	}
	return rest;
}

bool media_player::overflow(){
	if(_more){
		return !insufficient();
	}
	if(_apl.raw_pointer()){
		return _apl->overflow();
	}else if(_pass_audio.raw_pointer()){
		return _pass_audio->player().overflow();
	}else if(_vdec.raw_pointer()){
		return _vpl->overflow();
	}
	return true;
}

bool media_player::insufficient(){
	bool suf = false;
	if((!suf) && _apl.raw_pointer()){
		suf = suf || _apl->insufficient();
	}
	if((!suf) && _pass_audio.raw_pointer()){
		suf = suf || _pass_audio->player().insufficient();
	}
	if(_vdec.raw_pointer()){
		suf = suf || _vpl->insufficient();
	}
	return suf;
}

void media_player::pause_internal(){
	if(_started){
		_started = false;
		if(_vdec.raw_pointer()){
			_vpl->pause();
		}
		if(_apl.raw_pointer()){
			_apl->pause();
		}
		if(_pass_audio.raw_pointer()){
			_pass_audio->player().pause();
		}
	}
}

void media_player::pause(){
	_processing = false;
	pause_internal();
}

void media_player::seek_to(size_t ms){
	pause_internal();
	GUARD_LOCK(_vpkt_mtx);
	GUARD_LOCK(_vdec_mtx);
	GUARD_LOCK(_pkt_mtx);
	while(_vpkt_list.first()){
		_pkt_list.add(_vpkt_list.remove_first());
	}
	if(_adec.raw_pointer()){
		_adec->flush_buffer();
		_apl->drop_all();
	}
	if(_vdec.raw_pointer()){
		_vdec->flush_buffer();
		_vpl->drop_all();
	}
	_stream.seek_frame(ms);
	media_packet pkt;
	audio_frame TRACE_CONSTRUCT(afrm);
	video_frame TRACE_CONSTRUCT(vfrm);
	
	while(_stream.read_packet(pkt)){
		try{
			if(_adec.raw_pointer() && _adec->contains_data(pkt)){
				while(_adec->decode_frame(pkt, afrm));
			}else if(_vdec.raw_pointer() && _vdec->contains_data(pkt)){
				while(_vdec->decode_frame(pkt, vfrm));
			}
			if(pkt.presentation_time() >= ms){
				return;
			}
		}catch(...){}
	}
}

CCMEDIA_END
