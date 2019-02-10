#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Frame
{
public:
	struct Keyboard { bool ka[1024] = { false }; };
	struct Mouse { bool mcl=false;int mx,my; };
	struct XBox {
		bool start,back;
		bool a,b,x,y;
		bool up,down,left,right;
		bool left_sh,right_sh;
		int left_tr,right_tr;
		int xal,yal;int xar,yar;
	};
public:
	Frame()
	{
		std::cout<<"\033[1;33mstarting callidaria...\n\n";
		std::cout<<"\033[1;4;37;40mstarting OpenGL setup\033[0m\n\n";
		cT = 0; fps = 0; temp_fps = 0; lO = 0;
		SDL_Init(SDL_INIT_EVERYTHING);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
				SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

		frame = SDL_CreateWindow("callidaria", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
		context = SDL_GL_CreateContext(frame);
		glewInit();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (!SDL_IsGameController(0))
			std::cout << "no controllers found\n";
		else gc = SDL_GameControllerOpen(0);
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
		if (cT-pT<1000/mf) { SDL_Delay(1000/mf-SDL_GetTicks()+pT); }
	}
	void input(bool &run)
	{
		while (SDL_PollEvent(&fe)) {
			if (fe.type==SDL_QUIT) run=false;
			if (fe.type==SDL_KEYDOWN&&fe.key.keysym.sym<1024)
				kb.ka[fe.key.keysym.sym] = true;
			if (fe.type==SDL_KEYUP&&fe.key.keysym.sym<1024)
				kb.ka[fe.key.keysym.sym] = false;

			SDL_GetMouseState(&m.mx,&m.my);

			if (gc!=NULL) {
				xb.start=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_START);
				xb.back=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_BACK);
				xb.a=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_A);
				xb.b=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_B);
				xb.x=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_X);
				xb.y=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_Y);
				xb.up=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_DPAD_UP);
				xb.down=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_DPAD_DOWN);
				xb.left=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_DPAD_LEFT);
				xb.right=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
				xb.left_sh=SDL_GameControllerGetButton(gc,						SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
				xb.right_sh=SDL_GameControllerGetButton(gc,
					SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
				xb.xal=SDL_GameControllerGetAxis(gc,
					SDL_CONTROLLER_AXIS_LEFTX);
				xb.yal=SDL_GameControllerGetAxis(gc,
					SDL_CONTROLLER_AXIS_LEFTY);
				xb.xar=SDL_GameControllerGetAxis(gc,
					SDL_CONTROLLER_AXIS_RIGHTX);
				xb.yar=SDL_GameControllerGetAxis(gc,
					SDL_CONTROLLER_AXIS_RIGHTY);
				xb.left_tr=SDL_GameControllerGetAxis(gc,
					SDL_CONTROLLER_AXIS_TRIGGERLEFT);
				xb.right_tr=SDL_GameControllerGetAxis(gc,
					SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
			}
		}
	}
	void vanish()
	{
		std::cout<<"\n\033[1;33mclosing callidaria...\n";
		SDL_GameControllerClose(gc);
		SDL_GL_DeleteContext(context); SDL_Quit();
		std::cout<<"\033[1;35mgoodbye :(\033[0m\n";
	}
private:
	SDL_Window* frame; SDL_GLContext context;
	SDL_GameController* gc = NULL; SDL_Event fe;
	unsigned int pT, cT, fps, temp_fps, lO;
public:
	Keyboard kb; Mouse m; XBox xb;
};
