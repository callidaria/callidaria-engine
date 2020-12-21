#include "../gfx/renderer3d.h"

Renderer3D::Renderer3D()
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
}
void Renderer3D::add(const char* m,const char* t,const char* sm,const char* nm,const char* em,glm::vec3 p,float s,
		glm::vec3 r)
{
	Mesh proc = Mesh(m,t,sm,nm,em,p,s,r,&mofs);
	ml.push_back(proc);
}
void Renderer3D::load_vertex()
{
	int lm=ml.size();unsigned int vs=0;int io=0;
	for (int h=0;h<lm;h++) vs+=ml.at(h).v.size();
	/*float v[vs];*/std::vector<float> v; // ??any loadtime diff. with array instead of vector
	for (int j=0;j<lm;j++) {
		for (int i=0;i<ml.at(j).v.size();i++)
			//v[io+i]=ml.at(j).v[i];
			v.push_back(ml.at(j).v[i]);
		io+=ml.at(j).v.size();
	} glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);
	//glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER,v.size()*sizeof(float),v.data(),GL_STATIC_DRAW);
}
void Renderer3D::load_texture()
{
	for (int i=0;i<ml.size();i++) ml.at(i).texture();
	s3d.upload_int("tex",0);s3d.upload_int("sm",1);s3d.upload_int("emit",2);
	s3d.upload_int("shadow_map",3);s3d.upload_int("nmap",4);
}
void Renderer3D::load(Camera3D* c)
{
	load_vertex();
	shs.compile3d("shader/fbv_shadow.shader","shader/fbf_shadow.shader");
	s3d.compile3d("shader/vertex3d.shader","shader/fragment3d.shader");
	load_texture();
	upload_view(c->view3D);upload_proj(c->proj3D);
}
void Renderer3D::prepare()
{
	s3d.enable();
	glBindVertexArray(vao); // ??callings are to compare
	glActiveTexture(GL_TEXTURE0);
}
void Renderer3D::prepare(Camera3D* c)
{
	prepare();
	c->update();
	upload_view(c->view3D);upload_proj(c->proj3D);
	s3d.upload_vec3("view_pos",c->pos); // !!clearup the shader
}
void Renderer3D::render_mesh(int b,int e) // !!cleanup the forloop
{
	for (int i=b;i<e;i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).specmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).emitmap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).normap);
		glDrawArrays(GL_TRIANGLES,ml.at(i).ofs,ml.at(i).size);
	}
}
void Renderer3D::upload_model(glm::mat4 m) { s3d.upload_matrix("model",m); }
void Renderer3D::upload_view(glm::mat4 m) { s3d.upload_matrix("view",m); }
void Renderer3D::upload_proj(glm::mat4 m) { s3d.upload_matrix("proj",m); }
void Renderer3D::upload_shadow(glm::mat4 m) { s3d.upload_matrix("light_trans",m); }
