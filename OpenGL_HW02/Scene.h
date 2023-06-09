#pragma once

/*                 Standard                 */
#include <vector>


/*                 My Class                 */
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"

namespace ShaderTypes
{
	enum ShaderTypes
	{
		MAIN,
		SCREEN,
		DRAW_POINT,
		DRAW_FACE,
		DRAW_LINE,
		GRID,
		MAX_SHADER_NUM
	};
}

namespace PickMode
{
	enum PickMode
	{
		ADD_FACE,
		DELETE_FACE,
		POINT,
		MAX_PICK_MODE_NUM
	};
}

class Scene
{
public:
	Scene();
	~Scene();

	/* Getters */
	Camera& getCameraRef() { return camera; }

	/* Setters */
	void setPickMode(PickMode::PickMode newMode) { mode = newMode; }


	void draw();
	void pick(int x, int y);

private:
	void pickingFace(uint faceID);
	void deleteFace(uint faceID);
	void pickingPoint(float depthValue, uint faceID, int x, int y);

private:
	Shader shaders[ShaderTypes::MAX_SHADER_NUM];

	Camera camera;
	Mesh* mesh;

	PickMode::PickMode mode;

	unsigned int fbo;
	unsigned int fboTexId;
	unsigned int rbo;

	glm::mat4 projMat;
};
