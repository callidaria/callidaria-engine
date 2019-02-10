#include <iostream>
#include "cld_lin/frm/frame.h"
#include "cld_lin/gfx/renderer2d.h"
#include "cld_lin/fcn/button.h"
#include "cld_lin/gfx/rendereri.h"
#include "cld_lin/mat/camera2d.h"

int main(int argc, char** argv)
{
	Frame f = Frame();
	Renderer2D r2d = Renderer2D(); RendererI ri = RendererI();

	//TODO: setup

	Camera2D cam2d = Camera2D();
	r2d.load_wcam(&cam2d); ri.load_wcam(&cam2d);

	std::cout<<"\n\033[36mrunning...\n";
	bool run=true; while (run) {
		f.vsync(60); f.input(run);
		f.clear(0.2f, 0.3f, 0.8f);

		//TODO: loopcode

		f.update();
	}
	f.vanish();
	return 0;
}
