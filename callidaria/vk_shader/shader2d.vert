#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 colour;

layout(location = 0) out vec3 Colour;

void main()
{
	gl_Position = vec4(position,0.0,1.0);
	Colour = colour;
}
