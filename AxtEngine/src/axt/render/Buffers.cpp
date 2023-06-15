#include <pch.h>

#include "Buffers.h"

#include "axt/render/Renderer.h"

#include "axt/platform/OpenGL/GLBuffer.h"

namespace axt {

	//BufferItem(ShaderDataType t, const std::string& n, bool norm = false, uint32_t count = 0) : name{ n }, type{ t }, size{ GetDataTypeSize(t) }, offset{ 0 }, normalized{ norm }, count{ count } {}

	uint32_t GetRawItemCount(ShaderDataType t)
	{
		switch (t)
		{
			case (ShaderDataType::Float): return 1;
			case(ShaderDataType::Float2): return 2;
			case(ShaderDataType::Float3): return 3;
			case(ShaderDataType::Float4): return 4;

			case(ShaderDataType::Mat3): return 9;
			case(ShaderDataType::Mat4): return 16;

			case(ShaderDataType::Int): return 1;
			case(ShaderDataType::Int2): return 2;
			case(ShaderDataType::Int3): return 3;

			case(ShaderDataType::UInt): return 1;

			case(ShaderDataType::Bool): return 1;
		}
		AXT_CORE_ASSERT(false, "No ShaderDataType found.");
		return 0;
	}

	BufferItem::BufferItem(ShaderDataType t, const std::string& n, bool norm, uint32_t c, uint32_t off) :
		type{ t },
		name{ n },
		normalized{ norm },
		offset{ off },
		count{ c }
	{
		size = count * GetDataTypeSize(t);
	}

	BufferItem::BufferItem(ShaderDataType t, const std::string& n, bool norm) :
		name{ n },
		type{ t },
		size{ GetDataTypeSize(t) },
		offset{ 0 },
		normalized{ false },
		count{ GetRawItemCount(t) }
	{
		
	}

	uint32_t BufferItem::GetItemCount() const 
	{
		return count;
	}

	void BufferLayout::CalculateStride() 
	{
		int currentOffset{ 0 };
		itemStride = 0;
		for (BufferItem& currentItem : mItems) 
		{
			currentItem.offset = currentOffset;
			uint32_t& size{ currentItem.size };
			itemStride += size;
			currentOffset += size;
		}
	}

	void BufferLayout::AddItem(const BufferItem& item, int32_t position)
	{
		if (position >= 0)
		{
			int32_t adjustedPosition{ position + 1 };
			if (mItems.size() <= adjustedPosition)
			{
				mItems.resize(adjustedPosition);
				// Might check for ::reserve() performance
			}
			mItems.at(position) = item;
		}
		else
		{
			mItems.push_back(item);
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) 
	{
		switch (Render3D::GetApi()) 
		{
			case(RenderAPI::API::None): break;
			case(RenderAPI::API::OpenGL): return NewRef<OGLVertexBuffer>(vertices, size);//return (new OGLVertexBuffer{ vertices, size });
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) 
	{
		switch (Render3D::GetApi()) 
		{
			case(RenderAPI::API::None): break;
			case(RenderAPI::API::OpenGL): return NewRef<OGLVertexBuffer>(size);//return (new OGLVertexBuffer{ vertices, size });
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size) 
	{
		switch (Render3D::GetApi()) 
		{
			case(RenderAPI::API::None): break;
			case(RenderAPI::API::OpenGL): return NewRef<OGLIndexBuffer>(indices, size);//return (new OGLIndexBuffer{ indices, size });
		}

		AXT_CORE_ASSERT(false, "No render api found");
		return nullptr;
	}

}
