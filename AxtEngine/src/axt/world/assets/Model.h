#pragma once

#include <axt/Core.h>

#include <gltf/tiny_gltf.h>

#include <axt/render/VertexArray.h>
#include <axt/render/Buffers.h>

namespace axt
{

	// Just gonna use OpenGL bc that's all I use.
	// No point in abstraction anymore

	class AXT_API Model
	{
	public:
		Model(const std::string& filepath);
		//using BufferPair = std::pair<Ref<VertexBuffer>, Ref<IndexBuffer>>;
	protected:
		void RegisterModel();
		void RegisterNode(tinygltf::Node& node);
		void RegisterMesh(tinygltf::Mesh& mesh);

		tinygltf::Model mModel;
		Ref<VertexArray> mVertexArray;
		std::vector<Ref<VertexBuffer>> mVertexBuffers;
	};

}
