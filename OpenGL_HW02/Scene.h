#pragma once

/*                 Standard                 */
#include <vector>
#include <set>


/*                 My Class                 */
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Plane.h"
#include "Light.h"
#include "Quad.h"
#include "Skybox.h"

namespace ShaderTypes
{
	enum ShaderTypes
	{
		PICK,
		MODEL,
		DRAW_POINT,
		DRAW_FACE,
		GRID,
		PLANE,
		SHADOW_MAP,
		SKYBOX,
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
	/* Objects */
	Shader shaders[ShaderTypes::MAX_SHADER_NUM];

	Camera camera;

	Light light;

	Mesh mesh;

	Plane plane;

	Quad quad;

	Skybox skybox;

	/* Buffer object*/

	unsigned int pickingFbo;

	unsigned int pickingFboTexId;

	unsigned int rbo;

	unsigned int shadowMapFbo;

	unsigned int shadowMapFboTexId;


	/* Other attributes */

	bool textureUpdated;

	PickMode::PickMode mode;

	glm::mat4 projMat;

	glm::vec3 lightPos; // Using directional light but use a position to origin direction

	unsigned int shadowMapLength;
};
