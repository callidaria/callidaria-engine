#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../gfx/shader.h"
#include "../mat/camera2d.h"
#include "font.h"

#define MAX_CHAR_COUNT 1024

class Text
{
public:
	Text(Font* f);

	int add(char c,glm::vec2 p);
	void add(const char* s,glm::vec2 p);
	void load_vertex();
	void load_wcam(Camera2D* c);
	void load_text();
	void prepare();
	void render(int amnt,glm::vec4 col);
private:
	Shader sT;Font* font; // !!support more of the .fnt language in the future
	unsigned int vao,vbo,ibo;
	int ind=0;glm::vec2 o[MAX_CHAR_COUNT];
	int x[MAX_CHAR_COUNT],y[MAX_CHAR_COUNT];
	int w[MAX_CHAR_COUNT],h[MAX_CHAR_COUNT];
	int xo[MAX_CHAR_COUNT],yo[MAX_CHAR_COUNT];
	float ibv[8*MAX_CHAR_COUNT];
};
