#include "../frm/frame.h"

Frame::Frame(const char* title,int width,int height,bool fs)
	: w_res(width), h_res(height)
{
	// sdl setup
	SDL_Init(SDL_INIT_EVERYTHING); // ??maybe just video
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

	// creating window
	if (fs) m_frame = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,
			SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN); // ??delete the centering flags bc unessessary
	else m_frame = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,
			SDL_WINDOW_OPENGL); // ??windowposition flags irrelevant
	m_context = SDL_GL_CreateContext(m_frame);

	// opengl setup
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST); // ??remove for context based implementation. framebuffers will be mad about this!
	glEnable(GL_CULL_FACE); // ??it is bullshit to autoenable it this soon bcuz 2d only disables it @prepare()

	// audio setup
	m_alcdev = alcOpenDevice(NULL); // !!wat u doin' parameter! wat was i thinking!
	m_alccon = alcCreateContext(m_alcdev,NULL);
	alcMakeContextCurrent(m_alccon); // ??maybe autocontextualized in main thread

	// controller setup
	if (!SDL_IsGameController(0)) printf("no controllers found\n"); // !!decorate output
	else m_gc = SDL_GameControllerOpen(0); // !!multiple controller support

	m_cT = 0; m_fps = 0; m_tempFPS = 0; m_lO = 0; // ??all necessary & syntax
}
void Frame::clear(float cx,float cy,float cz)
{
	glClearColor(cx,cy,cz,1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // ??maybe outside option to clear without depth buffer
}
void Frame::update() { SDL_GL_SwapWindow(m_frame); }
/*
 *	THIS MIGHT BE EFFICTIVE FOR A VSYNC FUNCTION BUT THIS IS !!!NOT!!! APPROPRIATE FOR A DELTA TIME RELIANCE !
 *	MAKE VSYNC OPTIONAL AND BASE TIME RELATED UPDATES AND PHYSICS TO A LEGITIMATE DELTA TIME .
 *	THIS ISN'T THE 90s ANYMORE GODDAMMIT. THE LATER THIS HAPPENS THE MORE CODE HAS TO BE CHANGED WHEN IS DOES
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
		if (m_fe.type==SDL_QUIT) running = false; // exit the program when closing is requested

		// read keyboard input
		if (m_fe.type==SDL_KEYDOWN&&m_fe.key.keysym.sym<1024) kb.ka[m_fe.key.keysym.sym] = true;
		if (m_fe.type==SDL_KEYUP&&m_fe.key.keysym.sym<1024) kb.ka[m_fe.key.keysym.sym] = false;

		// read mouse input
		SDL_GetMouseState(&mouse.mx,&mouse.my);
		if (m_fe.button.button==SDL_BUTTON_LEFT) mouse.mcl = true; else mouse.mcl = false; // !!equal to func
		// !!add all mouse functions please. even the amiga mouse has more buttons

		// read controller input
		/*
		 * please! this is pure horror. this code is so damn ugly and big.
		 * ??reduction much ...maybe get values of all controller macros and find relation to loop everything
		 * */
		if (m_gc!=NULL) {
			xb.start = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_START);
			xb.back = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_BACK);
			xb.a = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_A);
			xb.b = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_B);
			xb.x = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_X);
			xb.y = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_Y);
			xb.up = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_DPAD_UP);
			xb.down = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_DPAD_DOWN);
			xb.left = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_DPAD_LEFT);
			xb.right = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
			xb.left_sh = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
			xb.right_sh = SDL_GameControllerGetButton(m_gc,SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			xb.xal = SDL_GameControllerGetAxis(m_gc,SDL_CONTROLLER_AXIS_LEFTX);
			xb.yal = SDL_GameControllerGetAxis(m_gc,SDL_CONTROLLER_AXIS_LEFTY);
			xb.xar = SDL_GameControllerGetAxis(m_gc,SDL_CONTROLLER_AXIS_RIGHTX);
			xb.yar = SDL_GameControllerGetAxis(m_gc,SDL_CONTROLLER_AXIS_RIGHTY);
			xb.left_tr = SDL_GameControllerGetAxis(m_gc,SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			xb.right_tr = SDL_GameControllerGetAxis(m_gc,SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
		}
	}
}
void Frame::vanish()
{
	SDL_GameControllerClose(m_gc); // closing controller reference

	// closing audio context & device
	alcMakeContextCurrent(NULL); // ??doesn't this happen automatically
	alcDestroyContext(m_alccon);
	alcCloseDevice(m_alcdev);

	// closing render context & program
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}
