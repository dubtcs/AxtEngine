#pragma once

#include "axt/Core.h"

namespace axt {

	enum class ShaderDataType 
	{
		None = 0, 
		Float, Float2, Float3, Float4, 
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		UInt,
		Bool
	};

	static uint32_t GetDataTypeSize(ShaderDataType type) 
	{
		switch (type) 
		{
			case(ShaderDataType::Float):return 4;
			case(ShaderDataType::Float2):return 8;
			case(ShaderDataType::Float3):return 12;
			case(ShaderDataType::Float4):return 16;

			case(ShaderDataType::Mat3):return 36;
			case(ShaderDataType::Mat4):return 64;

			case(ShaderDataType::Int):return 4;
			case(ShaderDataType::Int2):return 8;
			case(ShaderDataType::Int3):return 12;
			case(ShaderDataType::Int4):return 16;

			case(ShaderDataType::UInt):return 4;

			case(ShaderDataType::Bool):return 1;
		}

		AXT_CORE_ASSERT(false, "No ShaderDataType found");
		return 0;
	}

	struct AXT_API BufferItem 
	{
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;
		BufferItem() : type{ ShaderDataType::None } {} // This set to none should cause errors
		BufferItem(ShaderDataType t, const std::string& n, bool norm = false) : name{ n }, type{ t }, size{ GetDataTypeSize(t) }, offset{ 0 }, normalized{ norm } {}
		//BufferItem(GLenum type, const std::string& name, bool normalizaed = false) : name{name}
		uint32_t GetItemCount() const;
	};

	class AXT_API BufferLayout 
	{
	public:
		BufferLayout(const std::initializer_list<BufferItem>& newItems) : mItems{ newItems } { CalculateStride(); };
		//BufferLayout(const std::vector<BufferItem>& itemVector) : items{ itemVector } { CalculateStride(); }
	public:
		const std::vector<BufferItem>& GetItems() const { return mItems; }
		const uint32_t GetStride() const { return itemStride; }
		void AddItem(const BufferItem& item, int32_t position = -1);

		std::vector<BufferItem>::iterator begin() { return mItems.begin(); }
		std::vector<BufferItem>::iterator end() { return mItems.end(); }
		std::vector<BufferItem>::const_iterator begin() const { return mItems.begin(); }
		std::vector<BufferItem>::const_iterator end() const { return mItems.end(); }
	private:
		void CalculateStride();
	private:
		std::vector<BufferItem> mItems;
		uint32_t itemStride{ 0 };
	};

	class AXT_API VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() {};
		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	public:
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetLayout(const BufferLayout& newLayout) = 0;
		virtual BufferLayout& GetLayout() = 0;
		virtual void SubmitData(const void* data, uint32_t size, uint32_t offset = 0) const = 0;
	};

	class AXT_API IndexBuffer 
	{
	public:
		virtual ~IndexBuffer() {};
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t size);
	public:
		virtual uint32_t GetCount() const = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	};

}
