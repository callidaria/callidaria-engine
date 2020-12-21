#pragma once

#include <iostream>
#include "shader.h"
#include "mesh.h"
#include "../mat/camera3d.h"

class Renderer3D
{
public:
	// SETUP
	Renderer3D();
	void add(const char* m,const char* t,const char* sm,const char* nm,const char* em,glm::vec3 p,float s,
			glm::vec3 r);
	void load_vertex();
	void load_texture();
	void load(Camera3D* c);

	// UPDATE
	void prepare();
	void prepare(Camera3D* c);
	void render_mesh(int b,int e);

	// SHADER UPLOAD
	void upload_model(glm::mat4 m);
	void upload_view(glm::mat4 m);
	void upload_proj(glm::mat4 m);
	void upload_shadow(glm::mat4 m);
private:
	unsigned int vao,vbo;
	unsigned int mofs = 0;
public:
	Shader s3d,shs;
	std::vector<Mesh> ml;
};
