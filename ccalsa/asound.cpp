#include "ccalsa/asound.h"
#include "cclibc/callc.h"

CCALSA_BG

alsa_sound::alsa_sound FUNCTION_PARAMETER_LIST(const char *dev, stream_types s, io_modes m)
	: _snd_pcm(NULL){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_open, &_snd_pcm, dev, static_cast<snd_pcm_stream_t
>(s), m);
}

alsa_sound::~alsa_sound(){
	debug_assert(!snd_pcm_close(_snd_pcm));
	_snd_pcm = NULL;
}

#define ALSA_CALL_RETURN_NERROR(function, ...)	({snd_pcm_sframes_t _r_tmp = function(__VA_ARGS__);\
if(_r_tmp < 0)\
		THROW_NEW_EXCEPTION(alsa_error, #function, _r_tmp);\
_r_tmp;})

size_t alsa_sound::interleaved_write FUNCTION_PARAMETER_LIST(const void *frames, size_t f){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_writei, _snd_pcm, frames, f);
}

void alsa_sound::prepare_device FUNCTION_PARAMETER_LIST(){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_prepare, _snd_pcm);
}

void alsa_sound::shutdown FUNCTION_PARAMETER_LIST(){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_drop, _snd_pcm);
}

bool alsa_sound::resume_device FUNCTION_PARAMETER_LIST(){
	switch(int r = snd_pcm_resume(_snd_pcm)){
		case 0:
			return true;
		case -EAGAIN:
			return false;
		default:
			THROW_NEW_EXCEPTION(alsa_error, "snd_pcm_resume", r);
	}
}

size_t alsa_sound::available_frames FUNCTION_PARAMETER_LIST(){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_avail, _snd_pcm);
}

size_t alsa_sound::available_update_frames FUNCTION_PARAMETER_LIST(){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_avail_update, _snd_pcm);
}

size_t alsa_sound::rewindable_frames FUNCTION_PARAMETER_LIST(){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_rewindable, _snd_pcm);
}

size_t alsa_sound::rewind_frames FUNCTION_PARAMETER_LIST(size_t f){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_rewind, _snd_pcm, f);
}

size_t alsa_sound::forwardable_frames FUNCTION_PARAMETER_LIST(){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_forwardable, _snd_pcm);
}

size_t alsa_sound::forward_frames FUNCTION_PARAMETER_LIST(size_t f){
	return ALSA_CALL_RETURN_NERROR(snd_pcm_forward, _snd_pcm, f);
}

ssize_t alsa_sound::delayed_frames FUNCTION_PARAMETER_LIST(){
	snd_pcm_sframes_t f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_delay, _snd_pcm, &f);
	return f;
}

bool alsa_sound::wait_for_available(int ms){
	return snd_pcm_wait(_snd_pcm, ms);
}

sound_parameters::sound_parameters FUNCTION_PARAMETER_LIST(alsa_sound &a): _alsa(a){
	int r;
	const char *fun = "snd_pcm_hw_params_malloc";
	if((r = snd_pcm_hw_params_malloc(&_snd_pcm_params)) >= 0){
		fun = "snd_pcm_hw_params_any";
		if((r = snd_pcm_hw_params_any(_alsa._snd_pcm, _snd_pcm_params)) >= 0){
			return;
		}
		snd_pcm_hw_params_free(_snd_pcm_params);
	}
	THROW_NEW_EXCEPTION(alsa_error, fun, r);
}

sound_parameters::~sound_parameters(){
	snd_pcm_hw_params_free(_snd_pcm_params);
	_snd_pcm_params = NULL;
}

sound_parameters::access_modes sound_parameters::access_mode FUNCTION_PARAMETER_LIST() const{
	snd_pcm_access_t t = static_cast<snd_pcm_access_t>(-1);
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_access, _snd_pcm_params, &t);
	return static_cast<access_modes>(t);
}
	
void sound_parameters::access_mode FUNCTION_PARAMETER_LIST(access_modes m){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_access, _alsa._snd_pcm, _snd_pcm_params, static_cast<snd_pcm_access_t>(m));
}

sound_parameters::data_formats sound_parameters::data_format FUNCTION_PARAMETER_LIST() const{
	snd_pcm_format_t t = static_cast<snd_pcm_format_t>(-1);
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_format, _snd_pcm_params, &t);
	return static_cast<data_formats>(t);
}

