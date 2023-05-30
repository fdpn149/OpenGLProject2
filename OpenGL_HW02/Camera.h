#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

class Camera
{
public:
	glm::vec3 position;
	glm::vec3 worldUp;
	glm::vec3 forward, right, up;
	float pitch = 0, yaw = 0;
	float sensitivity = 0.1f;
	int lastX, lastY;

	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldUp);

	glm::mat4 getViewMatrix();
	void onMousePositionChanged(int xPos, int yPos);
	void onKeyPressed(wchar_t key, long deltaTime);

	bool firstMouse;
private:
	bool freeLooking;
};

