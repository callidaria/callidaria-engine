#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Frame
{
public:
	Frame()
	{
		cT = 0; fps = 0; temp_fps = 0; lO = 0;
		SDL_Init(SDL_INIT_EVERYTHING);
		frame = SDL_CreateWindow("scvsjc", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
		context = SDL_GL_CreateContext(frame);
		glewInit();
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}
	void clear(float cx, float cy, float cz)
	{
		glClearColor(cx, cy, cz, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void update()
	{
		SDL_GL_SwapWindow(frame);
	}
	void vsync(unsigned int frames)
	{
		unsigned int mf = frames;
		pT = cT; cT = SDL_GetTicks(); temp_fps++;
		if (cT-lO>=1000) { lO = cT; fps = temp_fps; temp_fps = 0; }
		if (cT-pT<1000/mf) { SDL_Delay(1000 / mf - SDL_GetTicks() + pT); }
	}
	void vanish()
	{
		SDL_GL_DeleteContext(context);
		SDL_Quit();
	}
private:
	unsigned int vao;
	SDL_Window* frame; SDL_GLContext context;
	unsigned int pT, cT, fps, temp_fps, lO;
};
