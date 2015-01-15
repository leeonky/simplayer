#include <SDL2/SDL.h>
#include "ccsdl2_mock_test/video_test.h"
#include "ccsdl2/video.h"
#include "ccsdl2/ttf.h"

using CCSDL2::sdl_video;
using CCSDL2::sdl_window;
using CCSDL2::sdl_renderer;
using CCSDL2::sdl_texture;
using CCSDL2::sdl_color;
using CCSDL2::sdl_rect;
using CCSDL2::sdl_locked_txture;

void video_test_class::video_create_des_window(){
	void *p;
	{
		sdl_window::style s = static_cast<sdl_window::style>(255);
		sdl_window wnd0(TRACE_PARAMS("", 0, 1, 2, 3, s));
		interface_testor::clear();
		sdl_window wnd(TRACE_PARAMS("", 0, 1, 2, 3, s));
		p = wnd._wnd;
		CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_CreateWindow"));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(""));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(0));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(1));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(2));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(3));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(5).equal(255));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_DestroyWindow"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));
}

void video_test_class::window_size(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	interface_testor::clear();
	wnd.width();
	wnd.height();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_GetWindowSize")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(wnd._wnd));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_GetWindowSize")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(wnd._wnd));

	interface_testor::clear();
	wnd.size(200, 100);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_SetWindowSize")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(wnd._wnd));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(200));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(100));
}

void video_test_class::window_position(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	interface_testor::clear();
	wnd.x();
	wnd.y();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_GetWindowPosition"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(wnd._wnd));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_GetWindowPosition"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(wnd._wnd));

	interface_testor::clear();
	wnd.position(10,20);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_SetWindowPosition")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(wnd._wnd));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(10));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(20));

}

void video_test_class::renderer_init_quit(){
	void *p;
	{
		sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
		interface_testor::clear();
		sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
		p = rnder._renderer;
	
		CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_CreateRenderer")); 
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(wnd._wnd));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(10));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SDL_RENDERER_SOFTWARE));
	}
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_DestroyRenderer"));
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(p));

}

void video_test_class::texture_init_quit(){
	void *p;
	{
		sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
		sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
		interface_testor::clear();
		sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200, sdl_renderer::blend));
		p = txture._texture;
	
		CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_CreateTexture")); 
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal((uint32_t)SDL_PIXELFORMAT_YV12));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(SDL_TEXTUREACCESS_STREAMING));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(100));
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(4).equal(200));
		CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_QueryTexture")); 
		CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(txture._texture));
		CPPUNIT_ASSERT(interface_testor::method(2).equal("SDL_SetTextureBlendMode")); 
		CPPUNIT_ASSERT(interface_testor::method(2).parameter(0).equal(txture._texture));
		CPPUNIT_ASSERT(interface_testor::method(2).parameter(1).equal(SDL_BLENDMODE_BLEND));
	}
	CPPUNIT_ASSERT(interface_testor::method(3).equal("SDL_DestroyTexture"));
	CPPUNIT_ASSERT(interface_testor::method(3).parameter(0).equal(p));

}

void video_test_class::sdl_rect_properties(){
	sdl_rect r(1,2,3,4);
	CPPUNIT_ASSERT(r.x == 1);
	CPPUNIT_ASSERT(r.y == 2);
	CPPUNIT_ASSERT(r.w == 3);
	CPPUNIT_ASSERT(r.h == 4);
	r.x = 4;
	r.y = 3;
	r.w = 2;
	r.h = 1;
	CPPUNIT_ASSERT(r.x == 4);
	CPPUNIT_ASSERT(r.y == 3);
	CPPUNIT_ASSERT(r.w == 2);
	CPPUNIT_ASSERT(r.h == 1);
}

void video_test_class::texture_lock_unlock(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	interface_testor::clear();
	void *p;
	size_t l;
	txture.lock(p, l);
	txture.unlock();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_LockTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(txture._texture));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_UnlockTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(txture._texture));
	
	sdl_rect rc(0,0,100,200);
	interface_testor::clear();
	txture.lock(rc, p, l);
	txture.unlock();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_LockTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(txture._texture));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(&rc));
	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_UnlockTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(txture._texture));
}

