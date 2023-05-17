#include <pch.h>

#include "Model.h"

#include <glad/glad.h>

namespace axt
{

	Model::Model(const std::string& filepath)
	{
		mVertexArray = VertexArray::Create();
		LoadGLTFModel(filepath);
		Register();
	}

	bool Model::LoadGLTFModel(const std::string& filepath)
	{
		tinygltf::TinyGLTF loader{};
		std::string err;
		std::string warn;

		bool pass{ loader.LoadASCIIFromFile(&mModel, &err, &warn, filepath) };

		if (!warn.empty())
		{
			AXT_WARN("{0} :: {1}", __FUNCTION__, warn);
		}
		if (!err.empty())
		{
			AXT_CORE_ASSERT(false, "{0} :: {1}", __FUNCTION__, err);
		}

		if (!pass)
		{
			AXT_WARN("{0} :: Model failed to load.", __FUNCTION__);
		}
	}

	void Model::RegisterSubmesh(tinygltf::Mesh& mesh)
	{
		for (size_t i{ 0 }; i < mModel.buffers.size(); i++)
		{
			const tinygltf::BufferView& view{ mModel.bufferViews[i] };
			if (!view.target) // == 0
			{
				AXT_WARN("Target is 0");
			}

			const tinygltf::Buffer& buffer{ mModel.buffers[view.buffer] };

			// Creating vertex and index buffers
			Ref<VertexBuffer> vb{ VertexBuffer::Create(buffer.data.size()) };
			vb->SubmitData(&buffer.data, buffer.data.size());

		}
	}

	void Model::RegisterModel(tinygltf::Node& node)
	{
		if (node.mesh >= 0 && node.mesh < mModel.meshes.size())
		{
			RegisterSubmesh(mModel.meshes[node.mesh]);
		}
		for (size_t i{ 0 }; i < node.children.size(); i++)
		{
			RegisterModel(mModel.nodes[node.children[i]]);
		}
	}

	void Model::Register()
	{
		mVertexArray->Bind();
		
		const tinygltf::Scene& scene{ mModel.scenes[mModel.defaultScene] };

		// looping through all scene nodes
		for (size_t i{ 0 }; i < scene.nodes.size(); i++)
		{
			RegisterModel(mModel.nodes[scene.nodes[i]]);
		}

	}

}