#include "pch.h"
#include "ConvexCombMap.h"

/*                 Standard                 */
#include <map>

/*                 OpenMesh                 */
#define _USE_MATH_DEFINES
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriConnectivity.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

/*                 Glm                      */
#include <glm/glm.hpp>
#include <glm/gtc/reciprocal.hpp>
#include "util_glm.hpp"

/*                 Eigen                    */
#include <eigen3/Eigen/Dense>

/*                 Utilities                */
#include "utilities.h"

/*                 Type Defs                */
typedef OpenMesh::TriMesh_ArrayKernelT<> TriMesh;

namespace
{
	/* Types */
	struct BoundaryLengthMap
	{
		std::map<TriMesh::HalfedgeHandle, float> lengths;
		float totalLen;
	};

	struct NeighborWeights
	{
		std::map<TriMesh::VertexHandle, float> weights;
		float totalWeight;
	};


	/* Variables */

	// Stores resulted map
	std::map<glm::vec3, glm::vec2> convexCombMap;

	// Store boundary lengths
	BoundaryLengthMap boundaryLengthMap = { {}, 0.0f };

	// Store weights from inner vertex to its neighbors
	std::map<TriMesh::VertexHandle, NeighborWeights> neighborWeightMap;

	// Store inner vertices for solve linear system
	std::vector<TriMesh::VertexHandle> innerVertHandles;


	/* Functions */

	TriMesh::HalfedgeHandle getFirstBoundaryHeH(const TriMesh& mesh);
	void calcBoundaryLengths(const TriMesh& mesh, const TriMesh::HalfedgeHandle& startBoundaryHeH);
	void calcBoundaryTexcoords(const TriMesh& mesh, const TriMesh::HalfedgeHandle& startBoundaryHeH);
	void calcWeights(TriMesh mesh);
	void solveLinearSystem(const TriMesh& mesh);
}

void ConvexCombMap::calc(const TriMesh& mesh)
{
	// Clear old data
	convexCombMap.clear();

	boundaryLengthMap.lengths.clear();
	boundaryLengthMap.totalLen = 0.0f;

	neighborWeightMap.clear();

	innerVertHandles.clear();

	// Return if mesh is empty
	if (mesh.faces_empty())
	{
		return;
	}

	// Get first boundary half edge handle
	TriMesh::HalfedgeHandle startBoundaryHeH = getFirstBoundaryHeH(mesh);

	// Calculate boundary lengths
	calcBoundaryLengths(mesh, startBoundaryHeH);

	// Caculate boundary texcoords
	calcBoundaryTexcoords(mesh, startBoundaryHeH);


	// Calculate inner vertices weights
	calcWeights(mesh);

	if (innerVertHandles.size() > 0)
	{
		// Calculate inner texcoord by solve linear system
		solveLinearSystem(mesh);
	}
}

glm::vec2 ConvexCombMap::map(const glm::vec3& vertex)
{
	return convexCombMap.at(vertex);
}

int ConvexCombMap::mapNum()
{
	return convexCombMap.size();
}

namespace
{
	TriMesh::HalfedgeHandle getFirstBoundaryHeH(const TriMesh& mesh)
	{
		std::vector<TriMesh::HalfedgeHandle> candidateHeH;

		// Loop all possible candidate
		for (auto he_it = mesh.halfedges_begin(); he_it != mesh.halfedges_end(); ++he_it)
		{
			if (mesh.is_boundary(*he_it))
			{
				candidateHeH.push_back(*he_it);

			}
		}

		// Choose boundary by max length
		int idx;
		int maxLen = 0;

		for (int iii = 0; iii != candidateHeH.size(); ++iii)
		{
			int length = 0;

			TriMesh::HalfedgeHandle currentHeh = candidateHeH[iii];

			do
			{
				++length;
				currentHeh = mesh.next_halfedge_handle(currentHeh);

			} while (currentHeh != candidateHeH[iii]);

			if (length > maxLen)
			{
				idx = iii;
				maxLen = length;
			}
		}

		return candidateHeH[idx];
	}

