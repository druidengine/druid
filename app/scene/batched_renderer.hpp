#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <vector>

// True batched renderer that combines all instances into a single mesh
// This gives us ONE draw call per renderer instead of thousands!
class InstancedRenderer
{
private:
	Mesh baseMesh;
	Material material;
	std::vector<Matrix> transforms;

	// Batched mesh that combines all instances into one mesh
	Mesh batchedMesh;
	bool batchedMeshCreated = false;

public:
	InstancedRenderer(Mesh mesh, Material mat) : baseMesh(mesh), material(mat)
	{
		// Initialize batched mesh
		batchedMesh = {0};
	}

	~InstancedRenderer()
	{
		if (batchedMeshCreated)
		{
			UnloadMesh(batchedMesh);
		}
	}

	void AddInstance(Vector3 position, Vector3 rotation, Vector3 scale)
	{
		// Create transform matrix: Scale -> Rotate -> Translate
		Matrix scaleMatrix = MatrixScale(scale.x, scale.y, scale.z);
		Matrix rotateMatrix = MatrixRotateXYZ(rotation);
		Matrix translateMatrix = MatrixTranslate(position.x, position.y, position.z);

		Matrix transform = MatrixMultiply(MatrixMultiply(scaleMatrix, rotateMatrix), translateMatrix);
		transforms.push_back(transform);

		// Mark batched mesh as outdated
		batchedMeshCreated = false;
	}

	// Update a specific instance transform and upload only that part
	bool UpdateInstance(int instanceIndex, Vector3 position, Vector3 rotation, Vector3 scale)
	{
		if (instanceIndex < 0 || instanceIndex >= static_cast<int>(transforms.size()))
		{
			return false; // Invalid index
		}

		// Create new transform matrix
		Matrix scaleMatrix = MatrixScale(scale.x, scale.y, scale.z);
		Matrix rotateMatrix = MatrixRotateXYZ(rotation);
		Matrix translateMatrix = MatrixTranslate(position.x, position.y, position.z);
		Matrix newTransform = MatrixMultiply(MatrixMultiply(scaleMatrix, rotateMatrix), translateMatrix);

		// Update the stored transform
		transforms[instanceIndex] = newTransform;

		// If batched mesh exists, update just this instance's vertices
		if (batchedMeshCreated && batchedMesh.vertices && batchedMesh.normals)
		{
			UpdateInstanceVertices(instanceIndex, newTransform);
			return true;
		}

		// If no batched mesh exists yet, just mark for rebuild
		batchedMeshCreated = false;
		return true;
	}

