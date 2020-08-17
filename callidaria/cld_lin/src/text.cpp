#include "../fcn/text.h"

Text::Text(Font* f)
	: font(f)
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	glGenBuffers(1,&ibo);
}
int Text::add(char c,glm::vec2 p) // !!passing x increment like this is very bad pracice with public method
{
	// position && sprite sheet identification
	o[ind] = p;
	int i = 0;
	while (i<96) { // ??maybe alternate iteration until correct index that is more performant
		if (font->id[i]==(int)c) break;
		i++;
	}
	
	// character information write
	x[ind] = font->x[i]; // ??less reserved arrays with bitshifted dataclusters
	y[ind] = font->y[i];
	w[ind] = font->wdt[i];
	h[ind] = font->hgt[i];
	xo[ind] = font->xo[i];
	yo[ind] = font->yo[i];
	ind++;

	return font->xa[i]*(font->mw/83.0f); // ??do this with a vec2 pointer maybe
}
void Text::add(const char* s,glm::vec2 p) // this function is actually good ...i'm surprised
{
	for (int i=0;i<strlen(s);i++) {
		if (s[i]!=' ') p.x += add(s[i],p);
		else p.x += 57.0f*(font->mw/83.0f);
	}
}
void Text::load_vertex() // !!no need to have this extra public vertex load function
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(font->v),font->v,GL_STATIC_DRAW);
}
void Text::load_wcam(Camera2D* c)
{
	load_vertex();
	sT.compile2d("shader/vertex_text.shader","shader/fragment_text.shader");
	sT.load_text(ibo); // shader upcoming .revmtd
	font->texture();
	sT.upload_float("wdt",font->mw);
	sT.upload_matrix("view",c->view2D); // !!please use a presetted camera matrix with static viewport for text
	sT.upload_matrix("proj",c->proj2D);
}
void Text::load_text() // ??another seperate loading method
{
	for (int i=0;i<ind;i++) { // ??why not natively write to this array. will fix all problems ?!?!?!
		ibv[i*8] = o[i].x;
		ibv[i*8+1] = o[i].y;
		ibv[i*8+2] = x[i];
		ibv[i*8+3] = y[i];
		ibv[i*8+4] = w[i];
		ibv[i*8+5] = h[i];
		ibv[i*8+6] = xo[i];
		ibv[i*8+7] = yo[i];
	} // big chunky useless bullshit. please clean !
}
void Text::prepare()
{
	sT.enable();
	glBindVertexArray(vao);
	glDisable(GL_CULL_FACE); // !!define standard flagging by majority usage
	glActiveTexture(GL_TEXTURE0);
}
void Text::render(int amnt,glm::vec4 col)
{
	sT.upload_vec4("colour",col); // ??shader uploads outside of prepare function
	glBindTexture(GL_TEXTURE_2D,font->tex);
	glBindBuffer(GL_ARRAY_BUFFER,ibo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(ibv),ibv,GL_DYNAMIC_DRAW); // !!buffer data call in main loop ´°___°`
	glDrawArraysInstanced(GL_TRIANGLES,0,6,amnt);
}
