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
		PICK,
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
	const Mesh& getMeshRef() const { return mesh; }
	bool isTextureUpdated() const { return textureUpdated; }

	/* Setters */
	void setPickMode(PickMode::PickMode newMode) { mode = newMode; }
	void setTextureToMesh(const std::string& file) { mesh.setTexture(file); }
	void setTextureUpdatedState(bool updated) { textureUpdated = updated; }

	void updateSelectedMeshTexcoords() { mesh.setTexcoord(); }

	void setNewSelectMesh() { mesh.setNewSelectMesh(); }

	void saveSelectedMeshData(const std::string& file) { mesh.saveSelectedAsJson(file); }
	void loadSelectedMeshData(const std::string& file) { mesh.loadSelectedFromJson(file); }

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

	//bool useTexture;
	bool textureUpdated;

	PickMode::PickMode mode;

	unsigned int fbo;
	unsigned int fboTexId;
	unsigned int rbo;

	glm::mat4 projMat;
};
