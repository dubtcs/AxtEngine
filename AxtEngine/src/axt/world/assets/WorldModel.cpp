#include <pch.h>

#include "WorldModel.h"

#include <glad/glad.h>

namespace axt
{

	ShaderDataType GLTypeToShaderDataType(const uint32_t& gl)
	{
		if (gl == GL_FLOAT)
			return ShaderDataType::Float;
		else if (gl == GL_INT)
			return ShaderDataType::Int;
		else if (gl == GL_UNSIGNED_INT)
			return ShaderDataType::UInt;
		else if (gl == GL_BOOL)
			return ShaderDataType::Bool;
		return ShaderDataType::None;
	}

	Model::Model(const std::string& filepath)
	{
		tinygltf::TinyGLTF loader;
		std::string msgError;
		std::string msgWarning;
		loader.LoadASCIIFromFile(&mModel, &msgError, &msgWarning, filepath);
		AXT_INFO("Loading model <{0}>", filepath);
		if (msgWarning.size() || msgError.size())
		{
			AXT_WARN("\tWarnings: {0}\n\tErrors: {1}", (msgWarning.size() ? msgWarning : "NONE"), (msgError.size() ? msgError : "NONE"));
		}

		Init();
	}

	void Model::Init()
	{
		// Create Buffers
		for (tinygltf::BufferView& view : mModel.bufferViews)
		{
			const tinygltf::Buffer& buffer{ mModel.buffers[view.buffer] };
			Ref<VertexBuffer> vBuffer{ VertexBuffer::Create(view.byteLength) };
			vBuffer->SubmitData(&buffer.data.at(0) + view.byteOffset, view.byteLength);
			mVertexBuffers.push_back(vBuffer);
		}

		for (const tinygltf::Mesh& mesh : mModel.meshes)
		{
			ProcessMesh(mesh);
		}
	}

	void Model::ProcessNode(const tinygltf::Node& node)
	{
		
	}

	void Model::ProcessMesh(const tinygltf::Mesh& mesh)
	{
		for (const tinygltf::Primitive& primitive : mesh.primitives)
		{
			for (const auto& attribute : primitive.attributes)
			{
				const tinygltf::Accessor& accessor{ mModel.accessors[attribute.second] };
				const Ref<VertexBuffer>& vBuffer{ mVertexBuffers.at(accessor.bufferView) };
				int32_t count{ accessor.type == TINYGLTF_TYPE_SCALAR ? 1 : accessor.type };
				BufferItem item{ GLTypeToShaderDataType(accessor.componentType), "ITEM_NAME", accessor.normalized, static_cast<uint32_t>(count), static_cast<uint32_t>(accessor.byteOffset) };
				vBuffer->Bind();
				vBuffer->SetLayout({ item });

				// DAMN
				// This might not be useful. Accessors might have different contexts to the same buffer, so setting the layout here might be overriding others that don't use the same layout
			}
		}
	}

}