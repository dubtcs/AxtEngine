#include <pch.h>

#include "Model.h"

#include <gltf/tiny_gltf.h>
#include <glad/glad.h>

namespace axt
{

	Model::Model(const std::string& filepath)
	{
		tinygltf::TinyGLTF loader;
		std::string messageError;
		std::string messageWarning;
		
		bool pass{ loader.LoadASCIIFromFile(&mModel, &messageWarning, &messageWarning, filepath) };
		messageError = (messageError.empty() ? "NONE" : messageError);
		messageWarning = (messageWarning.empty() ? "NONE" : messageWarning);
		if (pass)
		{
			AXT_INFO("Model [{0}] loaded.\n\tWarnings: {1}\n\tErrors: {2}", filepath, messageWarning, messageError);
		}
		else
		{
			AXT_WARN("Model [{0}] NOT loaded.\n\tWarnings: {1}\n\tErrors: {2}", filepath, messageWarning, messageError);
		}
		RegisterModel();
	}

	void Model::RegisterModel()
	{
		mVertexArray = VertexArray::Create();
		const tinygltf::Scene& scene{ mModel.scenes[mModel.defaultScene] };
		for (const int32_t& i : scene.nodes)
		{
			RegisterNode(mModel.nodes[i]);
		}
	}

	void Model::RegisterNode(tinygltf::Node& node)
	{
		if ((node.mesh >= 0) && (node.mesh < mModel.meshes.size()))
		{
			RegisterMesh(mModel.meshes[node.mesh]);
		}
		for (int32_t& i : node.children)
		{
			RegisterNode(mModel.nodes[i]);
		}
	}

	void Model::RegisterMesh(tinygltf::Mesh& mesh)
	{
		
	}

}

/*

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

		return pass;
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

			// Creating vertex buffers
			Ref<VertexBuffer> vb{ VertexBuffer::Create(static_cast<uint32_t>(view.byteLength)) };
			vb->SubmitData(&buffer.data + view.byteOffset, static_cast<uint32_t>(view.byteLength));

			// Adding the buffer to the vector
			mBuffers.push_back({ vb, nullptr });
		}

		using str_i32 = std::pair<std::string, int32_t>;

		// indices
		for (size_t i{ 0 }; i < mesh.primitives.size(); i++)
		{
			tinygltf::Primitive& primitive{ mesh.primitives[i] };
			tinygltf::Accessor& accessorMajor{ mModel.accessors[primitive.indices] };

			for (const str_i32& attribute : primitive.attributes)
			{
				tinygltf::Accessor& accessorMinor{ mModel.accessors[attribute.second] };
				int32_t stride{ accessorMinor.ByteStride(mModel.bufferViews[accessorMinor.bufferView]) };



				//Ref<VertexBuffer> vBuffer{VertexBuffer::Create()}
				//Ref<IndexBuffer> indexBuffer{ IndexBuffer::Create() };
			}
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

*/