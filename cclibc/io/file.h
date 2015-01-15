#ifndef CCLIBC_IO_FILE_H_
#define CCLIBC_IO_FILE_H_

#include <unistd.h>
#include "cclibc/env.h"
#include "cclibc/trace.h"
#include "cclibc/io/file_descriptor.h"
#include "cclibc/memory.h"

CCLIBC_BG

class bom{
public:
	const uint8_t *header;
	const size_t len;
	
	template<size_t S>
	bom(uint8_t (&h)[S]): header(h), len(S){ }
	bool match(file_descriptor &fl) const{
		uint8_t bom[10] = {};
		size_t l = fl.block_read(bom, sizeof(bom));
		if(l >= len && (!memcmp(header, bom, len))){
			fl.seek_set(len);
			return true;
		}
		fl.seek_set(0);
		return false;
	}
	static size_t trim_bom(file_descriptor &fl){
		static uint8_t utf8[] = {0xEF, 0xBB, 0xBF};
		static uint8_t utf16b[] = {0xFE, 0xFF};
		static uint8_t utf16s[] = {0xFF, 0xFE};
		static bom bs[] = {bom(utf8), bom(utf16b), bom(utf16s)};
		for(size_t i=0; i<sizeof(bs)/sizeof(bs[0]); ++i){
			if(bs[i].match(fl)){
				return bs[i].len;
			}
		}
		return 0;
	}
};

#ifdef STACK_TRACE_FLAG

#define file_size(...) file_size(INVOKE_TRACE,## __VA_ARGS__)
#define read_all(...) read_all(INVOKE_TRACE,## __VA_ARGS__)
#define write_all(...) write_all(INVOKE_TRACE,## __VA_ARGS__)
#define read_all_text(...) read_all_text(INVOKE_TRACE,## __VA_ARGS__)

#endif
class file: public file_descriptor{
public:
	file FUNCTION_PARAMETER_LIST(const char*,
		file_descriptor::mode m = file_descriptor::readwrite | file_descriptor::create);

	static uint64_t file_size FUNCTION_PARAMETER_LIST(const char*);

	template<typename POOL>
	static void* read_all FUNCTION_PARAMETER_LIST(const char *f){
		uint64_t s = file_size(f);
		debug_assert(s<0x80000000);
		pool_ptr<POOL> r_tmp(POOL::instance().allocate(s));
		file fl(TRACE_PARAMS(f, file_descriptor::readonly));
		fl.block_read(r_tmp, s);
		return r_tmp.release();
	}

	template<typename POOL>
	static void* read_all_text FUNCTION_PARAMETER_LIST(const char *f, size_t &len){
		uint64_t s = file_size(f);
		debug_assert(s<0x80000000);
		file fl(TRACE_PARAMS(f, file_descriptor::readonly));
		size_t bl = bom::trim_bom(fl);
		len=s-bl;
		pool_ptr<POOL> r_tmp(POOL::instance().allocate(len+1));
		fl.block_read(r_tmp, len);
		static_cast<char*>(r_tmp.raw_pointer())[len] = '\0';
		return r_tmp.release();
	}
	
	static void* read_all FUNCTION_PARAMETER_LIST(stack_pool &pool, const char *f){
		uint64_t s = file_size(f);
		debug_assert(s<0x80000000);
		void *r_tmp = pool.allocate(s);
		file fl(TRACE_PARAMS(f, file_descriptor::readonly));
		fl.block_read(r_tmp, s);
		return r_tmp;
	}

	static void* read_all_text FUNCTION_PARAMETER_LIST(stack_pool &pool, const char *f, size_t &len){
		uint64_t s = file_size(f);
		debug_assert(s<0x80000000);
		file fl(TRACE_PARAMS(f, file_descriptor::readonly));
		size_t bl = bom::trim_bom(fl);
		len=s-bl;
		void *r_tmp = pool.allocate(len+1);
		fl.block_read(r_tmp, len);
		static_cast<char*>(r_tmp)[len] = '\0';
		return r_tmp;
	}
	
	static void write_all FUNCTION_PARAMETER_LIST(const char *f, const void *data, size_t len){
		file fl(TRACE_PARAMS(f, file_descriptor::readwrite));
		fl.block_write(data, len);
	}
	
	static bool exist(const char *f){
		return !access(f, F_OK);
	}
};

CCLIBC_END

#endif // CCLIBC_IO_FILE_H_
