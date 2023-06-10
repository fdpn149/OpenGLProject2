#pragma once

/*                 Standard                 */
#include <set>
#include <vector>


/*                 My Class                 */
#include "Mesh.h"

class SelectedMesh
{
public:
	SelectedMesh() = default;

	/* Getters */

	const TriMesh& getMeshRef() const { return mesh; }
	std::set<unsigned int> getFaceIdSet() { return faceIdSet; }


	void addFace(std::vector<TriMesh::Point> faceVertices, unsigned int id);
	void deleteFace(const std::vector<TriMesh::Point>& faceVertices, unsigned int id);


private:
	TriMesh mesh;
	std::set<unsigned int> faceIdSet;
};

