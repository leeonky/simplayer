#ifndef CCMEDIA_MEDIA_PLAYER_H_
#define CCMEDIA_MEDIA_PLAYER_H_

#include "ccmedia/env.h"
#include "ccmedia/video_player.h"
#include "ccmedia/audio_player.h"
#include "ccmedia/passthrough_audio.h"
#include "ccmedia/cuesheet.h"


CCMEDIA_BG

#ifdef STACK_TRACE_FLAG

#define open_video(...) open_video(INVOKE_TRACE, ##__VA_ARGS__)
#define open_audio(...) open_audio(INVOKE_TRACE, ##__VA_ARGS__)
#define open_passthrough_audio(...) open_passthrough_audio(INVOKE_TRACE, ##__VA_ARGS__)
#define open_subtitle(...) open_subtitle(INVOKE_TRACE, ##__VA_ARGS__)
#define load_subtitle(...) load_subtitle(INVOKE_TRACE, ##__VA_ARGS__)
#define process(...) process(INVOKE_TRACE, ##__VA_ARGS__)

#endif

class media_player{
private:
	CCFFMPEG::media_stream _stream;
	CCLIBC::stack_pool _pool;
	bool _running;
	bool _started;
	bool _processing;

	CCLIBC::mutex _pkt_mtx;
	CCLIBC::single_list<CCFFMPEG::media_packet> _pkt_list;
	
	CCLIBC::stack_ptr<CCLIBC::thread> _vdec_thd;
	CCLIBC::stack_ptr<CCFFMPEG::video_decoder> _vdec;
	video_player *_vpl;
	CCLIBC::mutex _vpkt_mtx, _vdec_mtx;
	CCLIBC::condition _vpkt_cond;
	CCLIBC::single_list<CCFFMPEG::media_packet> _vpkt_list;

	void video_main(CCLIBC::semaphore&);

	CCLIBC::stack_ptr<CCFFMPEG::audio_decoder> _adec;
	CCLIBC::stack_ptr<audio_player> _apl;
	
	CCLIBC::stack_ptr<passthrough_audio> _pass_audio;
	CCLIBC::stack_ptr<media_subtitle_decoder> _media_subtitle;
	CCLIBC::stack_ptr<external_subtitle_decoder> _ext_subtitle;

	const size_t _most_buffer;
	bool _more;
	void update_state(){
		_more = _vpkt_list.size() >= _most_buffer;
	}
	size_t _play_start;
	size_t _play_end;
	void pause_internal();
	void start();
	bool read_packet FUNCTION_PARAMETER_LIST();

public:
	media_player FUNCTION_PARAMETER_LIST(const char*, size_t st = 0, size_t du = 0);
	~media_player();
	void open_video FUNCTION_PARAMETER_LIST(size_t, video_player&);
	void open_audio FUNCTION_PARAMETER_LIST(size_t, const char *dev = "default", size_t least = 2, size_t most = 10);
	void open_passthrough_audio FUNCTION_PARAMETER_LIST(size_t, const char *dev = "hdmi:AES0=0x06", size_t least = 2, size_t most = 10);
	void open_subtitle FUNCTION_PARAMETER_LIST(size_t, const char*, float, CCSDL2::sdl_color,
		float pb = 0.95, const char *s = "Sans", const char *fc = NULL, bool upfc = true);
	void load_subtitle FUNCTION_PARAMETER_LIST(const char*, const char*, float, CCSDL2::sdl_color,
		const char *cs = "UTF-8", float pb = 0.95, const char *s = "Sans", const char *fc = NULL, bool upfc = true);
	bool process FUNCTION_PARAMETER_LIST();
	CCFFMPEG::media_stream& stream(){
		return _stream;
	}
	size_t rest_time FUNCTION_PARAMETER_LIST();
	bool overflow();
	bool insufficient();
	bool pausing() const{
		return !_processing;
	}
	void pause();
	void resume(){
		_processing = true;
	}
	size_t current_clock FUNCTION_PARAMETER_LIST(){
		size_t pt = 0;
		if(_adec.raw_pointer()){
			CCLIBC::util::update_max(pt, _apl->current_clock());
		}
		else if(_pass_audio.raw_pointer()){
			CCLIBC::util::update_max(pt, _pass_audio->player().current_clock());
		}
		else if(_vdec.raw_pointer()){
			CCLIBC::util::update_max(pt, _vpl->current_clock());
		}
		return pt;
	}
	void seek_current(int ms){
		seek_to(static_cast<int>(current_clock()) + ms);
	}
	void seek_to(size_t);
	
	void prepare(){
		if(_play_start){
			seek_to(_play_start);
		}
	}
	
	size_t played_time() {
		return current_clock() - _play_start;
	}
	
	size_t duration() const{
		size_t rest = _stream.duration();
		if(_started){
			if(_vdec.raw_pointer()){
				CCLIBC::util::update_max(rest, _vdec->duration());
			}
			if(_adec.raw_pointer()){
				CCLIBC::util::update_max(rest, _adec->duration());
			}
			if(_pass_audio.raw_pointer()){
				CCLIBC::util::update_max(rest, _pass_audio->duration());
			}
		}
		if(rest){
			if(_play_end){
				CCLIBC::util::update_min(rest, _play_end);
			}
			rest -= _play_start;
		}
		return rest;
	}
};

CCMEDIA_END

#endif //CCMEDIA_MEDIA_PLAYER_H_
