#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "frame.h"
#include "renderer2d.h"
#include "button.h"
#include "rendereri.h"
#include "camera2d.h"

int main(int argc, char** argv)
{
	Frame frame = Frame();
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
	r2d.load(); ri.load();
	
	Camera2D cam2d = Camera2D(); cam2d.load(&r2d,&ri);

	std::cout<<"\n\033[36mrunning...\n";
	glm::vec2 tpos=glm::vec2(0);glm::vec2 tscl=glm::vec2(1);float trot=0.0f;
	SDL_Event fe; bool ka[1024] = { false }; int mx, my; bool mc = false;
	while (true) {
		frame.vsync(60);
		if(SDL_PollEvent(&fe)) {
			if (fe.type==SDL_QUIT) break;
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym==SDLK_ESCAPE)
				break;
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = true;
			if (fe.type==SDL_KEYUP&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = false;
			SDL_GetMouseState(&mx,&my);
		}
		frame.clear(0.2f, 0.3f, 0.8f);

		//input test
		if(ka[SDLK_e])trot+=2.0f;else if(ka[SDLK_q])trot-=2.0f;
		if(ka[SDLK_w])tpos.y+=2;else if(ka[SDLK_s])tpos.y-=2;
		if(ka[SDLK_a])tpos.x-=2;else if(ka[SDLK_d])tpos.x+=2;
		if(ka[SDLK_i])tscl.y+=0.01f;else if(ka[SDLK_k])tscl.y-=0.01f;
		if(ka[SDLK_j])tscl.x-=0.01f;else if(ka[SDLK_l])tscl.x+=0.01f;
		
		//gui test
		r2d.prepare();
		r2d.upload_model(r2d.sl.at(0).transform(tpos,tscl,trot));
		r2d.render_sprite(0,1);

		//anim test
		r2d.upload_model(glm::mat4(1.0f));r2d.render_anim(0,1);

		//button test
		b1.render(mx,my,mc); b2.render(mx,my,mc);

		//instance test
		ri.prepare();ri.render(0,12);ri.render(1,12);

		frame.update();
	}
	frame.vanish(); //cool method name
	return 0;
}
