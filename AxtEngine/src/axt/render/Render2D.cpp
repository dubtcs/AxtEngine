#include <pch.h>

#include "Render2D.h"

#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>

// 32 is usually the most for desktop gpus
constexpr int MAX_TEXTURE_UNITS = 32;

namespace axt {

	struct QuadVert {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinate;
		float textureId = 0;
	};

	struct Render2DScene {
		Ref<VertexArray> mVertexArray;
		Ref<VertexBuffer> mVertexBuffer;
		Ref<Shader> mShader;
		Ref<Texture2D> mWhiteTexture;

		uint32_t mIndexCount{ 0 };
		const uint32_t mMaxQuads{ 10000 };
		const uint32_t mMaxVertices{ mMaxQuads * 4 };
		const uint32_t mMaxIndices{ mMaxQuads * 6 };

		QuadVert* mQuadStart{ nullptr }; // starting point of quad array
		QuadVert* mCurrentQuadVertex{ nullptr }; // current index in scene

		std::array<Ref<Texture2D>, MAX_TEXTURE_UNITS> mTextureArray;
		int mTexturesUsed{ 1 };
		TextureLib mTextureLibrary;
	};

	static Render2DScene* sData;

	/*
	Batch render:
	Create VertexArray
	Create VertexBuffer with max storage
	Bufferlayout based on QuadVert
	Index buffer

	with textures:
	create a texture unit array, usually 32 units
	slot 0 is the white texture
	every time a new quad is submitted, check if the texture is already in the array
	if not, add it
	strip the textures at the end of the scene and reset slot 0 to the white texture
	add a sampler2D array to the frag shader
		need to add an array overload to shader::set();
	*/

	void Render2D::Init() {
		AXT_PROFILE_FUNCTION();

		sData = new Render2DScene{};
		sData->mVertexArray = VertexArray::Create();
		sData->mVertexBuffer = VertexBuffer::Create(sData->mMaxVertices * sizeof(QuadVert));

		// QuadVert structure
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
			{axt::ShaderDataType::Float2, "inTexPos"},
			{axt::ShaderDataType::Float, "inTexId"}
		};
		sData->mVertexBuffer->SetLayout(lBufferLayout);
		sData->mVertexArray->AddVertexBuffer(sData->mVertexBuffer);

		sData->mQuadStart = new QuadVert[sData->mMaxQuads];

		Unique<uint32_t[]> fIndexData{ NewUnique<uint32_t[]>(sData->mMaxIndices) };
		uint32_t fIndexOffset{ 0 };
		for (uint32_t i{ 0 }; i < sData->mMaxIndices; i += 6) {
			fIndexData[i] = fIndexOffset;
			fIndexData[i + 1] = fIndexOffset + 1;
			fIndexData[i + 2] = fIndexOffset + 2;

			fIndexData[i + 3] = fIndexOffset + 2;
			fIndexData[i + 4] = fIndexOffset + 3;
			fIndexData[i + 5] = fIndexOffset + 0;

			fIndexOffset += 4;
		}
		Ref<IndexBuffer> fIndexBuffer{ IndexBuffer::Create(fIndexData.get(), sData->mMaxIndices)};
		sData->mVertexArray->AddIndexBuffer(fIndexBuffer);

		//sData->mShader = axt::Shader::Create("Shader1", "shaders/bruh.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);
		sData->mShader = axt::Shader::Create("Shader1", "shaders/aio2_vp.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);

		// creating a white texture
		sData->mWhiteTexture = Texture2D::Create(1, 1);
		uint32_t fWhite{ 0xffffffff }; // 1 in all 4 channels
		sData->mWhiteTexture->SetData(&fWhite, sizeof(uint32_t));

		sData->mTextureArray[0] = sData->mWhiteTexture;
		sData->mTextureLibrary.Add("White", sData->mWhiteTexture);
		Ref<Texture2D> fBruh{ Texture2D::Create("textures/si.png") };
		sData->mTextureLibrary.Add("Bruh", fBruh);
	}

	void Render2D::Shutdown() {

		delete sData;
	}

