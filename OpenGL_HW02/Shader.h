#pragma once

/*                 Standard                 */
#include <string>


/*                 GLM                      */
#include <glm/glm.hpp>


class Shader
{
public:
	Shader() = default;

	Shader(const std::string& vertexPath, const std::string& fragmentPath);

	/* Use/Active */

	void use();

	/* Uniforms */

	void setInt(const char* name, int value);
	void setFloat(const char* name, float value);
	void set3Float(const char* name, float v0, float v1, float v2);
	void setVec3(const char* name, glm::vec3 vec);
	void setMat4(const char* name, glm::mat4 mat);

private:
	std::string readFileAsString(const std::string& path);

private:
	unsigned int id;

};

