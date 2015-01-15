#ifndef CCAPPLIB_STRING_UTIL_H_
#define CCAPPLIB_STRING_UTIL_H_

#include "ccapplib/env.h"
#include "cclibc/memory.h"
#include "cclibc/util/array_list.h"

CCAPPLIB_BG

template<typename POOL = typename CCLIBC::allocator>
class string_array{
private:
	CCLIBC::array_list<char*> _values;

public:
	size_t size() const{
		return _values.size();
	}

	char* operator[](size_t index) const{
		return _values[index];
	}

	void add FUNCTION_PARAMETER_LIST(CCLIBC::pool_ptr<POOL> &ptr){
		_values.add(static_cast<char*>(ptr.raw_pointer()));
		ptr.release();
	}

	void add FUNCTION_PARAMETER_LIST(const char *str){
		CCLIBC::pool_ptr<POOL> str_ptr(POOL::instance().allocate("%s", str));
		_values.add(static_cast<char*>(str_ptr.raw_pointer()));
		str_ptr.release();
	}

	void clear_all(){
		for(size_t i=0; i<_values.size(); i++){
			POOL::instance().deallocate(_values[i]);
		}
		_values.clear();
	}

	~string_array(){
		clear_all();
	}
};

#define INVALID_UTF16_CHAR 0xFFFF
class string_util{
private:
	static bool can_be_stopped(uint16_t c){
		const char *stopper = ",.!?;'\"}]|>)";
		return (c<=0x9FA5 && c>= 0x4E00) || index(stopper, c);
	}
	template<typename A, typename F, typename POOL>
	static void split_line_by_size_tp FUNCTION_PARAMETER_LIST(F fun, A arg, const char *s, size_t w, string_array<POOL> &out){
		debug_assert(fun);
		debug_assert(s);
		if(size_t tw = fun(arg, s)){
			CCLIBC::pool_ptr<POOL> tempstr(POOL::instance().allocate("%s", s));
			const char *start = static_cast<char*>(tempstr.raw_pointer());
			size_t slen = strlen(start);
			const char *next = start, *stopper = NULL;
			uint16_t c;
			bool changeline = false, is_lasf_char_0D = false;
			int isskip = 0;
			while((c = uni16_char(next, slen)) != INVALID_UTF16_CHAR){
				switch(c){
					case '\n':
						if(is_lasf_char_0D){
							is_lasf_char_0D = false;
							++start;
							continue;
						}
						changeline = true;
						stopper = next - 1;
						isskip = 1;
						break;
					case '\r':
						is_lasf_char_0D = true;
						changeline = true;
						stopper = next - 1;
						isskip = 1;
						break;
					case ' ':
						stopper = next - 1;
						isskip = 1;
					default:
						is_lasf_char_0D = false;
						break;
				}
				if(stopper){
					char save = *next;
					*const_cast<char*>(next) = '\0';
					if(changeline || (tw=fun(arg, start)) > w){
						size_t ln = stopper-start+1;
						CCLIBC::pool_ptr<POOL> linestr(POOL::instance().allocate(ln));
						memcpy(linestr.raw_pointer(), start, ln);
						static_cast<char*>(linestr.raw_pointer())[ln-1]='\0';
						out.add(linestr);
						start = stopper + isskip;
						stopper = NULL;
						changeline = false;
						isskip = 0;
					}
					*const_cast<char*>(next) = save;
				}
				if(can_be_stopped(c)){
					stopper = next;
				}
			}
			if(start!=stopper){
				out.add(start);
			}
		}
	}
public:
	static uint16_t uni16_char(const char *&s, size_t &len){
		uint16_t c = INVALID_UTF16_CHAR;
		if(len){
			if((s[0]&0xE0)==0xE0 && len>=3 ){
				c = ((s[0]&0xf)<<12) + ((s[1]&0x3F)<<6) + (s[2]&0x3F);
				len-=3;
				s+=3;
			}else if((s[0]&0xC0)==0xC0 && len>=2 ){
				c = ((s[0]&0x1F)<<6) + (s[1]&0x3F);
				len-=2;
				s+=2;
			}else if((s[0]&0x80) == 0){
				c = s[0] & 0x7f;
				--len;
				++s;
			}
		}
		return c;
	}
	template<typename A1, typename A2, typename POOL>
	static void split_line_by_size FUNCTION_PARAMETER_LIST(size_t(*spfun)(A1, const char*), A2& arg, const char *s, size_t w, string_array<POOL> &out){
		split_line_by_size_tp<A2&>(TRACE_PARAMS(spfun, arg, s, w, out));
	}
	
	template<typename A1, typename A2, typename POOL>
	static void split_line_by_size FUNCTION_PARAMETER_LIST(size_t(*spfun)(A1, const char*), const A2& arg, const char *s, size_t w, string_array<POOL> &out){
		split_line_by_size_tp<const A2&>(TRACE_PARAMS(spfun, arg, s, w, out));
	}

	template<typename POOL>
	static void split_line FUNCTION_PARAMETER_LIST(const char *s, string_array<POOL> &out){
		if(!s){
			s = "";
		}
		CCLIBC::pool_ptr<POOL> tempstr(POOL::instance().allocate("%s", s));
		const char *start = static_cast<char*>(tempstr.raw_pointer());
		const char *next = start;
		uint16_t c;
		bool changeline = false, is_lasf_char_0D = false;
		while((c = *next++)){
			switch(c){
				case '\n':
					if(is_lasf_char_0D){
						is_lasf_char_0D = false;
						++start;
						continue;
					}
					changeline = true;
					break;
				case '\r':
					is_lasf_char_0D = true;
					changeline = true;
					break;
				default:
					is_lasf_char_0D = false;
					break;
			}
			if(changeline){
				size_t ln = next-start;
				CCLIBC::pool_ptr<POOL> linestr(POOL::instance().allocate(ln));
				memcpy(linestr.raw_pointer(), start, ln-1);
				static_cast<char*>(linestr.raw_pointer())[ln-1]='\0';
				out.add(linestr);
				start = next;
				changeline = false;
			}
		}
		if(start == static_cast<char*>(tempstr.raw_pointer()) || start != next){
			out.add(start);
		}
	}
	
	static bool end_with(const char*, const char*);
	static bool end_with_ig(const char*, const char*);
};
#ifdef STACK_TRACE_FLAG

#define split_line_by_size(...) split_line_by_size(INVOKE_TRACE, ##__VA_ARGS__)
#define split_line(...) split_line(INVOKE_TRACE, ##__VA_ARGS__)

#endif

CCAPPLIB_END

#endif //CCAPPLIB_STRING_H_
