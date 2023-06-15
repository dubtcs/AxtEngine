#include <pch.h>

#include "OldWorldModel.h"

namespace axt
{
	using attribute_info = std::pair<int32_t, ShaderDataType>;
	attribute_info GetAttributeIndex(const std::string& attributeName)
	{
		if (attributeName == "POSITION")
			return { 0, ShaderDataType::Float3 };
		else if (attributeName == "NORMAL")
			return { 1, ShaderDataType::Float3 };
		else if (attributeName == "TEXCOORD_0")
			return { 2, ShaderDataType::Float2 };
		return { -1, ShaderDataType::None };
	}

	// Mesh
	static uint32_t foing{ 0 };
	Mesh::Mesh(tinygltf::Model& model, tinygltf::Mesh& mesh)
	{
		AXT_INFO("Creating Mesh");

		for (tinygltf::Primitive& primitive : mesh.primitives)
		{
			tinygltf::Accessor& indexAccessor{ model.accessors[primitive.indices] };
			for (auto& attribute : primitive.attributes)
			{
				tinygltf::Accessor& accessor{ model.accessors[attribute.second] };
				Ref<VertexBuffer> vBuffer{ mVertexBuffers[accessor.bufferView] };

				vBuffer->Bind();
				attribute_info info{ GetAttributeIndex(attribute.first) };
				vBuffer->GetLayout().AddItem({ info.second, "V_POSITION", accessor.normalized }, info.first);
			}
		}

	}

	// Model

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
		Init();
		AXT_TRACE("Model Loaded");
	}

	void Model::Init()
	{
		mVertexArray = VertexArray::Create();
		mVertexArray->Bind();
		const tinygltf::Scene& scene{ mModel.scenes[mModel.defaultScene] };

		for (tinygltf::BufferView& view : mModel.bufferViews)
		{
			const tinygltf::Buffer& buffer{ mModel.buffers[view.buffer] };
			// assuming contiguous
			Ref<VertexBuffer> vBuffer{ VertexBuffer::Create(view.byteLength) };
			vBuffer->SubmitData(&buffer.data.at(0) + view.byteOffset, view.byteLength);
			mVertexBuffers.push_back(vBuffer);
		}

		for (const int32_t& i : scene.nodes)
		{
			AXT_TRACE("Node {0}", scene.nodes[i]);
			CreateNode(mModel.nodes[i]);
		}

	}

	void Model::CreateNode(tinygltf::Node& node)
	{
		if (node.mesh >= 0)
		{
			CreateMesh(mModel.meshes[node.mesh]);
		}
		for (int32_t& i : node.children)
		{
			CreateNode(mModel.nodes[i]);
		}
	}

	void Model::CreateMesh(tinygltf::Mesh& mesh)
	{
		mMeshes.push_back({ mModel, mesh });
	}

}
