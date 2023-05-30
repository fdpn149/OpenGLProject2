#pragma once
#include <vector>
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

class Scene
{
	const int WIDTH = 800;
	const int HEIGHT = 600;

	Shader* shader, * screenShader, * drawPointShader, * drawFaceShader;
	Camera* camera;
	Mesh* mesh;

	uint framebuffer;
	uint textureColorbuffer;
	uint rbo;

	glm::mat4 projMat;

	void pickingFace(uint faceID);
	void deleteFace(uint faceID);
	void picking_point(float depthValue, uint faceID, int x, int y);
public:
	Scene();
	~Scene();
	uint mode = 0;
	void draw();
	void mouseUp();
	void changeDirection(const int& x, const int& y);
	void changePosition(wchar_t key, clock_t deltaTime);
	void picking(int x, int y);
};