void video_test_class::renderer_copy(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	interface_testor::clear();
	rnder.copy_texture(txture);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_RenderCopy"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(txture._texture));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(NULL));
	
	sdl_rect rc(0,0,100,200);
	interface_testor::clear();
	rnder.copy_texture(rc, txture);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_RenderCopy"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(txture._texture));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(NULL));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(&rc));
	
	interface_testor::clear();
	rnder.copy_texture(txture, rc);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_RenderCopy"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(txture._texture));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(&rc));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(NULL));

	sdl_rect rc2(10,10,20,20);
	interface_testor::clear();
	rnder.copy_texture(rc, txture, rc2);
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_RenderCopy"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(1).equal(txture._texture));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(&rc2));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(3).equal(&rc));
}

void video_test_class::renderer_present(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
	interface_testor::clear();
	rnder.present();
	CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_RenderPresent"));
	CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(rnder._renderer));
}

void video_test_class::sdl_color_properties(){
	sdl_color c(1,2,3,4);
	CPPUNIT_ASSERT(c.r == 1);
	CPPUNIT_ASSERT(c.g == 2);
	CPPUNIT_ASSERT(c.b == 3);
	CPPUNIT_ASSERT(c.a == 4);
	
	c.r = 4;
	c.g = 3;
	c.b = 2;
	c.a = 1;
	CPPUNIT_ASSERT(c.r == 4);
	CPPUNIT_ASSERT(c.g == 3);
	CPPUNIT_ASSERT(c.b == 2);
	CPPUNIT_ASSERT(c.a == 1);
}

void video_test_class::texture_lock_return_obj(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	interface_testor::clear();
	sdl_rect rc(0,0,1,2);
	{
		sdl_locked_txture lctxt(TRACE_PARAMS(txture, rc));
		CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_LockTexture")); 
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(txture._texture));

		CPPUNIT_ASSERT(lctxt.locked_rect().x == 0);
		CPPUNIT_ASSERT(lctxt.locked_rect().y == 0);
		CPPUNIT_ASSERT(lctxt.locked_rect().w == 1);
		CPPUNIT_ASSERT(lctxt.locked_rect().h == 2);
		CPPUNIT_ASSERT(&lctxt.texture() == &txture);
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(&lctxt._data));
	}

	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_UnlockTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(txture._texture));
}

void video_test_class::texture_lock_all_return_obj(){
	sdl_window wnd(TRACE_PARAMS("", 0, 0, 200, 100, sdl_window::none));
	sdl_renderer rnder(TRACE_PARAMS(wnd, sdl_renderer::software, 10));
	sdl_texture txture(TRACE_PARAMS(rnder, sdl_texture::yv12, sdl_texture::streaming, 100, 200));
	interface_testor::clear();
	{
		sdl_locked_txture lctxt(TRACE_PARAMS(txture));
		CPPUNIT_ASSERT(interface_testor::method(0).equal("SDL_LockTexture")); 
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(0).equal(txture._texture));

		CPPUNIT_ASSERT(lctxt.locked_rect().x == 0);
		CPPUNIT_ASSERT(lctxt.locked_rect().y == 0);
		CPPUNIT_ASSERT(lctxt.locked_rect().w == (int)txture.width());
		CPPUNIT_ASSERT(lctxt.locked_rect().h == (int)txture.height());
		CPPUNIT_ASSERT(&lctxt.texture() == &txture);
		CPPUNIT_ASSERT(interface_testor::method(0).parameter(2).equal(&lctxt._data));
	}

	CPPUNIT_ASSERT(interface_testor::method(1).equal("SDL_UnlockTexture")); 
	CPPUNIT_ASSERT(interface_testor::method(1).parameter(0).equal(txture._texture));
}

void video_test_class::sdl_rect_intersect_self(){
	sdl_rect r(1,2,3,4);
	r.intersect(r);
	CPPUNIT_ASSERT(r.x == 1);
	CPPUNIT_ASSERT(r.y == 2);
	CPPUNIT_ASSERT(r.w == 3);
	CPPUNIT_ASSERT(r.h == 4);
}

