#include "../gfx/renderer2d.h"

Renderer2D::Renderer2D()
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	glGenBuffers(1,&ebo);
}
int Renderer2D::add(glm::vec2 p,float w,float h,const char* t)
{
	Sprite proc = Sprite(p,w,h,t);
	sl.push_back(proc);
	return sl.size()-1; // less CPU expensive without sub ??TIME over SPACE
}
int Renderer2D::add(glm::vec2 p,float w,float h,const char* t,unsigned int r,unsigned int c,unsigned int f,
		unsigned int itn)
{
	Anim proc = Anim(p,w,h,t,r,c,f,itn);
	al.push_back(proc);
	return al.size()-1;
}
void Renderer2D::load_vertex() // ??make private
{
	int ls = sl.size(); int la = al.size();
	float v[(ls+la)*16]; unsigned int e[(ls+la)*6];
	for (int j = 0; j < ls+la; j++) {
		for (int i = 0; i < 16; i++) {
			if (j<ls) v[j*16+i] = sl.at(j).v[i];
			else v[j*16+i] = al.at(j-ls).v[i];
		} e[j*6] = j*4;e[j*6+1] = j*4+1;e[j*6+2] = j*4+2;e[j*6+3] = j*4+2;e[j*6+4] = j*4+3;e[j*6+5] = j*4+0;
	} glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(e),e,GL_STATIC_DRAW);
}
void Renderer2D::load_texture() // ??make private
{
	for (int i=0;i<sl.size();i++) sl.at(i).texture();
	for (int i=0;i<al.size();i++) al.at(i).texture();
}
void Renderer2D::load()
{
	load_vertex();
	s2d.compile2d("shader/vertex2d.shader","shader/fragment2d.shader");
	load_texture();
}
void Renderer2D::load(Camera2D* c)
{
	load();
	upload_view(c->view2D);
	upload_proj(c->proj2D);
}
void Renderer2D::prepare()
{
	s2d.enable();
	reset_shader(); // reset license?? what for??? -Gex
	glBindVertexArray(vao);
	// ??which disable is nessessary in prepare
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0); // ??make unnessessary by resetting after usage in extern class
}
void Renderer2D::render_sprite(int b,int e) // ??maybe solve with external loop and direct adression
{
	for (int i=b;i<e;i++) {
		glBindTexture(GL_TEXTURE_2D,sl.at(i).tex);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(int)));
	}
}
void Renderer2D::render_state(int s,glm::vec2 i) // !!NAMING BOYZZ; THIS IS A CRIME!!!
{
	glBindTexture(GL_TEXTURE_2D,al.at(s).tex);
	upload_row(al.at(s).r);upload_col(al.at(s).c);upload_tindex(i);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((s+sl.size())*6*sizeof(int)));
}
void Renderer2D::render_anim(int i)
{
	int index = al.at(i).setup();
	int r = al.at(i).r;int c = al.at(i).c; // !!kill r definition. it is not needed
	glm::vec2 ind = glm::vec2((int)(index%c),(int)(index/c));
	upload_row(r);upload_col(c);upload_tindex(ind);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sl.size())*6*sizeof(int)));
}
void Renderer2D::reset_shader() { upload_row(1);upload_col(1);upload_tindex(glm::vec2(0,0)); } // ??nessessary
int Renderer2D::get_max_sprite() { return sl.size(); } // ??where used
int Renderer2D::get_max_anim() { return al.size(); }
void Renderer2D::upload_model(glm::mat4 m) { s2d.upload_matrix("model",m); }
void Renderer2D::upload_view(glm::mat4 m) { s2d.upload_matrix("view",m); }
void Renderer2D::upload_proj(glm::mat4 m) { s2d.upload_matrix("proj",m); }
void Renderer2D::upload_row(int i) { s2d.upload_int("row",i); } // ??make private from here on
void Renderer2D::upload_col(int i) { s2d.upload_int("col",i); }
void Renderer2D::upload_tindex(glm::vec2 v) { s2d.upload_vec2("i_tex",v); }
