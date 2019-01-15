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
		std::cout<<"\033[34mreading instance vertex shader\n";
		std::string vertexSrc = read("shader/vertex_inst.shader");
		const char* vertexSource = vertexSrc.c_str();

		std::cout<<"\033[34mreading instance fragment shader\n";
		std::string fragmentSrc = read("shader/fragment_inst.shader");
		const char* fragmentSource = fragmentSrc.c_str();

		std::cout<<"\n\033[34mcompiling instance shaders\033[0m\n";
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
