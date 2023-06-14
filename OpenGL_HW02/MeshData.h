#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>



namespace MeshData
{
	using json = nlohmann::json;
	using TriMesh = OpenMesh::TriMesh_ArrayKernelT<>;

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

