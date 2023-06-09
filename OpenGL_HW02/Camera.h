#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

class Camera
{
public:
	Camera() = default;

	Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up);

	void updateViewMatrix();

	void setCameraView(glm::vec3 newPosition, glm::vec3 newLookAt, glm::vec3 newUp);

	void increaseCameraDistance(int distance);

	/* Getters */

	glm::mat4 getViewMatrix() const { return viewMatrix;  }
	glm::vec3 getPosition() const { return position; }
	glm::vec3 getLookAt() const { return lookAt;  }
	glm::vec3 getViewDir() const { return -glm::transpose(viewMatrix)[2]; }
	glm::vec3 getUpVector() const { return up; }
	glm::vec3 getRightVector() const { return glm::transpose(viewMatrix)[0]; }
	/* /Getters */


private:
	glm::mat4x4 viewMatrix;

	glm::vec3 position;
	glm::vec3 lookAt;
	glm::vec3 up;
};