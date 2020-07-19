#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <AL/alc.h>

#define MAX_CONTROLLER_COUNT 4 // ??make it dynamic. not that hard really!

/*
 *	CANT TEST THE MULTIPLE CONTROLLER FEATURE BECAUSE I ONLY HAVE ONE CONTROLLER
 *	NICE THINKING STUPID : SHOULD HAVE OCCURED TO YOU BEFORE YOU'VE IMPLEMENTED THE FEATURE
 *	DON'T USE THIS FEATURE UNTIL I WENT AND BOUGHT A SECOND CONTROLLER !
 * */
struct Keyboard { bool ka[1024] = { false }; };
struct Mouse { bool mcl=false,mcr=false;int mx,my,mw; };
struct XBox {
	bool start=false,back=false;
	bool a=false,b=false,x=false,y=false;
	bool up=false,down=false,left=false,right=false;
	bool left_sh=false,right_sh=false;
	int left_tr=0,right_tr=0;
	int xal=0,yal=0;int xar=0,yar=0;
};

class Frame
{
public:
	Frame(const char* title,int screen,bool fs);
	Frame(const char* title,int width,int height,bool fs); // used by standard constructor
	Frame(const char* title,int screen,int width,int height,bool fs);

	void clear(float cx,float cy,float cz); 	// clear the window
	void update(); 					// update the window
	void vsync(unsigned int frames); 		// cap frame count to specified value
	void input(bool &running); 			// check for user input
	void vanish(); 					// close program
private:
	void init();
	void setup(const char* title,int x,int y,int width,int height,bool fs);
	void get_screen(int screen,SDL_Rect* dim_screen);
public:
	Keyboard kb; Mouse mouse; XBox xb[MAX_CONTROLLER_COUNT];	// input device
	int w_res, h_res;						// window dimensions
private:
	SDL_Window* m_frame; SDL_GLContext m_context;			// frame members
	ALCdevice* m_alcdev; ALCcontext* m_alccon;			// audio members
	std::vector<SDL_GameController*> m_gc; SDL_Event m_fe;		// additional members
	unsigned int m_pT, m_cT, m_fps, m_tempFPS, m_lO;		// frame related members
};
