#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <AL/alc.h>

struct Keyboard { bool ka[285] = { false }; };
struct Mouse { bool mcl=false,mcr=false;int mx,my,mw; }; // !!current mouse update not tested. should work though
struct XBox { int xba[6];bool xbb[16] = { false }; };

class Frame
{
public:
	Frame(const char* title,int screen,SDL_WindowFlags fs);
	Frame(const char* title="callidaria-program",int width=1280,int height=720,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	Frame(const char* title,int screen,int width,int height,SDL_WindowFlags fs);

	void clear(float cx,float cy,float cz); 	// clear the window
	void update(); 					// update the window
	void vsync(unsigned int frames); 		// cap frame count to specified value
	void input(bool &running); 			// check for user input
	void vanish(); 					// close program
private:
	void init();
	void setup(const char* title,int x,int y,int width,int height,SDL_WindowFlags fs);
	void get_screen(int screen,SDL_Rect* dim_screen);
public:
	Keyboard kb; Mouse mouse; std::vector<XBox> xb;		// input device
	int w_res, h_res;					// window dimensions
private:
	SDL_Window* m_frame; SDL_GLContext m_context;			// frame members
	ALCdevice* m_alcdev; ALCcontext* m_alccon;			// audio members
	std::vector<SDL_GameController*> m_gc; SDL_Event m_fe;		// additional members
	unsigned int m_pT, m_cT, m_fps, m_tempFPS, m_lO;		// frame related members
};
