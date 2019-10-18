#version 330 core

in vec2 position;
in vec2 texCoords;

out vec2 blurTexCoords[11];

void main()
{
	gl_Position = vec4(position.x,position.y, 0.0, 1.0);

	vec2 originTex = position * 0.5 + 0.5;
	float pixel = 1.0 / (1080.0/4.0);

	for(int i=-5;i<5;i++) {
		blurTexCoords[i+5]=abs(originTex+vec2(0.0,pixel*i));
	}
}
