#include "pch.h"
#include "Shader.h"

/*                 Standard                 */
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


/*                 OpenGL                   */
#include <GL/glew.h>


/*                 GLM                      */
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
	int success;
	char log[512];

	/* Compile vertex shader */

	std::string vertexCode{ readFileAsString(vertexPath) };

	const char* vertexCodePtr = vertexCode.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexCodePtr, NULL);

	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << log << std::endl;
	}


	/* Compile fragment shader */

	std::string fragmentCode{ readFileAsString(fragmentPath) };

	const char* fragmentCodePtr = fragmentCode.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentCodePtr, NULL);

	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << log << std::endl;
	}


	/* Create and link program */

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);


	/* Free shaders*/

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setInt(const char* name, int value)
{
	glUniform1i(glGetUniformLocation(id, name), value);
}

void Shader::setFloat(const char* name, float value)
{
	glUniform1f(glGetUniformLocation(id, name), value);
}

void Shader::set3Float(const char* name, float v0, float v1, float v2)
{
	glUniform3f(glGetUniformLocation(id, name), v0, v1, v2);
}

void Shader::setVec3(const char* name, glm::vec3 vec)
{
	glUniform3f(glGetUniformLocation(id, name), vec.x, vec.y, vec.z);
}

void Shader::setMat4(const char* name, glm::mat4 mat)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat));
}

std::string Shader::readFileAsString(const std::string& path)
{
	std::string content;

	std::ifstream fileStream(path, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cout << "Can't open the file. File name: " << path << std::endl;

		return std::string();
	}

	std::stringstream sstream;
	sstream << fileStream.rdbuf();

	fileStream.close();

	return sstream.str();
}
