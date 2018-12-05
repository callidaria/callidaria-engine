#include <iostream>
#include "frame.h"
#include "renderer2d.h"
#include "button.h"
#include "rendereri.h"

int main(int argc, char** argv)
{
	Frame frame = Frame();
	Renderer2D r2d = Renderer2D(); RendererI ri = RendererI();

	//TODO : setup

	r2d.load(); ri.load();

	std::cout<<"\n\033[36mrunning...\n";
	SDL_Event fe; bool ka[1024] = { false }; int mx, my; bool mc = false;
	while (true) {
		frame.vsync(60);
		if(SDL_PollEvent(&fe)) {
			if (fe.type==SDL_QUIT) break;
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = true;
			if (fe.type==SDL_KEYUP&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = false;
			SDL_GetMouseState(&mx,&my);
		}
		frame.clear(0.2f, 0.3f, 0.8f);

		//TODO : loopcode

		frame.update();
	}
	frame.vanish();
	return 0;
}
