#version 130

in vec2 position;
in vec2 texCoords;

out vec2 TexCoords;
out vec2 blurTexCoords[11];

uniform mat4 model = mat4(1.0);
uniform mat4 view = mat4(1.0);
uniform mat4 proj = mat4(1.0);
uniform int ssdiv = 1;
uniform int ssidx = 1;

void main()
{
	TexCoords = texCoords;
	gl_Position = proj * view * model * vec4(position, 0.0, 1.0);
}
