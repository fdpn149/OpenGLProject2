#include "pch.h"
#include "Camera.h"


#include "Config.h"


Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
	: position(position)
	, lookAt(lookAt)
	, up(up)
{
	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	viewMatrix = glm::lookAt(position, lookAt, up);
}

void Camera::setCameraView(glm::vec3 newPosition, glm::vec3 newLookAt, glm::vec3 newUp)
{
	position = newPosition;
	lookAt = newLookAt;
	up = newUp;

	updateViewMatrix();
}

void Camera::increaseCameraDistance(int distance)
{
	position += (float)distance * (lookAt - position) * Config::SCROLL_SENSITIVITY;

	updateViewMatrix();
}