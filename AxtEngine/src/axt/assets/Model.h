#pragma once

#include <axt/Core.h>

#include <tiny_gltf.h>

#include <axt/platform/OpenGL/GLBuffer.h>
#include <axt/render/VertexArray.h>

namespace axt
{

	// Just gonna use OpenGL bc that's all I use.
	// No point in abstraction anymore

	class AXT_API Model
	{
	public:
		Model(const std::string& filepath);
	protected:
		bool LoadGLTFModel(const std::string& filepath);
		void RegisterSubmesh(tinygltf::Mesh& node);
		void RegisterModel(tinygltf::Node& node);
		void Register();
		tinygltf::Model mModel;

		Ref<VertexArray> mVertexArray;
		//Ref<VertexBuffer> mVertexBuffer;
		//Ref<IndexBuffer> mIndexBuffer;
	};

}
