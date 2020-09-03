#pragma once

#include <iostream>
#include "../frm/frame.h"
#include "font.h"
#include "text.h"

class TextField
{
public:
	TextField(Font* fnt);
	void render(Frame* frame);
private:
	Text txt;
};
