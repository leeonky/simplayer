#include <unistd.h>
#include <getopt.h>
#include "simplayer/player.h"
#include "simplayer/command_controller.h"
#include "ccmedia/media_player.h"
#include "ccmedia/cuesheet.h"
#include "cclibc/memory.h"
#include "cclibc/text.h"
#include "cclibc/io/file.h"
#include "cclibc/util/list.h"
#include "ccapplib/command_list.h"

using namespace ccmedia;
using namespace cclibc;
using namespace ccsdl2;
using namespace ccffmpeg;
using namespace ccapplib;
using namespace simplayer;

namespace{
	class player_settings{
	public:
		int x, y;
		int width, height;
		sdl_window::style video_style;
		int dump_stream;
		char tab_string[128];
		int pass_through;
		char sound_dev[128];
		int video_index, audio_index, sub_index;
		char ttf_font[1024];
		char ext_sub[1024];
		char ext_encoding[128];
		int start_track, end_track;
		char file[1024];
		int begin_track;
		player_settings()
			: x(0), y(0), width(800), height(450),	video_style(sdl_window::none), dump_stream(false), pass_through(false),
			video_index(-1), audio_index(-1), sub_index(-1), start_track(-1), end_track(-1), begin_track(-1){
			strcpy(tab_string, "\t");
			strcpy(sound_dev, "default");
			strcpy(ttf_font, "/usr/share/fonts/truetype/msyh.ttf");
			strcpy(ext_sub, "");
			strcpy(ext_encoding, "");
			strcpy(file, "");
		}
		player_settings(const player_settings &other)
			: x(other.x), y(other.y), width(other.width), height(other.height),
				video_style(other.video_style), dump_stream(other.dump_stream), pass_through(other.pass_through),
				video_index(-1), audio_index(-1), sub_index(-1), start_track(-1), end_track(-1), begin_track(-1){
			strcpy(tab_string, other.tab_string);
			strcpy(sound_dev, other.sound_dev);
			strcpy(ttf_font, "/usr/share/fonts/truetype/msyh.ttf");
			strcpy(ext_sub, "");
			strcpy(ext_encoding, "");
			strcpy(file, "");
		}
		void dump(){
			printf("%20s: %d,%d\n", "Location", x, y);
			printf("%20s: %d,%d\n", "Size", width, height);
			printf("%20s: %d\n", "Mode", video_style);
			printf("%20s: %s\n", "Dump stream", dump_stream ? "True" : "False");
			printf("%20s: %s\n", "Tab String", tab_string);
			printf("%20s: %s\n", "Pass-through", pass_through ? "True" : "False");
			printf("%20s: %s\n", "Sound Device", sound_dev);
			char buf[128];
			snprintf(buf, sizeof(buf), "%d", video_index);
			printf("%20s: %s\n", "Video", -1 == video_index ? "Auto" : buf);
			snprintf(buf, sizeof(buf), "%d", audio_index);
			printf("%20s: %s\n", "Autio", -1 == audio_index ? "Auto" : buf);
			snprintf(buf, sizeof(buf), "%d", sub_index);
			printf("%20s: %s\n", "Subtitle", -1 == sub_index ? "Auto" : buf);
			printf("%20s: %s\n", "Font", ttf_font);
			printf("%20s: %s\n", "External Subtitle", ext_sub);
			printf("%20s: %s\n", "External Charset", ext_encoding);
			printf("%20s: %d~%d\n", "Audio Track", start_track, end_track);
			printf("%20s: %s\n", "Target File", file);
		}
		void load_params(int c, char **args){
			int opt, option_index = 0;
			struct option long_options[] = {
				{"print", no_argument, &dump_stream, true},
				{"location", required_argument, NULL, 'l'},
				{"size", required_argument, NULL, 'z'},
				{"mode", required_argument, NULL, 'm'},
				{"video", required_argument, NULL, 'v'},
				{"audio", required_argument, NULL, 'a'},
				{"sub", required_argument, NULL, 's'},
				{"charset", required_argument, NULL, 'c'},
				{"pass", no_argument, &pass_through, true},
				{"device", required_argument, NULL, 'd'},
				{"track", required_argument, NULL, 't'},
				{"begin", required_argument, NULL, 'b'},
				{0, 0, 0, 0}};
			optind=1;
			while((opt = getopt_long_only(c, args, "l:z:m:v:a:s:c:d:t:", long_options, &option_index)) != -1){
				switch(opt){
					case 'l':
						sscanf(optarg, "%d,%d", &x, &y);
						break;
					case 'z':
						sscanf(optarg, "%d%*[x]%d", &width, &height);
						break;
					case 'm':
						int tmp;
						if(1 == sscanf(optarg, "%d", &tmp)){
							video_style = static_cast<sdl_window::style>(tmp);
						}
						break;
					case 'v':
						sscanf(optarg, "%d", &video_index);
						break;
					case 'a':
						sscanf(optarg, "%d", &audio_index);
						break;
					case 's':
						{
							bool is_num = true;
							for(int i = static_cast<int>(strlen(optarg))-1;
								i>=0 && (is_num = isdigit(optarg[i])); --i);
							if(is_num){
								sscanf(optarg, "%d", &sub_index);
							}else{
								char *chset = rindex(optarg, '?');
								if(chset){
									*chset++ = '\0';
									strncpy(ext_encoding, chset, sizeof(ext_encoding));
								}
								strncpy(ext_sub, optarg, sizeof(ext_sub));
							}
						}
						break;
					case 'c':
						sscanf(optarg, "%s", ext_encoding);
						break;
					case 'd':
						sscanf(optarg, "%s", sound_dev);
						break;
					case 't':
						if(2 != sscanf(optarg, "%d-%d", &start_track, &end_track)){
							if(1 == sscanf(optarg, "%d", &start_track)){
								end_track = start_track;
							}
						}
						break;
					case 'b':
						sscanf(optarg, "%d", &begin_track);
						break;
				}
			}
			strncpy(file, args[c-1], sizeof(file));
		}
	};
	void print_time(size_t t){
		t/=1000;
		int h = t/3600;
		int m = (t%3600)/60;
		int s = t%60;
		printf("%02d:%02d:%02d", h, m, s);
	}
}

