#version 330 core

in vec3 position;
in vec2 texCoords;
in vec3 normals;

out vec4 Position;
out vec2 TexCoords;
out vec4 Normals;
out vec4 light_transpos;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);
uniform mat4 light_trans;
uniform int tex_repeat = 1;

void main()
{
	TexCoords=texCoords*tex_repeat;
	Normals=model*vec4(normals,1.0);
	Position=model*vec4(position,1.0);
	gl_Position=proj*view*Position;
	light_transpos=light_trans*Position;
}