	void calcBoundaryLengths(const TriMesh& mesh, const TriMesh::HalfedgeHandle& startBoundaryHeH)
	{
		TriMesh::HalfedgeHandle currentHeH = startBoundaryHeH;

		do
		{
			// Get (u,v)
			TriMesh::VertexHandle fromVH = mesh.from_vertex_handle(currentHeH);
			TriMesh::VertexHandle toVH = mesh.to_vertex_handle(currentHeH);

			// Calculate length
			float length = (mesh.point(toVH) - mesh.point(fromVH)).length();
			boundaryLengthMap.lengths[currentHeH] = length;
			boundaryLengthMap.totalLen += length;

			// To next boundary edge
			currentHeH = mesh.next_halfedge_handle(currentHeH);

		} while (currentHeH != startBoundaryHeH);

	}

	void calcBoundaryTexcoords(const TriMesh& mesh, const TriMesh::HalfedgeHandle& startBoundaryHeH)
	{
		TriMesh::HalfedgeHandle currentHeH = startBoundaryHeH;

		// Set first vertex of boundary to bottom-left texcoord
		TriMesh::Point firstVertex = mesh.point(mesh.from_vertex_handle(currentHeH));
		convexCombMap[Utils::toGlmVec3(firstVertex)] = glm::vec2(0.0f);

		glm::vec2 lastTexCoord = glm::vec2(0.0f);

		int xDir = 1;
		int yDir = 0;

		float sideLength = 0;

		do
		{
			// Ratio of the length in 3D space to texcoord length
			float texcoordLength = boundaryLengthMap.lengths.at(currentHeH) / boundaryLengthMap.totalLen * 4;


			// Single side length range(0, 1)
			sideLength += texcoordLength;

			glm::vec2 texcoord = lastTexCoord;


			// Check if there is a corner, need delta to prevent small value issue
			if (sideLength <= 1.000001f)
			{
				texcoord.x += xDir * texcoordLength;
				texcoord.y += yDir * texcoordLength;
			}
			else
			{
				while (sideLength > 1.000001f)
				{
					sideLength -= 1.0f;

					// Y up
					if (xDir == 1)
					{
						xDir = 0;
						yDir = 1;

						texcoord = glm::vec2(1.0f, sideLength);
					}
					// X left
					else if (yDir == 1)
					{
						xDir = -1;
						yDir = 0;

						texcoord = glm::vec2(1.0f - sideLength, 1.0f);
					}
					// Y down
					else if (xDir == -1)
					{
						xDir = 0;
						yDir = -1;

						texcoord = glm::vec2(0.0f, 1.0f - sideLength);
					}
				}
			}

			// Save to result map, vertex use 'to' here since we handled first vertex as bottom-left corner
			glm::vec3 vertex = Utils::toGlmVec3(mesh.point(mesh.to_vertex_handle(currentHeH)));

			convexCombMap[vertex] = texcoord;

			lastTexCoord = texcoord;

			// To next boundary edge
			currentHeH = mesh.next_halfedge_handle(currentHeH);

		} while (currentHeH != startBoundaryHeH);
	}

