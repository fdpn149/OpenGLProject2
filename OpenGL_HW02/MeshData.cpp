#include "pch.h"
#include "MeshData.h"

void MeshData::to_json(json& j, const SelectedMeshData& data)
{
	std::vector<std::vector<float>> vertices(data.vertices.size(), std::vector<float>(3, 0.0f));


	for (int iii = 0; iii < data.vertices.size(); ++iii)
	{
		for (int jjj = 0; jjj < 3; ++jjj)
		{
			vertices[iii][jjj] = data.vertices[iii][jjj];
		}
	}

	std::vector<std::vector<float>> texcoords(data.texcoords.size(), std::vector<float>(2, 0.0f));
	for (int iii = 0; iii < data.texcoords.size(); ++iii)
	{
		for (int jjj = 0; jjj < 2; ++jjj)
		{
			texcoords[iii][jjj] = data.texcoords[iii][jjj];
		}
	}

	j = json{ { "vertices", vertices }, { "texcoords", texcoords }, { "indices", data.indices }, { "useTexture", data.useTexture } };
}

void MeshData::from_json(const json& j, SelectedMeshData& data)
{
	std::vector<std::vector<float>> vertices;
	std::vector<std::vector<float>> texcoords;

	j.at("vertices").get_to(vertices);
	j.at("texcoords").get_to(texcoords);
	j.at("indices").get_to(data.indices);
	j.at("useTexture").get_to(data.useTexture);

	data.vertices.resize(vertices.size());
	for (int iii = 0; iii < vertices.size(); ++iii)
	{
		for (int jjj = 0; jjj < 3; ++jjj)
		{
			data.vertices[iii][jjj] = vertices[iii][jjj];
		}
	}

	data.texcoords.resize(texcoords.size());
	for (int iii = 0; iii < texcoords.size(); ++iii)
	{
		for (int jjj = 0; jjj < 2; ++jjj)
		{
			data.texcoords[iii][jjj] = texcoords[iii][jjj];
		}
	}
}

void MeshData::to_json(json& j, const SelectedTextureData& data)
{
	j = json{ { "file", data.file } };
}

void MeshData::from_json(const json& j, SelectedTextureData& data)
{
	j.at("file").get_to(data.file);
}