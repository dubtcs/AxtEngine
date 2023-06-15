#pragma once

#include <axt/Core.h>

/*
Placeholder class just to get 3D models drawing to screen
*/

#include <axt/render/VertexArray.h>
#include <axt/render/Buffers.h>

#include <vector>

#include <gltf/tiny_gltf.h>

namespace axt
{

	class AXT_API Mesh
	{
	public:
		Mesh(tinygltf::Model& model, tinygltf::Mesh& mesh);
	protected:
		void Init();
	protected:
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
	};

	class AXT_API Model
	{
	public:
		Model(const std::string& filepath);
		void Bind() const; // Bind model for drawing
	protected:
		void Init();
		void CreateNode(tinygltf::Node& node);
		void CreateMesh(tinygltf::Mesh& mesh);
	protected:
		tinygltf::Model mModel;
		std::vector<Mesh> mMeshes;
		Ref<VertexArray> mVertexArray;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
	};

}
