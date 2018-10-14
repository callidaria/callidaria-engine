#include <iostream>
#include "frame.h"
#include "renderer.h"
#include "button.h"

int main(int argc, char** argv)
{
	Frame frame = Frame(); Renderer r = Renderer();

	//TODO: still no code here? ...write something, i'm bored.

	SDL_Event fe; bool ka[1024] = { false }; int mx, my; bool mc = false;
	while (true) {
		frame.vsync(60);
		if(SDL_PollEvent(&fe)) {
			if (fe.type==SDL_QUIT) break;
			if (fe.key.keysym.sym==SDLK_ESCAPE) break; //optional
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym<1024) ka[fe.key.keysym.sym] = true;
			if (fe.type==SDL_KEYUP&&fe.key.keysym.sym<1024) ka[fe.key.keysym.sym] = false;
			SDL_GetMouseState(&mx,&my);
		}
		frame.clear(0.2f, 0.3f, 0.8f);

		//TODO: give me something to do!

		frame.update();
	}
	frame.vanish(); //cool method name
}