SIMPLAYER_BG

class media_list{
public:
	enum file_type{
		media, cuefile,
	};
	class item: public CCLIBC::dual_list<item>::node{
	public:
		file_type type;
		stack_ptr<player_settings> setting;
		item(file_type t, stack_ptr<player_settings> &s)
			: type(t), setting(s.release()) {
		}
		virtual size_t media_count() const{
			return 1;
		}
		virtual ~item(){}
	};
	class media_item: public item{
	public:
		media_item(stack_ptr<player_settings> &s)
			: item(media, s){ }
	};
	class cue_item: public item{
	private:
	public:
		cuesheet cue;
		const cuesheet::track *start, *end;
		cue_item(stack_ptr<player_settings> &s)
			: item(cuefile, s), cue(TRACE_PARAMS(setting->file)),
			start(cue.tracks().first()), end(cue.tracks().last()) {
			if(!file::exist(cue.full_path())){
				THROW_NEW_EXCEPTION(CCLIBC::exception, "Target file '%s' of '%s' doesn't exist.", cue.full_path(), setting->file);
			}
			LIST_FOREACH(const cuesheet::track, tr, cue.tracks()){
				if(static_cast<int>(tr->index()) == setting->start_track){
					start = tr;
				}
				if(static_cast<int>(tr->index()) == setting->end_track){
					end = tr;
					break;
				}
			}
		}
		virtual size_t media_count() const{
			size_t c = 0;
			for(const cuesheet::track *tr = start, *te = end->next();
				tr != te; tr=tr->next()){
				++c;
			}
			return c;
		}
	};
	media_list(int c, char **args): _current_track(NULL) {
		stack_ptr<player_settings> com(STACK_NEW(_pool, player_settings));
		com->load_params(c, args);
		if(string_util::end_with_ig(com->file, ".cue")){
			list.add(STACK_NEW(_pool, cue_item, com));
		}else if(string_util::end_with_ig(com->file, ".lst")){
			load_list(args[0], *com);
			int i=1;
			while(i<com->start_track){
				item *it = list.first();
				if(cuefile == it->type){
					media_list::cue_item &ci = static_cast<media_list::cue_item&>(*it);
					if(static_cast<size_t>(com->start_track-i) >= ci.media_count()){
						list.remove(it);
						i+=ci.media_count();
					}else{
						for(;i<com->start_track; ++i){
							ci.start = ci.start->next();
						}
					}
				}else{
					list.remove(it);
					++i;
				}
			}
			if(com->end_track>0){
				item *rmit = list.first();
				while(rmit && i<=com->end_track){
					if(cuefile == rmit->type){
						media_list::cue_item &ci = static_cast<media_list::cue_item&>(*rmit);
						if(static_cast<size_t>(com->end_track-i+1) >= ci.media_count()){
							i += ci.media_count();
						}else{
							ci.end = ci.start;
							while(++i<=com->end_track){
								ci.end = ci.end->next();
							}
						}
					}else{
						++i;
					}
					rmit = rmit->next();
				}
				while(rmit){
					item *p = rmit->next();
					list.remove(rmit);
					rmit = p;
				}
			}
		}else{
			list.add(STACK_NEW(_pool, media_item, com));
		}
		_current = list.first();
		if(com.raw_pointer() && com->begin_track > 0){
			jump_to(com->begin_track);
		}
	}
	size_t index_of(const item &it, const cuesheet::track *tr = NULL){
		size_t id = 1;
		if(tr){
			const cuesheet::track *t = tr;
			while(t != static_cast<const media_list::cue_item&>(it).start && (t = t->previous())){
				++id;
			}
		}
		const media_list::item *t = &it;
		while((t = t->previous())){
			id += t->media_count();
		}
		return id;
	}
	item* next_item(){
		item *res = _current;
		if(_current){
			_current = _current->next();
		}
		return res;
	}
	const cuesheet::track* next_track(){
		const cuesheet::track *r = _current_track;
		_current_track = NULL;
		return r;
	}
	void prev_back(item &it){
		if(it.previous()){
			_current = it.previous();
			if(cuefile == _current->type){
				_current_track = static_cast<cue_item*>(_current)->end;
			}
		}
	}
	void jump_to(size_t i){
		LIST_FOREACH(item, it, list){
			if(1 == i){
				_current = it;
				return;
			}
			if(cuefile == it->type){
				media_list::cue_item &cuit = static_cast<media_list::cue_item&>(*it);
				for(const cuesheet::track *tr = cuit.start, *te = cuit.end->next();
					tr != te; tr=tr->next()){
					if(1 == i){
						_current = it;
						_current_track = tr;
						return;
					}
					--i;
				}
			}else{
				--i;
			}
		}
	}
private:
	void load_list(const char *arg0, const player_settings &com){
		command_list cmdlist(TRACE_PARAMS(arg0, com.file));
		LIST_FOREACH(const command_list::command_parameter, param, cmdlist.parameters()){
			stack_ptr<player_settings> setting(STACK_NEW(_pool, player_settings, com));
			setting->load_params(param->count(), clone_args(param->count(), param->arguments()));
			if(string_util::end_with_ig(setting->file, ".cue")){
				list.add(STACK_NEW(_pool, cue_item, setting));
			}else if(string_util::end_with_ig(setting->file, ".lst")){
				load_list(arg0, *setting);
			}else{
				list.add(STACK_NEW(_pool, media_item, setting));
			}
		}
	}
	char** clone_args(int c, char **args){
		size_t len = c * sizeof(char*);
		char** arg_array = static_cast<char**>(_pool.allocate(len));
		memcpy(arg_array, args, len);
		return arg_array;
	}
	item* _current;
	const cuesheet::track *_current_track;
	CCLIBC::stack_pool _pool;
	CCLIBC::dual_list<item> list;
};
	
