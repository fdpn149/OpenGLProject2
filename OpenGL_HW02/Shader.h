#pragma once
#include <glm/glm.hpp>

class Shader
{
	unsigned int programID;
	char** loadShaderSource(const char* file);
	void freeShaderSource(char** srcp);
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void set3Float(const char* name, float v0, float v1, float v2);
	void setVec3(const char* name, glm::vec3 vec);
	void setMat4(const char* name, glm::mat4 mat);
};

