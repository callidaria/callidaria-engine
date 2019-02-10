#version 330 core

in vec2 position;
in vec2 texCoords;

out vec4 Position;
out vec2 TexCoords;

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);

void main()
{
	TexCoords = texCoords;
	Position = model * vec4(position.xy,0.0,1.0);
	gl_Position = proj * view * Position;
}
