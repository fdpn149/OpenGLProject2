#pragma once

/*                 Standard                 */
#include <vector>
#include <set>


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

	/* Getters */
	Camera& getCameraRef() { return camera; }
	const TriMesh& getSelectedMeshRef() const { return mesh.getSelectedMeshRef(); }

	/* Setters */
	void setPickMode(PickMode::PickMode newMode) { mode = newMode; }


	void draw();
	void pick(int x, int y);

private:
	void pickFace(unsigned int faceId);
	void deleteFace(unsigned int faceId);
	void pickingPoint(float depthValue, unsigned int faceId, int x, int y);

private:
	Shader shaders[ShaderTypes::MAX_SHADER_NUM];

	Camera camera;

	Mesh mesh;

	PickMode::PickMode mode;

	unsigned int fbo;
	unsigned int fboTexId;
	unsigned int rbo;

	glm::mat4 projMat;
};
