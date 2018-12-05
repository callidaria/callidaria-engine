#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

class Anim
{
public:
	Anim(glm::vec2 p, float w, float h, std::vector<const char*> t, unsigned int f)
		: tps(t), fl(f)
	{
		p.x = p.x / 640.0f - 1; p.y = p.y / 360.0f - 2 + 1;
		w /= 640.0f; h /= 360.0f;

		v[0] = p.x; v[1] = p.y+h; v[2] = 0.0f; v[3] = 0.0f;
		v[4] = p.x+w; v[5] = p.y+h; v[6] = 1.0f; v[7] = 0.0f;
		v[8] = p.x+w; v[9] = p.y; v[10] = 1.0f; v[11] = 1.0f;
		v[12] = p.x; v[13] = p.y; v[14] = 0.0; v[15] = 1.0f;

		ft = 0; tn = t.size(); glGenTextures(20, tex);
	}
	void texture()
	{
		for (int i = 0; i < tps.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, tex[i]);
			int width, height;
			unsigned char* image = SOIL_load_image(tps.at(i),
				&width, &height, 0, SOIL_LOAD_RGBA);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			SOIL_free_image_data(image);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	void setup()
	{
		glBindTexture(GL_TEXTURE_2D, tex[ft/(fl/tn)]);
		ft++; if (ft==30) ft=0;
	}
private:
	std::vector<const char*> tps;
public:
	float v[16]; unsigned int tex[20];
	unsigned int tn, fl, ft;
};