	void ClearInstances()
	{
		transforms.clear();
		if (batchedMeshCreated)
		{
			UnloadMesh(batchedMesh);
			batchedMeshCreated = false;
		}
	}

private:
	// Update vertices for a specific instance in the batched mesh
	void UpdateInstanceVertices(int instanceIndex, const Matrix& transform)
	{
		if (!batchedMeshCreated || !batchedMesh.vertices || !batchedMesh.normals)
		{
			return;
		}

		int vertexOffset = instanceIndex * baseMesh.vertexCount;

		// Transform each vertex of this instance
		for (int v = 0; v < baseMesh.vertexCount; v++)
		{
			int srcIndex = v;
			int dstIndex = vertexOffset + v;

			// Transform position
			Vector3 pos = {baseMesh.vertices[srcIndex * 3 + 0], baseMesh.vertices[srcIndex * 3 + 1], baseMesh.vertices[srcIndex * 3 + 2]};
			pos = Vector3Transform(pos, transform);

			batchedMesh.vertices[dstIndex * 3 + 0] = pos.x;
			batchedMesh.vertices[dstIndex * 3 + 1] = pos.y;
			batchedMesh.vertices[dstIndex * 3 + 2] = pos.z;

			// Transform normal correctly for lighting
			Vector3 normal = {baseMesh.normals[srcIndex * 3 + 0], baseMesh.normals[srcIndex * 3 + 1], baseMesh.normals[srcIndex * 3 + 2]};

			// For normals, we need the inverse transpose of the rotation/scale part only (no translation)
			Matrix normalTransform = transform;
			normalTransform.m12 = 0.0f; // Remove translation
			normalTransform.m13 = 0.0f;
			normalTransform.m14 = 0.0f;

			// Apply inverse transpose for proper normal transformation
			normalTransform = MatrixTranspose(MatrixInvert(normalTransform));
			normal = Vector3Transform(normal, normalTransform);
			normal = Vector3Normalize(normal);

			batchedMesh.normals[dstIndex * 3 + 0] = normal.x;
			batchedMesh.normals[dstIndex * 3 + 1] = normal.y;
			batchedMesh.normals[dstIndex * 3 + 2] = normal.z;

			// Texture coordinates don't change
		}

		// Update the GPU buffer with the new vertex data
		// This updates only the vertex positions and normals for this instance
		UpdateMeshBuffer(batchedMesh, 0, batchedMesh.vertices, batchedMesh.vertexCount * 3 * sizeof(float), 0);
		UpdateMeshBuffer(batchedMesh, 2, batchedMesh.normals, batchedMesh.vertexCount * 3 * sizeof(float), 0);
	}

public:
	// Create a single mesh that contains all instances combined
	void CreateBatchedMesh()
	{
		if (transforms.empty() || batchedMeshCreated)
			return;

		printf("Creating batched mesh with %zu instances...\n", transforms.size());

		// Calculate total vertices needed
		int instanceCount = static_cast<int>(transforms.size());
		int totalVertices = baseMesh.vertexCount * instanceCount;

		// Allocate memory for combined mesh
		batchedMesh.vertexCount = totalVertices;
		batchedMesh.triangleCount = baseMesh.triangleCount * instanceCount;
		batchedMesh.vertices = static_cast<float*>(RL_MALLOC(totalVertices * 3 * sizeof(float)));
		batchedMesh.normals = static_cast<float*>(RL_MALLOC(totalVertices * 3 * sizeof(float)));

		if (baseMesh.texcoords)
		{
			batchedMesh.texcoords = static_cast<float*>(RL_MALLOC(totalVertices * 2 * sizeof(float)));
		}

		// Copy and transform vertices for each instance
		for (int i = 0; i < instanceCount; i++)
		{
			Matrix transform = transforms[i];
			int vertexOffset = i * baseMesh.vertexCount;

			// Transform each vertex of the base mesh
			for (int v = 0; v < baseMesh.vertexCount; v++)
			{
				int srcIndex = v;
				int dstIndex = vertexOffset + v;

				// Transform position
				Vector3 pos = {baseMesh.vertices[srcIndex * 3 + 0], baseMesh.vertices[srcIndex * 3 + 1], baseMesh.vertices[srcIndex * 3 + 2]};
				pos = Vector3Transform(pos, transform);

				batchedMesh.vertices[dstIndex * 3 + 0] = pos.x;
				batchedMesh.vertices[dstIndex * 3 + 1] = pos.y;
				batchedMesh.vertices[dstIndex * 3 + 2] = pos.z;

				// Transform normal correctly for lighting
				Vector3 normal = {baseMesh.normals[srcIndex * 3 + 0], baseMesh.normals[srcIndex * 3 + 1], baseMesh.normals[srcIndex * 3 + 2]};

				// For normals, we need the inverse transpose of the rotation/scale part only (no translation)
				// Extract rotation and scale from transform matrix
				Matrix normalTransform = transform;
				normalTransform.m12 = 0.0f; // Remove translation
				normalTransform.m13 = 0.0f;
				normalTransform.m14 = 0.0f;

				// Apply inverse transpose for proper normal transformation
				normalTransform = MatrixTranspose(MatrixInvert(normalTransform));
				normal = Vector3Transform(normal, normalTransform);
				normal = Vector3Normalize(normal);

				batchedMesh.normals[dstIndex * 3 + 0] = normal.x;
				batchedMesh.normals[dstIndex * 3 + 1] = normal.y;
				batchedMesh.normals[dstIndex * 3 + 2] = normal.z;

				// Copy texture coordinates (unchanged)
				if (baseMesh.texcoords && batchedMesh.texcoords)
				{
					batchedMesh.texcoords[dstIndex * 2 + 0] = baseMesh.texcoords[srcIndex * 2 + 0];
					batchedMesh.texcoords[dstIndex * 2 + 1] = baseMesh.texcoords[srcIndex * 2 + 1];
				}
			}
		}

		// Copy indices (adjusted for multiple instances)
		if (baseMesh.indices)
		{
			int totalIndices = baseMesh.triangleCount * 3 * instanceCount;
			batchedMesh.indices = static_cast<unsigned short*>(RL_MALLOC(totalIndices * sizeof(unsigned short)));

			for (int i = 0; i < instanceCount; i++)
			{
				int vertexOffset = i * baseMesh.vertexCount;
				int indexOffset = i * baseMesh.triangleCount * 3;

				for (int j = 0; j < baseMesh.triangleCount * 3; j++)
				{
					batchedMesh.indices[indexOffset + j] = baseMesh.indices[j] + vertexOffset;
				}
			}
		}

		// Upload to GPU
		UploadMesh(&batchedMesh, false);
		batchedMeshCreated = true;

		printf("Batched mesh created successfully with %d vertices!\n", totalVertices);
	}

	void DrawInstanced()
	{
		if (!transforms.empty())
		{
			// Debug: Print first time we draw
			static bool first_draw = true;
			if (first_draw)
			{
				printf("TRUE BATCHING: Combining %zu instances into 1 draw call!\n", transforms.size());
				first_draw = false;
			}

			// Create batched mesh if needed
			CreateBatchedMesh();

			// Draw the entire batched mesh in ONE draw call!
			if (batchedMeshCreated)
			{
				DrawMesh(batchedMesh, material, MatrixIdentity());
			}
		}
	}

	size_t GetInstanceCount() const
	{
		return transforms.size();
	}
};