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

	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void set3Float(const std::string& name, float v0, float v1, float v2);
	void setVec3(const std::string& name, glm::vec3 vec);
	void setMat4(const std::string& name, glm::mat4 mat);

private:
	std::string readFileAsString(const std::string& path);

private:
	unsigned int id;

};

