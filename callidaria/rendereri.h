#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaderi.h"
#include "instance.h"

class RendererI
{
public:
	RendererI()
	{
		std::cout<<"\033[34mgenerating instance buffers\n";
		glGenVertexArrays(1,&vao);
		glGenBuffers(1,&vbo); glGenBuffers(1,&ibo);
	}
	void add(glm::vec2 p, float w, float h, const char* t)
	{ Instance proc = Instance(p,w,h,t); il.push_back(proc); }
	void load_vertex()
	{
		std::cout<<"\n\033[40;37;1;4mloading inst renderer\033[0m\n\n";
		std::cout<<"\033[34mloading instanced vertices\n";
		int li = il.size(); float v[li*24];
		for (int j = 0; j < li; j++) {
			for (int i = 0; i < 24; i++) v[j*24+i] = il.at(j).v[i];
		}
		std::cout<<"uploading instance vertices\n";
		glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
	}
	void load_texture()
	{
		std::cout<<"\033[34mtexturing instanced objects\n";
		for (int i = 0; i < il.size(); i++) il.at(i).texture();
	}
	void load() {load_vertex();sI.compile(ibo);load_texture();}
	void prepare() { sI.enable(); glBindVertexArray(vao); }
	void render(int i, int amt)
	{
		glBindTexture(GL_TEXTURE_2D, il.at(i).tex);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)*100,
				il.at(i).o,GL_DYNAMIC_DRAW);
		glDrawArraysInstanced(GL_TRIANGLES, i*6, i*6+6, amt);
	}
	void set_offset(int i, int j, glm::vec2 o) { il.at(i).o[j] = o; }
private:
	unsigned int vao, vbo, ibo;
	ShaderI sI;
	std::vector<Instance> il;
};
