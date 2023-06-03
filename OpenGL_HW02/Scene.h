#pragma once
#include <vector>
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

class Scene
{
	int width = 800;
	int height = 600;

	Shader* shader, * screenShader, * drawPointShader, * drawFaceShader, *gridShader,* drawLineShader;
	Camera* camera;
	Mesh* mesh;

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
	void calculateSurround(std::vector<float>& percent);

	void resize();
};
