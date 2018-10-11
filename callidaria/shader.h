#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Shader
{
public:
	Shader() { }
	void compile()
	{
		const char* vertexSource = "#version 130\n\r"
			"in vec2 position;"
			"in vec2 texCoords;"
			"out vec2 TexCoords;"
			"out vec2 blurTexCoords[11];"
			"uniform mat4 model = mat4(1.0);"
			"uniform bool blur = bool(false);"
			"uniform vec2 bsize = vec2(640, 360);"
			"uniform vec2 ratiodiv = vec2(1, 1);"
			"void main()"
			"{"
			"	TexCoords = texCoords;"
			"	gl_Position = model * vec4(position, 0.0, 1.0);"
			"	if (blur) {"
			"		vec2 ctrTexCoords = vec2(position.x*(0.5/ratiodiv.x)+(0.5/ratiodiv.x),-position.y*(0.5/ratiodiv.y)+(0.5/ratiodiv.y));"
			"		float pixelSizeW = 1.0 / bsize.x; float pixelSizeH = 1.0 / bsize.y;"
			"		for (int i = -5; i <= 5; i++) {"
			"			blurTexCoords[i+5] = ctrTexCoords + vec2(pixelSizeW * i, pixelSizeH * i);"
			"		}"
				"}"
			"}";
		const char* fragmentSource = "#version 130\n\r"
			"in vec2 TexCoords;"
			"in vec2 blurTexCoords[11];"
			"out vec4 outColour;"
			"uniform bool blur = bool(false);"
			"uniform float alpha = float(1.0);"
			"uniform sampler2D tex;"
			"void main()"
			"{"
			"	if (!blur) {"
			"		vec4 mix = texture(tex, TexCoords);"
			"		if (mix.a<0.3) discard;"
			"		outColour = mix * alpha;"
			"	}"
			"	else {" //blur still in test stage
			"		vec4 blurt = vec4(0.0);"
			"		blurt += texture(tex, blurTexCoords[0]) * 0.0093;"
			"		blurt += texture(tex, blurTexCoords[1]) * 0.028002;"
			"		blurt += texture(tex, blurTexCoords[2]) * 0.065984;"
			"		blurt += texture(tex, blurTexCoords[3]) * 0.121703;"
			"		blurt += texture(tex, blurTexCoords[4]) * 0.175713;"
			"		blurt += texture(tex, blurTexCoords[5]) * 0.198596;"
			"		blurt += texture(tex, blurTexCoords[6]) * 0.175713;"
			"		blurt += texture(tex, blurTexCoords[7]) * 0.121703;"
			"		blurt += texture(tex, blurTexCoords[8]) * 0.065984;"
			"		blurt += texture(tex, blurTexCoords[9]) * 0.028002;"
			"		blurt += texture(tex, blurTexCoords[10]) * 0.0093;"
			"		outColour = blurt * alpha;"
			"	}"
			"}";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader);
		int status; glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) { char buffer[512]; glGetShaderInfoLog(vertexShader, 512, NULL, buffer); std::cout << buffer << std::endl; }
		else std::cout << "vertex shader compiled successfully" << std::endl;

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) { char buffer[512]; glGetShaderInfoLog(fragmentShader, 512, NULL, buffer); std::cout << buffer << std::endl; }
		else std::cout << "fragment shader compiled successfully" << std::endl;

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader); glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColour");
		glLinkProgram(shaderProgram);
		enable();

		int posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		int texAttrib = glGetAttribLocation(shaderProgram, "texCoords");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

		modelUni = glGetUniformLocation(shaderProgram, "model");
	}
	void enable()
	{
		glUseProgram(shaderProgram);
	}
private:
	unsigned int shaderProgram;
public:
	int modelUni;
};
