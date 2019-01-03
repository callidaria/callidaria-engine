#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "renderer2d.h"
#include "rendereri.h"

class Camera2D
{
public:
	Camera2D()
	{
		view2D=glm::lookAt(glm::vec3(0,-0.01f,1),glm::vec3(),
				glm::vec3(0,0,1));
		proj2D=glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.1f,10.0f);
	}
	void load(Renderer2D* r2d, RendererI* ri)
	{
		r2d->s2d.enable();
		r2d->upload_view(view2D);r2d->upload_proj(proj2D);
		ri->sI.enable();
		ri->upload_view(view2D);r2d->upload_proj(proj2D);
	}
private:
	glm::mat4 view2D,proj2D;
};
