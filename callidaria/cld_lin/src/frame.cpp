#include "../frm/frame.h"

/*
 *	ADD OUTPUT INFORMATION ABOUT SYSTEM AND WINDOW SETTINGS !!
 *	AND DECORATE ALL ERROR AND SYSTEM OUTPUTS .
 *	ALSO MAYBE IMPLEMENT WITH VIEWPORT RIGHT FROM THE BEGINNING ?
 * */
Frame::Frame() : w_res(800),h_res(600) { Frame("callidaria",800,600,false); }
Frame::Frame(const char* title,int screen,bool fs)
{
	init();

	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	w_res = dim_screen.w;
	h_res = dim_screen.h;
	setup(title,dim_screen.x,dim_screen.y,dim_screen.w,dim_screen.h,fs);
}
Frame::Frame(const char* title,int width,int height,bool fs)
	: w_res(width),h_res(height)
{
	init();
	setup(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,fs);
}
Frame::Frame(const char* title,int screen,int width,int height,bool fs)
	: w_res(width),h_res(height)
{
	init();

	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	setup(title,dim_screen.x+100,dim_screen.y+100,width,height,fs);
}
void Frame::clear(float cx,float cy,float cz)
{
	glClearColor(cx,cy,cz,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // ??maybe outside option to clear without depth buffer
}
void Frame::update() { SDL_GL_SwapWindow(m_frame); }
/*
 *	THIS MIGHT BE EFFICTIVE FOR A VSYNC FUNCTION BUT THIS IS !!!NOT!!! APPROPRIATE FOR DELTA TIME RELIANCE !
 *	MAKE VSYNC OPTIONAL AND BASE TIME RELATED UPDATES AND PHYSICS TO A LEGITIMATE DELTA TIME .
 *	THIS ISN'T THE 90s ANYMORE GODDAMMIT . THE LATER THIS HAPPENS THE MORE CODE HAS TO BE CHANGED WHEN IS DOES
 * */
void Frame::vsync(unsigned int frames)
{
	unsigned int mf = frames; // ??why this paranoid variable save. frames never gets changed
	m_pT = m_cT;
	m_cT = SDL_GetTicks();
	m_tempFPS++;
	if (m_cT-m_lO>=1000) {
		m_lO = m_cT;
		m_fps = m_tempFPS;
		m_tempFPS = 0;
	} // ??can this be joined to a single case differentiation
	if (m_cT-m_pT<1000/mf) SDL_Delay(1000/mf-SDL_GetTicks()+m_pT);
}
void Frame::input(bool &running)
{
	while (SDL_PollEvent(&m_fe)) {
		running = m_fe.type!=SDL_QUIT; // exit the program when closing is requested

		// read keyboard input
		if (m_fe.type==SDL_KEYDOWN&&m_fe.key.keysym.sym<1024) kb.ka[m_fe.key.keysym.sym] = true;
		if (m_fe.type==SDL_KEYUP&&m_fe.key.keysym.sym<1024) kb.ka[m_fe.key.keysym.sym] = false;

		// read mouse input
		SDL_GetMouseState(&mouse.mx,&mouse.my);
		mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT;
		mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT; // ??one or another. not only one at a time please?

		// read controller input
		/*
		 * please! this is pure horror. this code is so damn ugly and big.
		 * ??reduction much ...maybe get values of all controller macros and find relation to loop everything
		 * */
		for (int i=0;i<m_gc.size();i++) {
			xb[i].start = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_START);
			xb[i].back = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_BACK);
			xb[i].a = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_A);
			xb[i].b = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_B);
			xb[i].x = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_X);
			xb[i].y = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_Y);
			xb[i].up = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_DPAD_UP);
			xb[i].down = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			xb[i].left = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			xb[i].right = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
			xb[i].left_sh = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			xb[i].right_sh = SDL_GameControllerGetButton(m_gc.at(i),SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			xb[i].xal = SDL_GameControllerGetAxis(m_gc.at(i),SDL_CONTROLLER_AXIS_LEFTX);
			xb[i].yal = SDL_GameControllerGetAxis(m_gc.at(i),SDL_CONTROLLER_AXIS_LEFTY);
			xb[i].xar = SDL_GameControllerGetAxis(m_gc.at(i),SDL_CONTROLLER_AXIS_RIGHTX);
			xb[i].yar = SDL_GameControllerGetAxis(m_gc.at(i),SDL_CONTROLLER_AXIS_RIGHTY);
			xb[i].left_tr = SDL_GameControllerGetAxis(m_gc.at(i),SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			xb[i].right_tr = SDL_GameControllerGetAxis(m_gc.at(i),SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		}
	}
}
void Frame::vanish()
{
	// ??doing this with an array reference even cleaner
	for (int i=0;i<m_gc.size();i++) SDL_GameControllerClose(m_gc.at(i)); // closing controller reference

	// closing audio context & device
	alcMakeContextCurrent(NULL); // ??doesn't this happen automatically
	alcDestroyContext(m_alccon);
	alcCloseDevice(m_alcdev);

	// closing render context & program
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}
void Frame::init()
{
	// sdl setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3); // ??flexible support for older versions
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
}
void Frame::setup(const char* title,int x,int y,int width,int height,bool fs)
{
	// creating window
	if (fs) m_frame = SDL_CreateWindow(title,x,y,width,height,SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN);
	else m_frame = SDL_CreateWindow(title,x,y,width,height,SDL_WINDOW_OPENGL); // ??looks awful
	m_context = SDL_GL_CreateContext(m_frame);

	// opengl setup
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// openal setup
	m_alcdev = alcOpenDevice(NULL);
	m_alccon = alcCreateContext(m_alcdev,NULL);
	alcMakeContextCurrent(m_alccon); // ??maybe autocontextualized in main thread

	// controller setup
	int gcc = 0;
	while (SDL_IsGameController(gcc)) {
		m_gc.push_back(SDL_GameControllerOpen(gcc));
		gcc++;
	} if (gcc==0) printf("\033[0;34mno controllers plugged in\n"); // !!decorate output

	m_cT = 0; m_fps = 0; m_tempFPS = 0; m_lO = 0; // ??all necessary & syntax
}
void Frame::get_screen(int screen,SDL_Rect* dim_screen)
{
	// ??this would be fatal if false ...maybe close the program immediately afterwards to not cause mem leak
	// right now this falls back to a normalized state that can't fail ...ugly and not recommended
	if (screen<SDL_GetNumVideoDisplays()||SDL_GetDisplayBounds(screen,dim_screen)!=0) { // !!no, i can't watch
		// ??equivalence is unnessessary through boolcast
		printf("\033[1;31mscreen could not be set: %s\n",SDL_GetError()); // !!decoration much
		printf("\033[1;36m\t=> falling back to standard configuration\n");
		dim_screen->x = 0; dim_screen->y = 0; dim_screen->w = 1280; dim_screen->h = 720;
	} // ??laptop falls back into standard config at screen zero ...why the fkk? i have at least one screen????
	else { // !!obfuscate asap
		// !!test this output on fedora system
		printf("\033[1;36mframe resolution is set to: %ix%i\n",dim_screen->w,dim_screen->h);
	}
}
