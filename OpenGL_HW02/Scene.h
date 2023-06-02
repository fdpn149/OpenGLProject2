#pragma once
#include <vector>
#include <string>

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "FaceGroup.h"

class Scene
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Shader* shader, * screenShader, * drawPointShader, * drawFaceShader, *gridShader;
	Camera* camera;
	Mesh* mesh;

	std::string currentGroup;
	std::map<std::string, FaceGroup> faceGroupsMap;

	uint framebuffer;
	uint textureColorbuffer;
	uint rbo;

	glm::mat4 projMat;

	void pickingFace(uint faceID);
	void deleteFace(uint faceID);
	void pickingPoint(float depthValue, uint faceID, int x, int y);
public:
	Scene();
	~Scene();
	uint mode = 0;
	void draw();
	void mouseUp();
	void changeDirection(int x, int y);
	void changePosition(wchar_t key, clock_t deltaTime);
	void changePosition(int x, int y);
	void changeDistance(int delta);

	void picking(int x, int y);

	void addGroup(std::string groupName);
	void deleteGroup(std::string groupName);
	void setCurrentGroup(std::string groupName);
	void setCurrentGroupColor(glm::vec3 color);
};
