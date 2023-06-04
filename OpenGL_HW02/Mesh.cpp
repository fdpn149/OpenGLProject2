#include "pch.h"
#include "Mesh.h"
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

glm::vec3 Mesh::pointToVec3(const TriMesh::Point& point)
{
	return glm::vec3(point[0], point[1], point[2]);
}

TriMesh::Point Mesh::vec3ToPoint(const glm::vec3 vec)
{
	return TriMesh::Point(vec.x, vec.y, vec.z);
}

Mesh::Mesh()
{
	modelMat = glm::mat4(1.0f);

	OpenMesh::IO::read_mesh(model, "assets/models/UnionSphere.obj");
	//OpenMesh::IO::read_mesh(model, "assets/models/armadillo.obj");

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



	glGenVertexArrays(1, &vao3);
	glGenBuffers(1, &vbo3);


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

void Mesh::drawLine()
{
	glBindVertexArray(vao3);
	glDrawArrays(GL_LINES, 0, lines.size());
	glBindVertexArray(0);
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
			std::set<TriMesh::VertexHandle> selected_vhs;

			for (auto selected_f_it = selected.faces_begin(); selected_f_it != selected.faces_end(); selected_f_it++)
			{
				for (auto selected_fv_it = selected.fv_begin(*selected_f_it); selected_fv_it.is_valid(); selected_fv_it++)
				{
					selected_vhs.insert(*selected_fv_it);
				}
			}

			TriMesh::FaceHandle model_fh = model.face_handle(faceID);

			std::vector<TriMesh::VertexHandle> face_vertices;

			for (TriMesh::FVIter model_fv_it = model.fv_iter(model_fh); model_fv_it.is_valid(); model_fv_it++)
			{
				TriMesh::Point model_point = model.point(*model_fv_it);
				TriMesh& s = selected;
				auto find_point = std::find_if(selected_vhs.begin(), selected_vhs.end(),
					[model_point, s](const TriMesh::VertexHandle& selected_vh) {return s.point(selected_vh) == model_point; });
				if (find_point != selected_vhs.end())
				{
					face_vertices.push_back(*find_point);
				}
				else
				{
					face_vertices.push_back(selected.add_vertex(model_point));
				}
			}

			selected.add_face(face_vertices);

#ifdef WRITE_OBJ
			OpenMesh::IO::write_mesh(selected, "assets/models/selected.obj");
#endif // WRITE_OBJ

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
#ifdef DEBUG
				printf("FaceID in Selected is %d\n", f_it->idx());
#endif
				selected.delete_face(*f_it);
			}
		}
		selected.garbage_collection();

#ifdef WRITE_OBJ
		OpenMesh::IO::write_mesh(selected, "assets/models/selected.obj");
#endif

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

void Mesh::setLinePosition()
{
	glBindVertexArray(vao3);
	glBindBuffer(GL_ARRAY_BUFFER, vbo3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lines.size() * 3, lines.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}

void Mesh::calculateSurround(std::vector<float>& percent)
{
	percent.clear();
	lines.clear();
#pragma region CALCULATE_PERCENT
	TriMesh::HalfedgeIter h_it = selected.halfedges_begin();
	if (h_it == selected.halfedges_end())
		return;
	for (; h_it != selected.halfedges_end(); h_it++)
	{
		if (selected.is_boundary(*h_it))
			break;
	}

	TriMesh::HalfedgeHandle hh_init(*h_it);

	TriMesh::HalfedgeHandle hh(hh_init);
	

	//travel all halfedge on boundary
	float total = 0;
	int t = 0;
	do
	{
		TriMesh::VertexHandle vh = selected.to_vertex_handle(hh);
		TriMesh::VertexHandle vh2 = selected.from_vertex_handle(hh);

		TriMesh::Point p1 = selected.point(vh);
		TriMesh::Point p2 = selected.point(vh2);

		float length = (p1 - p2).norm();

		percent.push_back(length);
		total += length;

		hh = selected.next_halfedge_handle(hh);
		if (!hh.is_valid())
			return;
	} while (hh != hh_init);

	//calculate percent
	float sum = 0;
	for (float& f : percent)
	{
		sum += f / total;
		f = sum;
	}
#pragma endregion

	std::vector<TriMesh::VertexHandle> inside_points;

	int count = 0;
	for (auto v_it = selected.vertices_begin(); v_it != selected.vertices_end(); v_it++) //find center_point
	{
		if (!selected.is_boundary(*v_it))
		{
			inside_points.push_back(*v_it);
			TriMesh::Point center_point = selected.point(*v_it);
			glm::vec3 center_vec = pointToVec3(center_point);

			TriMesh::Point prev_point, curr_point, next_point;
			glm::vec3 prev_vec, curr_vec, next_vec;
			glm::vec3 to_center, to_current;

			for (auto voh_it = selected.voh_begin(*v_it); voh_it != selected.voh_end(*v_it); voh_it++)
			{
				prev_point = selected.point(selected.opposite_vh(*voh_it));
				curr_point = selected.point(selected.to_vertex_handle(*voh_it));
				next_point = selected.point(selected.opposite_he_opposite_vh(*voh_it));

				prev_vec = pointToVec3(prev_point);
				curr_vec = pointToVec3(curr_point);
				next_vec = pointToVec3(next_point);

				to_center = center_vec - prev_vec;
				to_current = curr_vec - prev_vec;
				float gamma = glm::acos(glm::dot(to_center, to_current) / (to_center.length() * to_current.length()));

				to_center = center_vec - next_vec;
				to_current = curr_vec - next_vec;
				float beta = glm::acos(glm::dot(to_center, to_current) / (to_center.length() * to_current.length()));

				float w = 1.0f / glm::tan(beta) + 1.0f / glm::tan(gamma);

				printf("%f\n", w);
			}
		}
	}
	
	//setLinePosition();
}
