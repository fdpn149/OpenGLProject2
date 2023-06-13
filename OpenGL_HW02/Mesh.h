#pragma once

/*                 Standard                 */
#include <iostream>
#include <set>
#include <map>
#include <string>


/*                 GLM                      */
#include <glm/glm.hpp>


/*                 OpenMesh                 */
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>


/*                 JSON                     */
#include <nlohmann/json.hpp>
using json = nlohmann::json;



/*                 My Class                 */
#include "Shader.h"


/*                 Type def                 */
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

namespace MeshData
{
	struct SelectedMeshData
	{
		std::vector<TriMesh::Point> vertices;
		std::vector<glm::vec2> texcoords;
		std::vector<unsigned int> indices;
		bool useTexture;

		unsigned int vao;
		unsigned int vbo[2];
		unsigned int ebo;
	};

	struct SelectedTextureData
	{
		unsigned int id;
		std::string file;
	};

	void to_json(json& j, const SelectedMeshData& data);
	void from_json(const json& j, SelectedMeshData& data);

	void to_json(json& j, const SelectedTextureData& data);
	void from_json(const json& j, SelectedTextureData& data);
}

class Mesh
{	
public:
	/*struct SelectedMeshData
	{
		std::vector<TriMesh::Point> vertices;
		std::vector<glm::vec2> texcoords;
		std::vector<unsigned int> indices;
		bool useTexture;

		unsigned int vao;
		unsigned int vbo[2];
		unsigned int ebo;
	};*/

	Mesh();
	Mesh(const std::string& file);

	void loadMesh(const std::string& file);

	void saveSelectedAsJson(const std::string& file);
	void loadSelectedFromJson(const std::string& file);


	const TriMesh& getSelectedMeshRef() const { return selectedMesh; }
	const std::vector<TriMesh::Point>& getSelectedVertices() const { return (*(selectedMeshData.end() - 1)).vertices; }
	const std::vector<glm::vec2>& getSelectedTexcoords() const { return (*(selectedMeshData.end() - 1)).texcoords; }
	const std::vector<unsigned int>& getSelectedIndices() const { return (*(selectedMeshData.end() - 1)).indices; }
	unsigned int getTexIdByIdx(int idx) { return textureIds.at(idx); }
	int getTextureNum() { return textureIds.size(); }

	void setTexture(const std::string& file);
	void setTexcoord();

	void draw();
	void drawSelected(Shader& shader);
	//void drawSelecetedFaces();
	void drawPoint();
	//void drawLine();	//For Debug

	void setNewSelectMesh();


	void addFaceToSelectedById(int faceId);
	void deleteFaceFromSelectedById(int faceId);

	TriMesh::Point findClosestPoint(uint faceID, glm::vec3 worldPos);
	void setPointPosition(glm::vec3 position);
	//void setLinePosition();

	//void to_json(json& j, const SelectedMeshData& data);
	//void from_json(json& j, SelectedMeshData& data);

private:
	void updateSelectedBufferObjects();
	void updateSelectedFVMap(int face_3verticesID[]);

	void initSelectedBufferObjs();
	void loadSelectedBufferObjs();
	void loadSelectedTextures();


private:
	TriMesh modelMesh;
	TriMesh selectedMesh;

	std::map<int, int> selectedModelFaceMap;
	std::map<int, int> selectedModelVertMap;

	OpenMesh::FPropHandleT<int> faceIdPropHanlde;
	OpenMesh::VPropHandleT<int> vertIdPropHandle;


	std::vector<TriMesh::Point> modelVertices;
	std::vector<unsigned int> modelIndices;

	std::vector<MeshData::SelectedMeshData> selectedMeshData;
	std::vector<MeshData::SelectedTextureData> selectedTextureData;

	glm::mat4 modelMat;

	std::vector<TriMesh::Point> lines;

	unsigned int selectedVertVbo;

	unsigned int modelVbo;
	unsigned int modelVao;
	unsigned int modelEbo;

	std::vector<unsigned int> textureIds;

	unsigned int selectedTexId;

	//unsigned int vao3, vbo3;	//Draw Line (For Debug)
};