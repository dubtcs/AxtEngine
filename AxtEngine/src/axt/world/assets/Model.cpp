#include <pch.h>

#include "Model.h"

#include <axt/render/Buffers.h>

#include <gltf/tiny_gltf.h>
#include <glad/glad.h>

static const axt::BufferLayout GVertexBufferLayout{
	{axt::ShaderDataType::Float3, "inPos"},
	{axt::ShaderDataType::Float4, "inColor"},
	{axt::ShaderDataType::Float2, "inTexPos"},
};

namespace axt
{

	static int32_t GetAttributeIndex(const std::string& attributeName)
	{
		if (attributeName == "POSITION")
			return 0;
		else if (attributeName == "NORMAL")
			return 1;
		else if (attributeName == "TEXCOORD_0")
			return 2;
		return -1;
	}

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
		mVertexBuffers.resize(mModel.bufferViews.size());

		const tinygltf::Scene& scene{ mModel.scenes[mModel.defaultScene] };
		for (const int32_t& i : scene.nodes)
		{
			RegisterNode(mModel.nodes[i]);
		}
		AXT_TRACE("Total BufferViews: {0}", mModel.bufferViews.size());
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
		AXT_INFO(mesh.name);

		for (tinygltf::BufferView& view : mModel.bufferViews)
		{
			const tinygltf::Buffer& buffer{ mModel.buffers[view.buffer] };
			Ref<VertexBuffer> vBuffer{ VertexBuffer::Create(view.byteLength) };
			vBuffer->SetLayout(GVertexBufferLayout);
			AXT_TRACE("VBuffer bytelength: {0}", view.byteLength);
			mVertexBuffers.at(view.buffer) = vBuffer;
			
			// need to add glTarget as a parameter
			vBuffer->SubmitData(&buffer.data.at(0) + view.byteOffset, view.byteLength);
			mVertexBuffers.push_back(vBuffer);
		}

		/*for (int32_t i{ 0 }; i < mModel.bufferViews.size(); i++)
		{
			tinygltf::BufferView& view{ mModel.bufferViews[i] };

		}*/

		//AXT_TRACE(mesh.primitives.size());

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& indexAccessor{ mModel.accessors[primitive.indices] };
			for (auto& attribute : primitive.attributes)
			{
				tinygltf::Accessor& accessor{ mModel.accessors[attribute.second] };
				int32_t stride{ accessor.ByteStride(mModel.bufferViews[accessor.bufferView]) };
				//AXT_TRACE("BufferView (index, bytelength): ({0}, {1})", accessor.bufferView, mModel.bufferViews[accessor.bufferView].byteLength);

				int32_t attributeIndex{ GetAttributeIndex(attribute.first) };
				
				AXT_TRACE("Attribute Info:\n\tComponent Type: {0}", accessor.componentType);

				BufferItem item{ ShaderDataType::Float, attribute.first, accessor.normalized };

			}
		}
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