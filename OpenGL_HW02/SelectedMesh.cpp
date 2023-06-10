#include "pch.h"
#include "SelectedMesh.h"

#include <iostream>
#include <algorithm>

#include "Config.h"

void SelectedMesh::addFace(std::vector<TriMesh::Point> faceVertices, unsigned int id)
{
	if (faceIdSet.find(id) == faceIdSet.end() && faceVertices.size() == 3)
	{
		std::vector<bool> isExist(faceVertices.size(), false);
		std::vector<TriMesh::VertexHandle> faceVertHandles(faceVertices.size());

		for (TriMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			auto it = std::find(faceVertices.begin(), faceVertices.end(), mesh.point(*v_it));

			if (it != faceVertices.end())
			{
				faceVertHandles[it - faceVertices.begin()] = *v_it;
				isExist[it - faceVertices.begin()] = true;
			}
		}

		for (int iii = 0; iii < faceVertHandles.size(); ++iii)
		{
			if (!isExist[iii])
			{
				faceVertHandles[iii] = mesh.add_vertex(faceVertices[iii]);
			}
		}


		mesh.add_face(faceVertHandles);
		faceIdSet.insert(id);
	}

	mesh.request_vertex_status();
	mesh.request_face_status();
	mesh.request_edge_status();
	mesh.request_face_normals();
	mesh.update_normals();
}

void SelectedMesh::deleteFace(const std::vector<TriMesh::Point>& faceVertices, unsigned int id)
{
	if (faceIdSet.find(id) != faceIdSet.end() && faceVertices.size() == 3)
	{
		/* Loop all faces */
		for (TriMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			// Check if is deleting face
			bool isDeletingFace = true;
			for (TriMesh::FVIter fv_it = mesh.fv_iter(f_it); fv_it.is_valid(); ++fv_it)
			{
				if (std::find(faceVertices.begin(), faceVertices.end(), mesh.point(*fv_it)) == faceVertices.end())
				{
					isDeletingFace = false;
					break;
				}
			}

			// Delete the face and stop loop
			if (isDeletingFace)
			{
				mesh.delete_face(*f_it);
				break;
			}
		}

		faceIdSet.erase(id);
	}

	mesh.garbage_collection();

	mesh.request_vertex_status();
	mesh.request_face_status();
	mesh.request_edge_status();
	mesh.request_face_normals();
	mesh.update_normals();
}
