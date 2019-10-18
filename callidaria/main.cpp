#include <iostream>
#include "cld_lin/frm/frame.h"

int main(int argc, char** argv)
{
	Frame f = Frame();

	//TODO: setup

	Camera2D cam2d = Camera2D();

	bool run=true; while (run) {
		f.vsync(60); f.input(run);
		f.clear(.1f, .1f, .1f);

		//TODO: loopcode

		f.update();
	}
	f.vanish();
	return 0;
}
