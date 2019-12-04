#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Camera3D
{
public:
	Camera3D(glm::vec3 cp)
		: pos(cp)
	{
		front=glm::vec3(0.0f,0.0f,-1.0f);up=glm::vec3(0.0f,1.0f,0.0f);
		view3D=glm::lookAt(pos,pos+front,up);
		proj3D=glm::perspective(glm::radians(90.0f),1280.0f/720.0f,0.1f,
				10000.0f);
	}
	void update()
	{
		view3D=glm::lookAt(pos,pos+front,up);
	}
public:
	glm::vec3 pos,front,up;
	glm::mat4 view3D,proj3D;
};