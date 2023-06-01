#include "pch.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

Mesh::Mesh()
{
	modelMat = glm::mat4(1.0f);

	OpenMesh::IO::read_mesh(model, "assets/models/armadillo.obj");

	model.request_face_normals();
	model.request_vertex_status();
	model.request_face_status();
	model.request_edge_status();
	model.update_normals();

	std::vector<TriMesh::Point> vertices;
	vertices.reserve(model.n_vertices());
	for (TriMesh::VertexIter v_it = model.vertices_begin(); v_it != model.vertices_end(); ++v_it)
	{
		vertices.push_back(model.point(*v_it));

		TriMesh::Point p = model.point(*v_it);
	}

	std::vector<uint> indices;
	indices.reserve(model.n_faces() * 3);
	for (TriMesh::FaceIter f_it = model.faces_begin(); f_it != model.faces_end(); ++f_it)
	{
		for (TriMesh::FaceVertexIter fv_it = model.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			indices.push_back(fv_it->idx());
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	glGenBuffers(1, &vert_vbo);


	glGenVertexArrays(1, &vao2);

	glGenBuffers(1, &vbo2);

}

void Mesh::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::drawFace()
{
	glBindVertexArray(vao);

	for (uint face : selectedFace)
	{
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLuint*)(face * 3 * sizeof(GLuint)));
	}

	glBindVertexArray(0);
}

void Mesh::drawPoint()
{
	glPointSize(15.0f);
	glDrawArrays(GL_POINTS, 0, 1);
}

void Mesh::drawSelected()
{
	glBindVertexArray(vao2);
	glDrawElements(GL_TRIANGLES, selected.n_faces() * 3, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
}

void Mesh::addSelectedFace(uint faceID)
{
	if (faceID >= 0 && faceID < model.n_faces())
	{
		bool inserted = selectedFace.insert(faceID).second;

		if (inserted)
		{
			TriMesh::FaceHandle fh = model.face_handle(faceID);
			std::vector<TriMesh::VertexHandle> face_vertices;
			TriMesh::Point select_point;
			std::vector<TriMesh::Point>::iterator vecp_it;

			int i = 0;
			for (TriMesh::FVIter fv_it = model.fv_iter(fh); fv_it.is_valid(); fv_it++)
			{
				select_point = model.point(*fv_it);

				vecp_it = std::find(select_vertices.begin(), select_vertices.end(), select_point);
				
				if (vecp_it == select_vertices.end())
				{
					TriMesh::VertexHandle vh = selected.add_vertex(select_point);

					face_vertices.push_back(vh);
					select_vertices.push_back(select_point);
				}
				else
				{
					TriMesh::VertexHandle vh = selected.vertex_handle(vecp_it - select_vertices.begin());
					if (!vh.is_valid())
					{
						vh = selected.add_vertex(select_point);
					}
					face_vertices.push_back(vh);
				}
				
				i++;
			}
			selected.add_face(face_vertices);


			OpenMesh::IO::write_mesh(selected, "assets/models/selected.obj");

			selected.request_face_normals();
			selected.request_vertex_status();
			selected.request_face_status();
			selected.request_edge_status();
			selected.update_normals();

			std::vector<TriMesh::Point> vertices;
			vertices.reserve(selected.n_vertices());
			for (TriMesh::VertexIter v_it = selected.vertices_begin(); v_it != selected.vertices_end(); ++v_it)
			{
				vertices.push_back(selected.point(*v_it));

				TriMesh::Point p = selected.point(*v_it);
			}

			std::vector<uint> indices;
			indices.reserve(selected.n_faces() * 3);
			for (TriMesh::FaceIter f_it = selected.faces_begin(); f_it != selected.faces_end(); ++f_it)
			{
				for (TriMesh::FaceVertexIter fv_it = selected.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
				{
					indices.push_back(fv_it->idx());
				}
			}

			glBindVertexArray(vao2);
			glBindBuffer(GL_ARRAY_BUFFER, vbo2);
			glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

			glBindVertexArray(0);
		}
	}
}

void Mesh::deleteSelectedFace(uint faceID)
{
	std::set<uint>::iterator setu_it;
	setu_it = selectedFace.find(faceID);
	if (setu_it != selectedFace.end())
	{
		selectedFace.erase(faceID);

		TriMesh::FaceHandle fh(faceID);
		std::set<TriMesh::Point> targetVertices;
		for (auto fv_it = model.fv_begin(fh); fv_it != model.fv_end(fh); fv_it++)
		{
			targetVertices.insert(model.point(*fv_it));
		}

		for (auto f_it = selected.faces_begin(); f_it != selected.faces_end(); f_it++)
		{
			int same = 0;
			for (auto fv_it = selected.fv_begin(*f_it); fv_it != selected.fv_end(*f_it); fv_it++)
			{
				if (targetVertices.find(selected.point(*fv_it)) != targetVertices.end())
					same++;
			}
			if (same == 3)
			{
				printf("FaceID in Selected is %d\n", f_it->idx());
				selected.delete_face(*f_it);
			}
		}
		selected.garbage_collection();

		OpenMesh::IO::write_mesh(selected, "assets/models/selected.obj");


		selected.request_face_normals();
		selected.request_vertex_status();
		selected.request_face_status();
		selected.request_edge_status();
		selected.update_normals();

		std::vector<TriMesh::Point> vertices;
		vertices.reserve(selected.n_vertices());
		for (TriMesh::VertexIter v_it = selected.vertices_begin(); v_it != selected.vertices_end(); ++v_it)
		{
			vertices.push_back(selected.point(*v_it));

			TriMesh::Point p = selected.point(*v_it);
		}

		std::vector<uint> indices;
		indices.reserve(selected.n_faces() * 3);
		for (TriMesh::FaceIter f_it = selected.faces_begin(); f_it != selected.faces_end(); ++f_it)
		{
			for (TriMesh::FaceVertexIter fv_it = selected.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
			{
				indices.push_back(fv_it->idx());
			}
		}


		glBindVertexArray(vao2);
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TriMesh::Point) * vertices.size(), vertices.size() > 0 ? &vertices[0] : NULL, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), vertices.size() > 0 ? &indices[0] : NULL, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}

TriMesh::Point Mesh::findClosestPoint(uint faceID, glm::vec3 worldPos)
{
	OpenMesh::FaceHandle fh = model.face_handle(faceID);
	if (!fh.is_valid())
	{
		printf("invalid\n");
		return TriMesh::Point();
	}

	double minDistance = 0.0;
	TriMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	TriMesh::FVIter fv_it = model.fv_iter(fh);
	TriMesh::VertexHandle closestVH = *fv_it;
	TriMesh::Point v1 = model.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		TriMesh::Point v = model.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}

	return model.point(closestVH);
}

void Mesh::setPointPosition(glm::vec3 position)
{
	glBindBuffer(GL_ARRAY_BUFFER, vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), glm::value_ptr(position), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
