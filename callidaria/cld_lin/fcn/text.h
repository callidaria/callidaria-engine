#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../gfx/shader.h"
#include "../mat/camera2d.h"
#include "font.h"

class Text
{
public:
	Text(Font* in_font)
		: font(in_font)
	{
		glGenVertexArrays(1,&vao);
		glGenBuffers(1,&vbo);glGenBuffers(1,&ibo);
	}
	int add(char c,glm::vec2 p)
	{
		o[ind]=p;
		int i=0;while(i<96) { if(font->id[i]==(int)c) break; i++; }
		x[ind]=font->x[i];y[ind]=font->y[i];
		w[ind]=font->wdt[i];h[ind]=font->hgt[i];
		xo[ind]=font->xo[i];yo[ind]=font->yo[i];
		ind++;
		return font->xa[i]*(font->mw/83.0f);
	}
	void add(const char* s,glm::vec2 p) {
		for (int i=0;i<strlen(s);i++) {
			if (s[i]!=' ') p.x+=add(s[i],p);
			else p.x+=57.0f*(font->mw/83.0f);
		}
	}
	void load_vertex()
	{
		glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(font->v),font->v,
				GL_STATIC_DRAW);
	}
	void load_wcam(Camera2D* c)
	{
		load_vertex();
		sT.compile2d("shader/vertex_text.shader",
				"shader/fragment_text.shader");
		sT.load_text(ibo);
		font->texture();
		sT.upload_float("wdt",font->mw);
		sT.upload_matrix("view",c->view2D);
		sT.upload_matrix("proj",c->proj2D);
	}
	void load_text()
	{
		for(int i=0;i<ind;i++) {
			ibv[i*8]=o[i].x;ibv[i*8+1]=o[i].y;
			ibv[i*8+2]=x[i];ibv[i*8+3]=y[i];
			ibv[i*8+4]=w[i];ibv[i*8+5]=h[i];
			ibv[i*8+6]=xo[i];ibv[i*8+7]=yo[i];
		}
	}
	void prepare()
	{
		sT.enable();glBindVertexArray(vao);
		glDisable(GL_CULL_FACE);glActiveTexture(GL_TEXTURE0);
	}
	void render(int amnt,glm::vec4 col)
	{
		sT.upload_vec4("colour",col);
		glBindTexture(GL_TEXTURE_2D,font->tex);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(ibv),ibv,GL_DYNAMIC_DRAW);
		glDrawArraysInstanced(GL_TRIANGLES,0,6,amnt);
	}
private:
	Shader sT;Font* font;
	unsigned int vao,vbo,ibo;
	int ind=0;glm::vec2 o[1024];int x[1024],y[1024],w[1024],h[1024];
	int xo[1024],yo[1024];
	float ibv[8*1024];
};