void sound_parameters::data_format FUNCTION_PARAMETER_LIST(data_formats f){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_format, _alsa._snd_pcm, _snd_pcm_params, static_cast<snd_pcm_format_t>(f));
}

bool sound_parameters::test_format FUNCTION_PARAMETER_LIST(data_formats f){
	return !snd_pcm_hw_params_test_format(_alsa._snd_pcm, _snd_pcm_params, static_cast<snd_pcm_format_t>(f));
}

size_t sound_parameters::sound_channels FUNCTION_PARAMETER_LIST() const{
	unsigned c = -1;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_channels, _snd_pcm_params, &c);
	return c;
}

void sound_parameters::sound_channels FUNCTION_PARAMETER_LIST(size_t c){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_channels, _alsa._snd_pcm, _snd_pcm_params, c);
}

size_t sound_parameters::sound_channels_near FUNCTION_PARAMETER_LIST(size_t c){
	unsigned tc = c;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_channels_near, _alsa._snd_pcm, _snd_pcm_params, &tc);
	return tc;
}

size_t sound_parameters::sample_rate FUNCTION_PARAMETER_LIST() const{
	unsigned c = -1;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_rate, _snd_pcm_params, &c, NULL);
	return c;
}

void sound_parameters::sample_rate FUNCTION_PARAMETER_LIST(size_t r){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_rate, _alsa._snd_pcm, _snd_pcm_params, r, 0);
}

size_t sound_parameters::sample_rate_near FUNCTION_PARAMETER_LIST(size_t r){
	unsigned ur = r;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_rate_near, _alsa._snd_pcm, _snd_pcm_params, &ur, NULL);
	return ur;
}

size_t sound_parameters::period_size FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t p = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_period_size, _snd_pcm_params, &p, NULL);
	return p;
}

void sound_parameters::period_size FUNCTION_PARAMETER_LIST(size_t f){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_size, _alsa._snd_pcm, _snd_pcm_params, f, 0);
}

size_t sound_parameters::period_size_near FUNCTION_PARAMETER_LIST(size_t f){
	snd_pcm_uframes_t uf = f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_size_near, _alsa._snd_pcm, _snd_pcm_params, &uf, NULL);
	return uf;
}

size_t sound_parameters::max_period_size FUNCTION_PARAMETER_LIST(size_t f){
	snd_pcm_uframes_t uf = f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_size_max, _alsa._snd_pcm, _snd_pcm_params, &uf, NULL);
	return uf;
}

size_t sound_parameters::max_period_size FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t uf = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_period_size_max, _snd_pcm_params, &uf, NULL);
	return uf;
}

size_t sound_parameters::min_period_size FUNCTION_PARAMETER_LIST(size_t f){
	snd_pcm_uframes_t uf = f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_size_min, _alsa._snd_pcm, _snd_pcm_params, &uf, NULL);
	return uf;
}

size_t sound_parameters::min_period_size FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t uf = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_period_size_min, _snd_pcm_params, &uf, NULL);
	return uf;
}

size_t sound_parameters::period_time FUNCTION_PARAMETER_LIST() const{
	unsigned us = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_period_time, _snd_pcm_params, &us, NULL);
	return us;
}

void sound_parameters::period_time FUNCTION_PARAMETER_LIST(size_t us){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_time, _alsa._snd_pcm, _snd_pcm_params, us, 0);
}

size_t sound_parameters::period_time_near FUNCTION_PARAMETER_LIST(size_t us){
	unsigned rus = us;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_time_near, _alsa._snd_pcm, _snd_pcm_params, &rus, NULL);
	return rus;
}
	
size_t sound_parameters::max_period_time FUNCTION_PARAMETER_LIST(size_t us){
	unsigned rus = us;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_time_max, _alsa._snd_pcm, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::max_period_time FUNCTION_PARAMETER_LIST() const{
	unsigned rus = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_period_time_max, _snd_pcm_params, &rus, NULL);
	return rus;
}
	
size_t sound_parameters::min_period_time FUNCTION_PARAMETER_LIST(size_t us){
	unsigned rus = us;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_period_time_min, _alsa._snd_pcm, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::min_period_time FUNCTION_PARAMETER_LIST() const{
	unsigned rus = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_period_time_min, _snd_pcm_params, &rus, NULL);
	return rus;
}

