#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

class Font
{
public:
	Font(const char* ff,const char* tf,float w,float h)
		: tp(tf),mw(w)
	{
		FILE* file=fopen(ff,"r");
		if(file==NULL) { printf("couldn't find font %s\n",ff); }
		int offset=0;
		while(true) {
			char lh[128];int res=fscanf(file,"%s",lh);
			if(res==EOF) break;
			else {
				if(strcmp(lh,"kernings")==0) break;
				else if(strcmp(lh,"char")==0) {
					int mid,mx,my,mw,mh,mxo,myo,mxa,mp,mc;
fscanf(file," id=%d\t\tx=%d\ty=%d\twidth=%d\theight=%d\txoffset=%d\tyoffset=%d\txadvance=%d\tpage=%i\tchnl=%d\n",
		&mid,&mx,&my,&mw,&mh,&mxo,&myo,&mxa,&mp,&mc);
					id[offset]=mid;
					x[offset]=mx;y[offset]=my;
					wdt[offset]=mw;hgt[offset]=mh;
					xo[offset]=mxo;yo[offset]=myo;
					xa[offset]=mxa;offset++;
				}
			}
		}
		v[0] = w; v[1] = 0; v[2] = 1.0f; v[3] = 0.0f;
		v[4] = 0; v[5] = -h; v[6] = 0.0f; v[7] = 1.0f;
		v[8] = w; v[9] = -h; v[10] = 1.0f; v[11] = 1.0f;
		v[12] = w; v[13] = 0; v[14] = 1.0f; v[15] = 0.0f;
		v[16] = 0; v[17] = -h; v[18] = 0.0; v[19] = 1.0f;
		v[20] = 0; v[21] = 0; v[22] = 0.0f; v[23] = 0.0f;
		glGenTextures(1,&tex);
	}
	void texture()
	{
		glBindTexture(GL_TEXTURE_2D,tex);int width,height;
		unsigned char* image=SOIL_load_image(tp,&width,&height,0,
				SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
				GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void setup() { glBindTexture(GL_TEXTURE_2D,tex); }
public:
	float v[24],mw;
	int id[96],x[96],y[96],wdt[96],hgt[96],xo[96],yo[96],xa[96];
	const char* tp;
	unsigned int tex;
};
