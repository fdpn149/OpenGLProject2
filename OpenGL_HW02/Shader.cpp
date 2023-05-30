#pragma warning(disable:4996)
#include "pch.h"
#include "Shader.h"

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	char** vShaderCode = loadShaderSource(vertexPath);
	char** fShaderCode = loadShaderSource(fragmentPath);

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	};

	// fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!fragment)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	};

	programID = glCreateProgram();
	glAttachShader(programID, vertex);

	glAttachShader(programID, fragment);
	
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	
	freeShaderSource(vShaderCode);
	freeShaderSource(fShaderCode);
}

char** Shader::loadShaderSource(const char* file)
{
	FILE* fp = fopen(file, "rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* src = new char[sz + 1];
	fread(src, sizeof(char), sz, fp);
	src[sz] = '\0';
	char** srcp = new char* [1];
	srcp[0] = src;
	return srcp;
}

void Shader::freeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

void Shader::use()
{
	glUseProgram(programID);
}

void Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name), value);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name), value);
}

void Shader::set3Float(const char* name, float v0, float v1, float v2)
{
	glUniform3f(glGetUniformLocation(programID, name), v0, v1, v2);
}

void Shader::setVec3(const char* name, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(programID, name), vec.x, vec.y, vec.z);
}

void Shader::setMat4(const char* name, glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(programID, name), 1, GL_FALSE, glm::value_ptr(mat));
}