void sound_parameters::install_to_device FUNCTION_PARAMETER_LIST(){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params, _alsa._snd_pcm, _snd_pcm_params);
}
	
void sound_parameters::buffer_size FUNCTION_PARAMETER_LIST(size_t f){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_size, _alsa._snd_pcm, _snd_pcm_params, f);
}

size_t sound_parameters::buffer_size FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t p = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_buffer_size, _snd_pcm_params, &p);
	return p;
}

size_t sound_parameters::buffer_size_near FUNCTION_PARAMETER_LIST(size_t f){
	snd_pcm_uframes_t p = f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_size_near, _alsa._snd_pcm, _snd_pcm_params, &p);
	return p;
}
	
size_t sound_parameters::max_buffer_size FUNCTION_PARAMETER_LIST(size_t f){
	snd_pcm_uframes_t p = f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_size_max, _alsa._snd_pcm, _snd_pcm_params, &p);
	return p;
}

size_t sound_parameters::max_buffer_size FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t p = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_buffer_size_max, _snd_pcm_params, &p);
	return p;
}
	
size_t sound_parameters::min_buffer_size FUNCTION_PARAMETER_LIST(size_t f){
	snd_pcm_uframes_t p = f;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_size_min, _alsa._snd_pcm, _snd_pcm_params, &p);
	return p;
}

size_t sound_parameters::min_buffer_size FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t p = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_buffer_size_min, _snd_pcm_params, &p);
	return p;
}

void sound_parameters::buffer_time FUNCTION_PARAMETER_LIST(size_t us){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_time, _alsa._snd_pcm, _snd_pcm_params, us, 0);
}

size_t sound_parameters::buffer_time FUNCTION_PARAMETER_LIST() const{
	unsigned rus = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_buffer_time, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::buffer_time_near FUNCTION_PARAMETER_LIST(size_t us){
	unsigned rus = us;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_time_near, _alsa._snd_pcm, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::max_buffer_time FUNCTION_PARAMETER_LIST(size_t us){
	unsigned rus = us;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_time_max, _alsa._snd_pcm, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::max_buffer_time FUNCTION_PARAMETER_LIST() const{
	unsigned rus = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_buffer_time_max, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::min_buffer_time FUNCTION_PARAMETER_LIST(size_t us){
	unsigned rus = us;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_set_buffer_time_min, _alsa._snd_pcm, _snd_pcm_params, &rus, NULL);
	return rus;
}

size_t sound_parameters::min_buffer_time FUNCTION_PARAMETER_LIST() const{
	unsigned rus = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_hw_params_get_buffer_time_min, _snd_pcm_params, &rus, NULL);
	return rus;
}


soft_parameters::soft_parameters FUNCTION_PARAMETER_LIST(alsa_sound &a): _alsa(a){
	int r;
	const char *fun = "snd_pcm_sw_params_malloc";
	if((r = snd_pcm_sw_params_malloc(&_snd_pcm_params)) >= 0){
		fun = "snd_pcm_sw_params_current";
		if((r = snd_pcm_sw_params_current(_alsa._snd_pcm, _snd_pcm_params)) >= 0){
			return;
		}
		snd_pcm_sw_params_free(_snd_pcm_params);
		_snd_pcm_params = NULL;
	}
	THROW_NEW_EXCEPTION(alsa_error, fun, r);
}

soft_parameters::~soft_parameters(){
	snd_pcm_sw_params_free(_snd_pcm_params);
	_snd_pcm_params = NULL;
}
	
void soft_parameters::waited_period FUNCTION_PARAMETER_LIST(size_t f){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_sw_params_set_avail_min, _alsa._snd_pcm, _snd_pcm_params, f);
}

size_t soft_parameters::waited_period FUNCTION_PARAMETER_LIST() const{
	snd_pcm_uframes_t p = 0;
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_sw_params_get_avail_min, _snd_pcm_params, &p);
	return p;
}

void soft_parameters::install_to_device FUNCTION_PARAMETER_LIST(){
	INVOKE_THROW_RETURN(int, < 0, alsa_error, snd_pcm_sw_params, _alsa._snd_pcm, _snd_pcm_params);
}

CCALSA_END