	void calcWeights(TriMesh mesh)
	{
		for (auto v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			// Check if is inner vertex
			if (!mesh.is_boundary(*v_it))
			{
				NeighborWeights neighborWeights{ {}, 0.0f };

				// Save inner vertices
				innerVertHandles.push_back(TriMesh::VertexHandle(*v_it));

				// Ignore handle postfix here for simplicity
				for (auto incidentEdge_it = mesh.voh_begin(*v_it); incidentEdge_it != mesh.voh_end(*v_it); ++incidentEdge_it)
				{
					// Handle viNext part
					TriMesh::VertexHandle vi = mesh.to_vertex_handle(*incidentEdge_it);

					TriMesh::HalfedgeHandle  viToviNext = mesh.next_halfedge_handle(*incidentEdge_it);

					TriMesh::VertexHandle viNext = mesh.to_vertex_handle(viToviNext);

					TriMesh::HalfedgeHandle viNextTov0 = mesh.find_halfedge(viNext, *v_it);

					// Calculate gamma
					float gamma = 0.0f;

					// Check if is a valid shape
					if (viNextTov0.is_valid())
					{
						gamma = mesh.calc_sector_angle(viToviNext);
					}
					else
					{
						continue;
					}

					// Handle viPrev part
					TriMesh::HalfedgeHandle viPrevTovi = mesh.prev_halfedge_handle(mesh.opposite_halfedge_handle(*incidentEdge_it));

					TriMesh::VertexHandle viPrev = mesh.from_vertex_handle(viPrevTovi);

					TriMesh::HalfedgeHandle v0ToviPrev = mesh.find_halfedge(*v_it, viPrev);

					// Calculate beta
					float betaPrev = 0.0f;

					// Check if is a valid shape
					if (v0ToviPrev.is_valid())
					{
						betaPrev = mesh.calc_sector_angle(v0ToviPrev);
					}
					else
					{
						continue;
					}

					// Calculate weight
					float weight = glm::cot(gamma) + glm::cot(betaPrev);

					// Save the weight
					neighborWeights.weights[vi] = weight;
					neighborWeights.totalWeight += weight;
				}

				// Save v0's neightbor weights
				neighborWeightMap[*v_it] = neighborWeights;
			}
		}
	}

	void solveLinearSystem(const TriMesh& mesh)
	{
		// Fill matrix A
		Eigen::MatrixXf A(innerVertHandles.size(), innerVertHandles.size());

		for (int row = 0; row < innerVertHandles.size(); ++row)
		{
			auto& currentNeighborWeights = neighborWeightMap[innerVertHandles[row]];

			// Fill matrix A
			for (int col = 0; col < innerVertHandles.size(); ++col)
			{

				if (row == col)
				{
					A(row, col) = currentNeighborWeights.totalWeight;
				}
				else
				{
					A(row, col) = -currentNeighborWeights.weights[innerVertHandles[col]];
				}
			}
		}

		// Fill vector Bs
		Eigen::VectorXf b_x(innerVertHandles.size());
		Eigen::VectorXf b_y(innerVertHandles.size());

		for (int row = 0; row < innerVertHandles.size(); ++row)
		{
			auto& currentNeighborWeights = neighborWeightMap[innerVertHandles[row]];

			float rowXTotal = 0.0f;
			float rowYTotal = 0.0f;

			// Iterate through all neighbor vertices
			for (auto it  = currentNeighborWeights.weights.begin();
					  it != currentNeighborWeights.weights.end()  ;
				    ++it
				)
			{
				TriMesh::VertexHandle vh = (*it).first;
				float weight = (*it).second;

				// Exclude non-boundary vertices
				if (mesh.is_boundary(vh))
				{
					glm::vec2 neghborVertTexcoord = convexCombMap[Utils::toGlmVec3(mesh.point(vh))];

					rowXTotal += weight * neghborVertTexcoord.x;
					rowYTotal += weight * neghborVertTexcoord.y;
				}
			}

			b_x(row) = rowXTotal;
			b_y(row) = rowYTotal;
		}

		// Solve
		Eigen::VectorXf X, Y;
		X = A.lu().solve(b_x);
		Y = A.lu().solve(b_y);

		// Store to result map
		for (int iii = 0; iii != innerVertHandles.size(); ++iii)
		{
			glm::vec2 texcoord(X(iii), Y(iii));

			glm::vec3 vertex = Utils::toGlmVec3(mesh.point(innerVertHandles[iii]));

			convexCombMap[vertex] = texcoord;
		}
	}

}