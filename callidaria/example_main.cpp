#include <iostream>
#include "frame.h"
#include "renderer2d.h"
#include "button.h"
#include "rendereri.h"

int main(int argc, char** argv)
{
	Frame frame = Frame();
	Renderer2D r2d = Renderer2D(); RendererI ri = RendererI();

	//gui example
	r2d.add(glm::vec2(100,100),100,100,"res/testbild.png");

	//button example
	Button b1 = Button(&r2d,glm::vec2(10,680),150,30,"test",false);
	Button b2 = Button(&r2d,glm::vec2(10,630),150,30,"mtest",false);

	//instance example
	ri.add(glm::vec2(500,100),40,40,"res/testbild.png");
	ri.add(glm::vec2(500,100),40,40,"res/testbild.png");
	int index=0;for(float y=0;y<=1;y+=0.2f) {
		for(float x=0;x<=0.2f;x+=0.2f)
			ri.set_offset(0,index++,glm::vec2(x,y));
	}
	index=0;for(float y=0;y<=1;y+=0.2f) {
		for(float x=0.6f;x<=0.8f;x+=0.2f)
			ri.set_offset(1,index++,glm::vec2(x,y));
	}
	r2d.load(); ri.load();

	std::cout<<"\n\033[36mrunning...\n";
	SDL_Event fe; bool ka[1024] = { false }; int mx, my; bool mc = false;
	while (true) {
		frame.vsync(60);
		if(SDL_PollEvent(&fe)) {
			if (fe.type==SDL_QUIT) break;
			if (fe.key.keysym.sym==SDLK_ESCAPE) break; //optional
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = true;
			if (fe.type==SDL_KEYUP&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = false;
			SDL_GetMouseState(&mx,&my);
		}
		frame.clear(0.2f, 0.3f, 0.8f);

		//gui test
		r2d.prepare(); r2d.render_sprite(0,1);

		//button test
		b1.render(mx,my,mc); b2.render(mx,my,mc);

		//instance test
		ri.prepare(); ri.render(0,100); ri.render(1,100);

		frame.update();
	}
	frame.vanish(); //cool method name
	return 0;
}
