#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "sprite.h"
#include "anim.h"
#include "../mat/camera2d.h"

class Renderer2D
{
public:
	// SETUP
	Renderer2D();
	int add(glm::vec2 p,float w,float h,const char* t);
	int add(glm::vec2 p,float w,float h,const char* t,unsigned int r,unsigned int c,unsigned int f,
			unsigned int itn);
	void load_vertex();
	void load_texture();
	void load();
	void load(Camera2D* c);

	// UPDATE
	void prepare();
	void render_sprite(int b, int e);
	void render_state(int s, glm::vec2 i);
	void render_anim(int i);
	void reset_shader();

	// GETTERS
	int get_max_sprite();
	int get_max_anim();

	// SHADER UPLOAD
	void upload_model(glm::mat4 m);
	void upload_view(glm::mat4 m);
	void upload_proj(glm::mat4 m);
	void upload_row(int i);
	void upload_col(int i);
	void upload_tindex(glm::vec2 v);
private:
	unsigned int vao,vbo,ebo;
	Shader s2d;
	std::vector<Sprite> sl;
	std::vector<Anim> al;
};