	void Render2D::SceneStart(const OrthoCamera& camera) {
		AXT_PROFILE_FUNCTION();
		sData->mCurrentQuadVertex = sData->mQuadStart;
		sData->mIndexCount = 0;

		sData->mShader->Bind();
		sData->mShader->SetValue("uViewProjection", camera.GetViewProjection());

		int fShaderSampler2D[MAX_TEXTURE_UNITS];
		for (int i{ 0 }; i < MAX_TEXTURE_UNITS; i++) {
			fShaderSampler2D[i] = i;
		}
		sData->mShader->SetValue("uTextures", fShaderSampler2D, MAX_TEXTURE_UNITS);

		sData->mTexturesUsed = 1;
		sData->mTextureArray[0] = sData->mWhiteTexture; // always set 0 to default texture
	}

	void Render2D::SceneEnd() {
		AXT_PROFILE_FUNCTION();

		uint32_t fDataSize{ static_cast<uint32_t>((char*)sData->mCurrentQuadVertex - (char*)sData->mQuadStart) };
		sData->mVertexBuffer->SubmitData(sData->mQuadStart, fDataSize);

		Flush();
	}

	void Render2D::Flush() {

		// loop through textures, bind them to their slot (0-31)
		for (int i{ 0 }; i < sData->mTexturesUsed; i++) {
			AXT_CORE_TRACE("TEX HEIGHT: {0}", (sData->mTextureArray[i]->GetHeight()));
			sData->mTextureArray[i]->Bind(i);
		}

		AXT_CORE_TRACE("{0} -> Textures used: {1}", __FUNCSIG__, sData->mTexturesUsed);

		RenderCommand::DrawIndexed(sData->mVertexArray, sData->mIndexCount);
	}

	void Render2D::DrawQuad(const QuadProperties&& fQuad) {
		AXT_PROFILE_FUNCTION();

		if (sData->mIndexCount + 4 > sData->mMaxQuads) {
			AXT_CORE_ASSERT(false, "Render2D::DrawQuad QuadAmount limit overflow!");
			return;
		}

		float fTexId{ 0 };
		if (sData->mTextureLibrary.Contains(fQuad.texName)) {
			bool fTexFound{ false };
			const Ref<Texture>& fTextureRequest{ sData->mTextureLibrary.Get(fQuad.texName) };
			// check if the texture is in the used textures array
			for (int i{ 0 }; i < sData->mTexturesUsed; i++) {
				if (sData->mTextureArray[i].get() == fTextureRequest.get()) {
					fTexId = static_cast<float>(i);
					fTexFound = true;
					break;
				}
			}
			// requested texture exists but isn't loaded
			if (!fTexFound) {
				sData->mTextureArray[sData->mTexturesUsed++] = sData->mTextureLibrary.Get(fQuad.texName);
				fTexId = static_cast<float>(sData->mTexturesUsed - 1);
			}
		}
		// if the texture doesn't exits it just uses index 0 - the white texture

		// bottom left
		sData->mCurrentQuadVertex->position = fQuad.position;
		sData->mCurrentQuadVertex->color = fQuad.color;
		sData->mCurrentQuadVertex->textureCoordinate = { 0.f, 0.f };
		sData->mCurrentQuadVertex->textureId = fTexId;
		sData->mCurrentQuadVertex++;

		// bottom right
		sData->mCurrentQuadVertex->position = { fQuad.position.x + fQuad.size.x, fQuad.position.y, 0.f };
		sData->mCurrentQuadVertex->color = fQuad.color;
		sData->mCurrentQuadVertex->textureCoordinate = { 1.f, 0.f };
		sData->mCurrentQuadVertex->textureId = fTexId;
		sData->mCurrentQuadVertex++;

		//top right
		sData->mCurrentQuadVertex->position = { fQuad.position.x + fQuad.size.x, fQuad.position.y + fQuad.size.y, 0.f };
		sData->mCurrentQuadVertex->color = fQuad.color;
		sData->mCurrentQuadVertex->textureCoordinate = { 1.f, 1.f };
		sData->mCurrentQuadVertex->textureId = fTexId;
		sData->mCurrentQuadVertex++;

		// top left
		sData->mCurrentQuadVertex->position = { fQuad.position.x, fQuad.position.y + fQuad.size.y, 0.f };
		sData->mCurrentQuadVertex->color = fQuad.color;
		sData->mCurrentQuadVertex->textureCoordinate = { 0.f, 1.f };
		sData->mCurrentQuadVertex->textureId = fTexId;
		sData->mCurrentQuadVertex++;

		sData->mIndexCount += 6;
	}

}