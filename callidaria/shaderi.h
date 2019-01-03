#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class ShaderI
{
public:
	ShaderI() { }
	void compile(unsigned int ibo)
	{
		std::cout<<"\n\033[1;4;37;40mcompiling inst shader\033[0m\n\n";
		std::cout<<"\033[34mreading instance vertex & fragment\n";
		const char* vertexSource = "#version 330\n\r"
			"in vec2 position;"
			"in vec2 texCoords;"
			"in vec2 offset;"
			"out vec2 TexCoords;"
			"uniform mat4 model = mat4(1.0);"
			"uniform mat4 view = mat4(1.0);"
			"uniform mat4 proj = mat4(1.0);"
			"void main()"
			"{"
			"	TexCoords = texCoords;"
			"	gl_Position = proj * view * model * vec4(position+offset, 0.0, 1.0);"
			"}";
		const char* fragmentSource = "#version 330\n\r"
			"in vec2 TexCoords;"
			"out vec4 outColour;"
			"uniform float alpha = float(1.0);"
			"uniform sampler2D tex;"
			"void main()"
			"{"
			"	vec4 mix = texture(tex, TexCoords);"
			"	if (mix.a<0.3) discard;"
			"	outColour = mix * alpha;"
			"}";
		std::cout<<"compiling instance shaders\n";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader); int status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			std::cout << "\033[1;31minstance vertex shader error:\033[36m\n" << buffer << "\033[0m\n";
		}
		else std::cout << "\033[1;32minstance vertex shader compiled successfully\033[0m\n";

		unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			std::cout << "\033[1;31minstance fragment shader error\033[36m\n" << buffer << "\033[0m\n"; }
		else std::cout << "\033[1;32minstance fragment shader compiled successfully\033[0m\n" << std::endl;

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColour");
		glLinkProgram(shaderProgram);
		enable();

		int posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,
				4*sizeof(float),0);

		int texAttrib = glGetAttribLocation(shaderProgram, "texCoords");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,
				4*sizeof(float),(void*)(2*sizeof(float)));

		int offsetAttrib = glGetAttribLocation(shaderProgram, "offset");
		glEnableVertexAttribArray(offsetAttrib);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glVertexAttribPointer(offsetAttrib,2,GL_FLOAT,GL_FALSE,
				2*sizeof(float),0);
		glVertexAttribDivisor(offsetAttrib,1);

		modelUni = glGetUniformLocation(shaderProgram, "model");
		viewUni = glGetUniformLocation(shaderProgram, "view");
		projUni = glGetUniformLocation(shaderProgram, "proj");
	}
	void enable()
	{
		glUseProgram(shaderProgram);
	}
public:
	unsigned int shaderProgram;
public:
	int modelUni, viewUni, projUni;
};
