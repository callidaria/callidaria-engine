#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class Shader2D
{
public:
	Shader2D() { }
	void compile()
	{
		std::cout<<"\n\033[1;4;37;40mcreating 2D shader\033[0m\n\n";
		std::cout<<"\033[34mreading 2D vertex shader\n";
		std::string vertexSrc=read("shader/vertex2d.shader");
		const char* vertexSource = vertexSrc.c_str();

		std::cout<<"\033[34mreading 2D fragment shader\n";
		std::string fragmentSrc=read("shader/fragment2d.shader");
		const char* fragmentSource = fragmentSrc.c_str();

		std::cout<<"\n\033[34mcompiling 2D shaders\033[0m\n";
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader); int status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			std::cout << "\033[1;31m2D vertex shader error:\033[36m\n" << buffer << "\033[0m\n"; }
		else std::cout << "\033[1;32m2D vertex shader compiled successfully\033[0m\n";

		unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			std::cout << "\033[1;31m2D fragment shader error\033[36m\n" << buffer << "\033[0m\n"; }
		else std::cout << "\033[1;32m2D fragment shader compiled successfully\033[0m\n" << std::endl;

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

		modelUni = glGetUniformLocation(shaderProgram, "model");
		viewUni = glGetUniformLocation(shaderProgram, "view");
		projUni = glGetUniformLocation(shaderProgram, "proj");
	}
	void enable()
	{
		glUseProgram(shaderProgram);
	}
private:
	std::string read(const char* path)
	{
		std::string ptr;std::ifstream file(path);
		if (!file.is_open())
			std::cout<<"\033[1;31mno shader found\033[0m\n";
		else std::cout<<"\033[1;32mshader loaded\033[0m\n";
		std::string line = ""; while (!file.eof()) {
			std::getline(file,line);
			ptr.append(line + "\n");
		} file.close();
		return ptr;
	}
private:
	unsigned int shaderProgram;
public:
	int modelUni, viewUni, projUni;
};
