#include <iostream>
#include "cld_lin/frm/frame.h"
#include "cld_lin/gfx/renderer2d.h"
#include "cld_lin/fcn/button.h"
#include "cld_lin/gfx/rendereri.h"
#include "cld_lin/mat/camera2d.h"

int main(int argc, char** argv)
{
	Frame f = Frame();
	Renderer2D r2d = Renderer2D(); RendererI ri = RendererI();

	//gui example
	r2d.add(glm::vec2(100,100),200,100,"res/testbild.png");

	//anim example
	std::vector<const char*> ts;
	ts.push_back("res/testbild.png");ts.push_back("res/bwtestbild.jpg");
	r2d.add(glm::vec2(150,300),100,100,ts,30);

	//button example
	Button b1 = Button(&r2d,glm::vec2(10,680),150,30,"test",false);
	Button b2 = Button(&r2d,glm::vec2(10,630),150,30,"mtest",false);

	//instance example
	ri.add(glm::vec2(500,100),40,40,"res/testbild.png");
	ri.add(glm::vec2(500,100),40,40,"res/bwtestbild.jpg");
	int index=0;for(float y=0;y<=500;y+=50) {
		for(float x=0;x<=50;x+=50)
			ri.set_offset(0,index++,glm::vec2(x,y));
	}
	index=0;for(float y=0;y<=500;y+=50) {
		for(float x=150;x<=200;x+=50)
			ri.set_offset(1,index++,glm::vec2(x,y));
	}

	//camera and upload
	Camera2D cam2d = Camera2D();
	r2d.load_wcam(&cam2d); ri.load_wcam(&cam2d);

	std::cout<<"\n\033[36mrunning...\n";
	glm::vec2 tpos=glm::vec2(0);glm::vec2 tscl=glm::vec2(1);float trot=0.0f;
	bool run=true; while (run) {
		f.vsync(60); f.input(run);
		f.clear(0.2f, 0.3f, 0.8f);

		//input test
		if(f.kb.ka[SDLK_e])trot-=2.0f;else if(f.kb.ka[SDLK_q])trot+=2.0f;
		if(f.kb.ka[SDLK_w])tpos.y+=2;else if(f.kb.ka[SDLK_s])tpos.y-=2;
		if(f.kb.ka[SDLK_a])tpos.x-=2;else if(f.kb.ka[SDLK_d])tpos.x+=2;
		if(f.kb.ka[SDLK_i])tscl.y+=0.01f;
		else if(f.kb.ka[SDLK_k])tscl.y-=0.01f;
		if(f.kb.ka[SDLK_j])tscl.x-=0.01f;
		else if(f.kb.ka[SDLK_l])tscl.x+=0.01f;
		
		//gui test
		r2d.prepare();
		r2d.upload_model(r2d.sl.at(0).transform(tpos,tscl,trot));
		r2d.render_sprite(0,1);

		//anim test
		r2d.upload_model(glm::mat4(1.0f));
		r2d.render_anim(0,1);

		//button test
		b1.render(f.m.mx,f.m.my,f.m.mcl);b2.render(f.m.mx,f.m.my,f.m.mcl);

		//instance test
		ri.prepare();ri.render(0,12);ri.render(1,12);

		f.update();
	}
	f.vanish();
	return 0;
}
