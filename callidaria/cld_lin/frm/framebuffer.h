#pragma once

#include <iostream>
#include "../gfx/shader.h"

class FrameBuffer
{
public:
	FrameBuffer() {  }
	FrameBuffer(const char* vsp, const char* fsp)
	{
		float verts[] = {
			-1.0f,1.0f,0.0f,1.0f,
			-1.0f,-1.0f,0.0f,0.0f,
			1.0f,-1.0f,1.0f,0.0f,
			-1.0f,1.0f,0.0f,1.0f,
			1.0f,-1.0f,1.0f,0.0f,
			1.0f,1.0f,1.0f,1.0f
		};
		glGenVertexArrays(1,&vao); glGenBuffers(1,&vbo);
		glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(verts),&verts,GL_STATIC_DRAW);
		sfb.compile(vsp,fsp);
		glGenFramebuffers(1,&fbo); glBindFramebuffer(GL_FRAMEBUFFER,fbo);
		glGenTextures(1,&tex); glBindTexture(GL_TEXTURE_2D,tex);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1280,720,0,GL_RGB,
				GL_UNSIGNED_BYTE,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,tex,0);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	void bind() { glBindFramebuffer(GL_FRAMEBUFFER,fbo); }
	void close() { glBindFramebuffer(GL_FRAMEBUFFER,0); }
	void render()
	{
		sfb.enable(); glBindVertexArray(vao);
		glBindTexture(GL_TEXTURE_2D,tex); glDrawArrays(GL_TRIANGLES,0,6);
	}
	unsigned int get_tex() { return tex; }
private:
	Shader sfb;
	unsigned int tex;
	unsigned int vao,vbo,fbo;
};
