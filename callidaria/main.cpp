#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "engine/frame.h"
#include "engine/renderer2d.h"
#include "engine/button.h"
#include "engine/rendereri.h"
#include "engine/camera2d.h"

int main(int argc, char** argv)
{
	Frame frame = Frame();
	Renderer2D r2d = Renderer2D(); RendererI ri = RendererI();

	//TODO: setup

	//camera and upload
	Camera2D cam2d = Camera2D();
	r2d.load_wcam(&cam2d); ri.load_wcam(&cam2d);

	std::cout<<"\n\033[36mrunning...\n";
	SDL_Event fe; bool ka[1024] = { false }; int mx, my; bool mc = false;
	bool run=true; while (run) {
		frame.vsync(60);
		while(SDL_PollEvent(&fe)) {
			if (fe.type==SDL_QUIT) run=false;
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym==SDLK_ESCAPE)
				run=false;
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = true;
			if (fe.type==SDL_KEYUP&&fe.key.keysym.sym<1024)
				ka[fe.key.keysym.sym] = false;
			SDL_GetMouseState(&mx,&my);
		}
		frame.clear(0.2f, 0.3f, 0.8f);

		//TODO: loopcode

		frame.update();
	}
	frame.vanish();
	return 0;
}
