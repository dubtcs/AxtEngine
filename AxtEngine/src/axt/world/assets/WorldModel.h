#pragma once

#include <axt/Core.h>

#include <axt/render/VertexArray.h>
#include <axt/render/Buffers.h>

#include <vector>
#include <string>

#include <gltf/tiny_gltf.h>

namespace axt
{

	class AXT_API Mesh
	{
	public:
		Mesh();
	protected:
		
	};

	class AXT_API Model
	{
	public:
		Model(const std::string& filepath);
	protected:
		void Init();
		void ProcessNode(const tinygltf::Node& node);
		void ProcessMesh(const tinygltf::Mesh& mesh);
	protected:
		tinygltf::Model mModel;
		Ref<VertexArray> mVertexArray;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
		std::vector<Mesh> mMeshes;
	};

}