void video_test_class::sdl_rect_intersect_same(){
	sdl_rect r(1,2,3,4), r0(1,2,3,4);
	r.intersect(r0);
	CPPUNIT_ASSERT(r.x == 1);
	CPPUNIT_ASSERT(r.y == 2);
	CPPUNIT_ASSERT(r.w == 3);
	CPPUNIT_ASSERT(r.h == 4);
}

void video_test_class::sdl_rect_contains(){
	sdl_rect r(0, 0, 3, 2);

	CPPUNIT_ASSERT(!r.contains(-1,-1));
	CPPUNIT_ASSERT(!r.contains(0 ,-1));
	CPPUNIT_ASSERT(!r.contains(1 ,-1));
	CPPUNIT_ASSERT(!r.contains(2 ,-1));
	CPPUNIT_ASSERT(!r.contains(3 ,-1));
	
	CPPUNIT_ASSERT(!r.contains(-1,0));
	CPPUNIT_ASSERT(r.contains(0,0));
	CPPUNIT_ASSERT(r.contains(1,0));
	CPPUNIT_ASSERT(r.contains(2,0));
	CPPUNIT_ASSERT(!r.contains(3,0));
	
	CPPUNIT_ASSERT(!r.contains(-1,1));
	CPPUNIT_ASSERT(r.contains(0,1));
	CPPUNIT_ASSERT(r.contains(1,1));
	CPPUNIT_ASSERT(r.contains(2,1));
	CPPUNIT_ASSERT(!r.contains(3,1));


	CPPUNIT_ASSERT(!r.contains(-1,2));
	CPPUNIT_ASSERT(!r.contains(0 ,2));
	CPPUNIT_ASSERT(!r.contains(1 ,2));
	CPPUNIT_ASSERT(!r.contains(2 ,2));
	CPPUNIT_ASSERT(!r.contains(3 ,2));
}
namespace{
	void intersect_check(const sdl_rect &ri, const sdl_rect &r1, const sdl_rect &r2){
		try{
			for(int x=ri.x; x<ri.w+ri.x; x++){
				for(int y=ri.y; y<ri.h+r1.y; y++){
					CPPUNIT_ASSERT_EXT(r1.contains(x,y), "\n%d,%d\n", x, y);
					CPPUNIT_ASSERT_EXT(r2.contains(x,y), "\n%d,%d\n", x, y);
				}
			}
			for(int x=r1.x; x<r1.w+r1.x; x++){
				for(int y=r1.y; y<r1.h+r1.y; y++){
					if(!ri.contains(x,y)){
						CPPUNIT_ASSERT_EXT(!r2.contains(x,y), "\n%d,%d\n", x, y);
					}
				}
			}
			for(int x=r2.x; x<r2.w+r2.x; x++){
				for(int y=r2.y; y<r2.h+r2.y; y++){
					if(!ri.contains(x,y)){
						CPPUNIT_ASSERT_EXT(!r1.contains(x,y), "\n%d,%d\n", x, y);
					}
				}
			}
		}catch(...){
			printf("Error at: (%d,%d,%d,%d) and (%d,%d,%d,%d)\n", r1.x, r1.y, r1.w, r1.h, r2.x, r2.y, r2.w, r2.h);
			throw;
		}
	}
}

#ifdef STACK_TRACE_FLAG

//#define intersect_check(...) intersect_check(INVOKE_TRACE, ##__VA_ARGS__)

#endif

void video_test_class::sdl_rect_intersect(){
	sdl_rect ri, r1, r2;
	//printf("%d,%d,%d,%d\n", ri.x, ri.y, ri.w, ri.h);

	//4
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,1,2,2);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,1,1,1);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,1,3,3);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);

	//1
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,-1,2,2);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,-1,1,1);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,-1,4,4);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	//3
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,1,2,2);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,1,1,1);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,1,4,4);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	//2
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,-1,2,2);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,-1,1,1);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,-1,4,4);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	
	//top left
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,1,2,2);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,1,1,1);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(1,1,3,3);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	//top right
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,1,3,3);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	//left bottom
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,-1,3,3);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	//contained
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-1,-1,5,5);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
	
	//no intersection
	ri=r1=sdl_rect(0,0,3,3);
	r2=sdl_rect(-2,-2,1,1);
	ri.intersect(r2);
	intersect_check(ri, r1, r2);
}