class main_player{
private:
	static int media_index;
	static bool handle_stop(const char*, main_player&){
		return false;
	}
	static bool handle_seek_to(const char *cmd, main_player &pl){
		int ms;
		if(1 == sscanf(cmd, "%*s %d", &ms)){
			if(media_list::cuefile == pl.item.type){
				size_t t = pl.player->current_clock();
				const cuesheet::track *tr = static_cast<media_list::cue_item&>(pl.item).cue.find_track(t);
				if(tr){
					pl.player->seek_to(ms + tr->start());
				}
			}else{
				pl.player->seek_to(ms);
			}
		}
		return true;
	}
	static bool handle_seek_current(const char *cmd, main_player &pl){
		int ms;
		if(1 == sscanf(cmd, "%*s %d", &ms)){
			pl.player->seek_current(ms);
		}
		return true;
	}
	static bool handle_pause(const char*, main_player &pl){
		pl.player->pause();
		return true;
	}
	static bool handle_resume(const char*, main_player &pl){
		pl.player->resume();
		return true;
	}
	static bool handle_time(const char*, main_player &pl){
		size_t now = 0, duration = 0;
		if(media_list::cuefile == pl.item.type){
			size_t t = pl.player->current_clock();
			const cuesheet::track *tr = static_cast<media_list::cue_item&>(pl.item).cue.find_track(t);
			if(tr){
				now = t - tr->start();
				duration = (tr->end() ?
					tr->end() : pl.player->stream().duration(pl.item.setting->audio_index)) - tr->start();
			}
		}else{
			now = pl.player->played_time();
			duration = pl.player->duration();
		}
		print_time(now);
		printf("/");
		print_time(duration);
		printf("\n");
		fflush(stdout);
		return true;
	}
	static bool handle_title(const char*, main_player &pl){
		if(media_list::cuefile == pl.item.type){
			media_list::cue_item &cuit = static_cast<media_list::cue_item&>(pl.item);
			if(const cuesheet::track *tr = cuit.cue.find_track(pl.player->current_clock())){
				printf("%5d%sTrack%s%02d%s%s%s%s\n", static_cast<int>(pl.list.index_of(pl.item, tr)), pl.item.setting->tab_string, pl.item.setting->tab_string, static_cast<int>(tr->index()), 
					pl.item.setting->tab_string, tr->performer(), pl.item.setting->tab_string, tr->title());
			}
		}else{
			char *p =rindex(pl.item.setting->file, '/');
			printf("%5d%sFile%s%s\n", static_cast<int>(pl.list.index_of(pl.item)), pl.item.setting->tab_string, pl.item.setting->tab_string, p ? p+1 : pl.item.setting->file);
		}
		fflush(stdout);
		return true;
	}
	static bool handle_next(const char*, main_player &pl){
		if(media_list::cuefile == pl.item.type){
			media_list::cue_item &cuit = static_cast<media_list::cue_item&>(pl.item);
			if(const cuesheet::track *tr = cuit.cue.find_track(pl.player->current_clock())){
				if((tr = tr->next())){
					pl.player->seek_to(tr->start());
				}else{
					pl.moving = true;
				}
			}
		}else{
			pl.moving = true;
		}
		return true;
	}
	static bool handle_prev(const char*, main_player &pl){
		if(media_list::cuefile == pl.item.type){
			media_list::cue_item &cuit = static_cast<media_list::cue_item&>(pl.item);
			if(const cuesheet::track *tr = cuit.cue.find_track(pl.player->current_clock())){
				if(tr->previous()){
					tr = tr->previous();
					pl.player->seek_to(tr->start());
				}else{
					pl.list.prev_back(pl.item);
					pl.moving = true;
				}
			}
		}else{
			pl.list.prev_back(pl.item);
			pl.moving = true;
		}
		return true;
	}
	static bool handle_jump(const char *cmd, main_player &pl){
		int i;
		if(1 == sscanf(cmd, "%*s %d", &i) && i>0){
			pl.list.jump_to(i);
			pl.moving = true;
		}
		return true;
	}
	static bool handle_status(const char *cmd, main_player &pl){
		int index = -1, now = -1;
		if(media_list::cuefile == pl.item.type){
			size_t t = pl.player->current_clock();
			const cuesheet::track *tr = static_cast<media_list::cue_item&>(pl.item).cue.find_track(t);
			if(tr){
				now = t - tr->start();
			}
			media_list::cue_item &cuit = static_cast<media_list::cue_item&>(pl.item);
			if(const cuesheet::track *tr = cuit.cue.find_track(pl.player->current_clock())){
				index = static_cast<int>(pl.list.index_of(pl.item, tr));
			}
		}else{
			index = static_cast<int>(pl.list.index_of(pl.item));
			now = pl.player->played_time();
		}
		printf("%d%s", index, pl.item.setting->tab_string);
		print_time(now);
		if(pl.player->pausing()){
			printf("%s%s", pl.item.setting->tab_string, "pausing");
		}
		printf("\n");
		fflush(stdout);
		return true;
	}
public:
	object_ptr<cuesheet> cue;
	object_ptr<media_player> player;
	media_list::item &item;
	media_list &list;
	bool moving;
	main_player(media_list::item &it, media_list &l): item(it), list(l), moving(false) {}
	bool play(object_ptr<video_player> &vpl, const cuesheet::track *stt = NULL) {
		try{
			if(item.setting->dump_stream){
				if(media_list::cuefile == item.type){
					media_list::cue_item &ci = static_cast<media_list::cue_item&>(item);
					media_stream strm(TRACE_PARAMS(ci.cue.full_path()));
					int audio = strm.find_audio();
					if(-1 == audio){
						THROW_NEW_EXCEPTION(exception, "No audio for cue: %s", item.setting->file);
					}
					printf("Performer:%s%s\n", item.setting->tab_string, ci.cue.performer());
					printf("Title:%s%s\n", item.setting->tab_string, ci.cue.title());
					printf("Target:%s%s\n", item.setting->tab_string, ci.cue.full_path());
					for(const cuesheet::track *tr = ci.start, *te = ci.end->next();
						tr != te; tr=tr->next()){
						int du;
						if(tr->end()){
							du = tr->end() - tr->audio_start();
						}else{
							du = strm.duration(audio) - tr->audio_start();
						}
						du/=1000;
						char buf[128];
						snprintf(buf, sizeof(buf), "%02d:%02d", du/60, du%60);
						printf("%5d%sTrack%s%02d%s%s%s%s%s%s\n", media_index++,  item.setting->tab_string, item.setting->tab_string,
							static_cast<int>(tr->index()), item.setting->tab_string, buf,item.setting->tab_string,
							tr->performer(), item.setting->tab_string, tr->title());
					}
				}else if(media_list::media == item.type){
					media_stream strm(TRACE_PARAMS(item.setting->file));
					string_array<> strs;
					strm.media_info(strs);
					for(size_t i=0; i<strs.size(); i++){
						printf("Stream%s%s\n", item.setting->tab_string, strs[i]);
					}
					char *p = rindex(item.setting->file, '/');
					char buf[128];
					int du = strm.duration();
					if(!du){
						du = strm.max_duration();
					}
					du/=1000;
					snprintf(buf, sizeof(buf), "%02d:%02d", du/60, du%60);
					printf("%5d%sFile%s%s%s%s\n", media_index++,  item.setting->tab_string,
						item.setting->tab_string, p ? p+1 : item.setting->file, item.setting->tab_string, buf);
				}
			}else{
				const char *file = item.setting->file;
				size_t start = 0, end = 0;
				if(media_list::cuefile == item.type){
					media_list::cue_item &ci = static_cast<media_list::cue_item&>(item);
					start = ci.start->start();
					end = ci.end->end();
					file = ci.cue.full_path();
				}

				player.reassign(OBJECT_NEW(media_player, TRACE_PARAMS(file, start, end)));
				if(-1 == item.setting->video_index){
					item.setting->video_index = player->stream().find_video();
				}
				if(-1 == item.setting->audio_index){
					item.setting->audio_index = player->stream().find_audio();
				}
				if(-1 == item.setting->sub_index){
					item.setting->sub_index = player->stream().find_subtitle();
				}
				if(-1 != item.setting->video_index){
					if(!vpl.raw_pointer()){
						vpl.reassign(OBJECT_NEW(video_player,
							TRACE_PARAMS(item.setting->x, item.setting->y, item.setting->width, item.setting->height, item.setting->video_style)));
					}
					player->open_video(item.setting->video_index, *vpl);
					if(strlen(item.setting->ext_sub)){
						const char *str[] = {"utf8", "gb18030", "utf16le"};
						const char *p = encoding::detect_encoding(item.setting->ext_sub, sizeof(str)/sizeof(str[0]), str);
						player->load_subtitle(item.setting->ext_sub, item.setting->ttf_font, 1, sdl_color(255, 255, 255),
							strlen(item.setting->ext_encoding) ? item.setting->ext_encoding : (p ? p : "ascii"));
					}
					else if(-1 != item.setting->sub_index){
						player->open_subtitle(item.setting->sub_index, item.setting->ttf_font, 1, sdl_color(255, 255, 255));
					}
				}else if(vpl.raw_pointer()){
					vpl.reassign(NULL);
				}
				if(-1 != item.setting->audio_index){
					if(item.setting->pass_through && player->stream().bit_audio(item.setting->audio_index)){
						player->open_passthrough_audio(item.setting->audio_index, item.setting->sound_dev);
					}else{
						player->open_audio(item.setting->audio_index, item.setting->sound_dev);
					}
				}
				bool playing = true;
				command_controller<main_player> ctrl;
				ctrl.regist_command("stop", handle_stop, *this);
				ctrl.regist_command("to", handle_seek_to, *this);
				ctrl.regist_command("forward", handle_seek_current, *this);
				ctrl.regist_command("time", handle_time, *this);
				ctrl.regist_command("pause", handle_pause, *this);
				ctrl.regist_command("resume", handle_resume, *this);
				ctrl.regist_command("title", handle_title, *this);
				ctrl.regist_command("next", handle_next, *this);
				ctrl.regist_command("prev", handle_prev, *this);
				ctrl.regist_command("jump", handle_jump, *this);
				ctrl.regist_command("status", handle_status, *this);
				if(stt){
					player->seek_to(stt->start());
				}
				do{
					try{
						playing = player->process();
					}catch(const posix_error *e){
						throw;
					}catch(const exception *e){
						fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
						#ifdef STACK_TRACE_FLAG
						exception::last_stack().print_stack(stderr);
						#endif
						thread::sleep_nano(1000 * 1000);
					}
					ctrl.command_processing();
				}while(playing && ctrl.running() && !moving);
				if(ctrl.running() && !moving){
					thread::sleep_nano(player->rest_time()*static_cast<uint64_t>(1000*1000));
				}
				return ctrl.running();
			}
		}catch(const exception *e){
			fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
			#ifdef STACK_TRACE_FLAG
			exception::last_stack().print_stack(stderr);
			#endif
		}
		return true;
	}
};
int main_player::media_index = 1;

int play_main(int c, char** args){
	try{
		object_ptr<video_player> vpl;
		media_list list(c, args);
		media_list::item *it;
		bool run = true;
		while(run && (it = list.next_item())){
			main_player player(*it, list);
			run = player.play(vpl, list.next_track());
		}
		return 0;
	}catch(const exception *e){
		fprintf(stderr, "Got an exception: %s. At %s:%d\n", e->message(), e->file(), e->line());
		#ifdef STACK_TRACE_FLAG
		exception::last_stack().print_stack(stderr);
		#endif
		return 1;
	}
}

SIMPLAYER_END