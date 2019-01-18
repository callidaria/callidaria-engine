#version 130

in vec2 TexCoords;

out vec4 outColour;

uniform sampler2D tex;

void main()
{
	vec4 mix = texture(tex, TexCoords);
	outColour = mix;
}
