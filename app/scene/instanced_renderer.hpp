#pragma once

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <vector>

// Simpler approach using DrawMeshInstanced
class InstancedRenderer
{
private:
	Mesh baseMesh;
	Material material;
	std::vector<Matrix> transforms;

public:
	InstancedRenderer(Mesh mesh, Material mat) : baseMesh(mesh), material(mat)
	{
	}

	void AddInstance(Vector3 position, Vector3 rotation, Vector3 scale)
	{
		// Fix matrix multiplication order: Scale -> Rotate -> Translate
		Matrix scaleMatrix = MatrixScale(scale.x, scale.y, scale.z);
		Matrix rotateMatrix = MatrixRotateXYZ(rotation);
		Matrix translateMatrix = MatrixTranslate(position.x, position.y, position.z);

		// Correct order: translate * rotate * scale
		Matrix transform = MatrixMultiply(MatrixMultiply(scaleMatrix, rotateMatrix), translateMatrix);

		transforms.push_back(transform);
	}

	void ClearInstances()
	{
		transforms.clear();
	}

	void DrawInstanced()
	{
		if (!transforms.empty())
		{
			// Try TRUE GPU instancing first
			DrawMeshInstanced(baseMesh, material, transforms.data(), static_cast<int>(transforms.size()));
		}
	}
	size_t GetInstanceCount() const
	{
		return transforms.size();
	}
};