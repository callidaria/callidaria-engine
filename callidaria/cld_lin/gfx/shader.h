#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader() { }
	void compile2d(const char* vspath, const char* fspath)
	{
		std::string vertexSrc=read(vspath);
		const char* vertexSource = vertexSrc.c_str();
		std::string fragmentSrc=read(fspath);
		const char* fragmentSource = fragmentSrc.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader); int status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			std::cout << "\033[1;31mfb vertex shader error:\033[36m\n" << buffer << "\033[0m\n"; }

		unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			std::cout << "\033[1;31mfb fragment shader error\033[36m\n" << buffer << "\033[0m\n"; }

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColour");
		glLinkProgram(shaderProgram);
		enable();

		int posAttrib = glGetAttribLocation(shaderProgram,"position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,
				4*sizeof(float),0);

		int texAttrib = glGetAttribLocation(shaderProgram,"texCoords");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,
				4*sizeof(float),(void*)(2*sizeof(float)));
	}
	void compile3d(const char* vspath,const char* fspath)
	{
		std::string vertexSrc=read(vspath);
		const char* vertexSource = vertexSrc.c_str();
		std::string fragmentSrc=read(fspath);
		const char* fragmentSource = fragmentSrc.c_str();

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexSource, NULL);
		glCompileShader(vertexShader); int status;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
			std::cout << "\033[1;31mfb vertex shader error:\033[36m\n" << buffer << "\033[0m\n"; }

		unsigned int fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
		if (status != GL_TRUE) {
			char buffer[512];
			glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
			std::cout << "\033[1;31mfb fragment shader error\033[36m\n" << buffer << "\033[0m\n"; }

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glBindFragDataLocation(shaderProgram, 0, "outColour");
		glLinkProgram(shaderProgram);
		enable();

		int posAttrib = glGetAttribLocation(shaderProgram,"position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib,3,GL_FLOAT,GL_FALSE,
				8*sizeof(float),0);

		int texAttrib = glGetAttribLocation(shaderProgram,"texCoords");
		glEnableVertexAttribArray(texAttrib);
		glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,
				8*sizeof(float),(void*)(3*sizeof(float)));

		int nmlAttrib = glGetAttribLocation(shaderProgram,"normals");
		glEnableVertexAttribArray(nmlAttrib);
		glVertexAttribPointer(nmlAttrib,3,GL_FLOAT,GL_FALSE,
				8*sizeof(float),(void*)(5*sizeof(float)));
	}
	void load_index(unsigned int ibo)
	{
		int offsetAttrib = glGetAttribLocation(shaderProgram,"offset");
		glEnableVertexAttribArray(offsetAttrib);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glVertexAttribPointer(offsetAttrib,2,GL_FLOAT,GL_FALSE,
				2*sizeof(float),0);
		glVertexAttribDivisor(offsetAttrib,1);
	}
	void load_text(unsigned int ibo)
	{
		int offsetAttrib=glGetAttribLocation(shaderProgram,"offset");
		glEnableVertexAttribArray(offsetAttrib);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glVertexAttribPointer(offsetAttrib,2,GL_FLOAT,GL_FALSE,
				8*sizeof(float),0);
		glVertexAttribDivisor(offsetAttrib,1);
		int texposAttrib=glGetAttribLocation(shaderProgram,"texpos");
		glEnableVertexAttribArray(texposAttrib);
		glVertexAttribPointer(texposAttrib,2,GL_FLOAT,GL_FALSE,
				8*sizeof(float),(void*)(2*sizeof(float)));
		glVertexAttribDivisor(texposAttrib,1);
		int boundsAttrib=glGetAttribLocation(shaderProgram,"bounds");
		glEnableVertexAttribArray(boundsAttrib);
		glVertexAttribPointer(boundsAttrib,2,GL_FLOAT,GL_FALSE,
				8*sizeof(float),(void*)(4*sizeof(float)));
		glVertexAttribDivisor(boundsAttrib,1);
		int cursorAttrib=glGetAttribLocation(shaderProgram,"cursor");
		glEnableVertexAttribArray(cursorAttrib);
		glVertexAttribPointer(cursorAttrib,2,GL_FLOAT,GL_FALSE,
				8*sizeof(float),(void*)(6*sizeof(float)));
		glVertexAttribDivisor(cursorAttrib,1);
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
		std::string line = ""; while (!file.eof()) {
			std::getline(file,line);
			ptr.append(line + "\n");
		} file.close();
		return ptr;
	}
public:
	void upload_int(const char* loc,int i)
	{ glUniform1i(glGetUniformLocation(shaderProgram,loc),i); }
	void upload_float(const char* loc,float f)
	{ glUniform1f(glGetUniformLocation(shaderProgram,loc),f); }
	void upload_vec2(const char* loc,glm::vec2 v)
	{ glUniform2f(glGetUniformLocation(shaderProgram,loc),v.x,v.y); }
	void upload_vec3(const char* loc,glm::vec3 v)
	{ glUniform3f(glGetUniformLocation(shaderProgram,loc),v.x,v.y,v.z); }
	void upload_vec4(const char* loc,glm::vec4 v)
	{ glUniform4f(glGetUniformLocation(shaderProgram,loc),v.x,v.y,v.z,v.w); }
	void upload_matrix(const char* loc,glm::mat4 m)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram,loc),
				1,GL_FALSE,glm::value_ptr(m));
	}
private:
	unsigned int shaderProgram;
};
