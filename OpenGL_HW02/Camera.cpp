#include "pch.h"
#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
{
	this->position = position;
	this->worldUp = worldUp;
	this->forward = glm::normalize(position - target);
	this->right = glm::normalize(glm::cross(target, worldUp));
	this->up = glm::normalize(glm::cross(right, target));
	this->pitch = 0;
	this->yaw = 0;

	// get mouse into the application
	lastX = SCR_WIDTH / 2;
	lastY = SCR_HEIGHT / 2;

	firstMouse = true;
	freeLooking = true;
}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp)
{
	this->position = position;
	this->worldUp = worldUp;
	this->pitch = pitch;
	this->yaw = yaw;
	forward.x = glm::cos(pitch) * glm::sin(yaw);
	forward.y = glm::sin(pitch);
	forward.z = glm::cos(pitch) * glm::cos(yaw);
	forward = glm::normalize(forward);
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));

	// get mouse into the application
	lastX = SCR_WIDTH / 2;
	lastY = SCR_HEIGHT / 2;

	firstMouse = true;
	freeLooking = true;
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position - forward, worldUp);
}

void Camera::onMousePositionChanged(int xPos, int yPos, int key)
{
	if (freeLooking)
	{
		if (firstMouse)
		{
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		float xOffset = (float)lastX - xPos;
		float yOffset = (float)yPos - lastY;

		lastX = xPos;
		lastY = yPos;

		xOffset *= sensitivity;
		yOffset *= sensitivity;

		if (key == 0)
		{
			yaw += xOffset;
			pitch += yOffset;


			if (pitch > 89.0f)
				pitch = 89.0f;
			else if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 front;
			front.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
			front.y = sin(glm::radians(pitch));
			front.z = cos(glm::radians(pitch)) * cos(glm::radians(yaw));

			forward = glm::normalize(front);
			right = glm::normalize(glm::cross(forward, worldUp));
			up = glm::normalize(glm::cross(right, forward));
		}
		else if (key == 1)
		{
			position += xOffset * right * 0.1f;
			position -= yOffset * up * 0.1f;
		}
	}
}

void Camera::onMouseWheelScroll(int delta)
{
	position -= (float)delta * forward * 0.001f;
}

void Camera::onKeyPressed(wchar_t key, long deltaTime)
{
	float cameraSpeed = 0.0001f * deltaTime;

	switch (key)
	{
	case 'w':
		position -= cameraSpeed * forward;
		break;
	case 's':
		position += cameraSpeed * forward;
		break;
	case 'd':
		position -= cameraSpeed * right;
		break;
	case 'a':
		position += cameraSpeed * right;
		break;
	case 'e':
		position += cameraSpeed * up;
		break;
	case 'q':
		position -= cameraSpeed * up;
		break;
	}
}
