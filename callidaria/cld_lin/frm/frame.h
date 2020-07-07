#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <AL/al.h> // ??overinclusion
#include <AL/alc.h>

struct Keyboard { bool ka[1024] = { false }; };
struct Mouse { bool mcl=false;int mx,my; };
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
	// !!standard contructor
	Frame(const char* title, int width, int height, bool fs);

	void clear(float cx, float cy, float cz); 	// clear the window
	void update(); 					// update the window
	void vsync(unsigned int frames); 		// cap frame count to specified value
	void input(bool &running); 			// check for user input
	void vanish(); 					// close program
private:
	SDL_Window* m_frame; SDL_GLContext m_context;		// frame members
	ALCdevice* m_alcdev; ALCcontext* m_alccon;		// audio members
	SDL_GameController* m_gc = NULL; SDL_Event m_fe;	// additional members
	unsigned int m_pT, m_cT, m_fps, m_tempFPS, m_lO;	// frame related members
public:
	Keyboard kb; Mouse mouse; XBox xb;	// input devices
	int w_res, h_res;			// window dimensions
};
