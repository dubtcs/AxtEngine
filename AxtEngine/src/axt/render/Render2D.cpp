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

	static Render2DScene* gData;

	static const glm::vec4 gQuadPositions[4]{
		glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f },
		glm::vec4{  0.5f, -0.5f, 0.0f, 1.f },
		glm::vec4{  0.5f,  0.5f, 0.0f, 1.f },
		glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f },
	};

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

		gData = new Render2DScene{};
		gData->mVertexArray = VertexArray::Create();
		gData->mVertexBuffer = VertexBuffer::Create(gData->mMaxVertices * sizeof(QuadVert));

		// QuadVert structure
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
			{axt::ShaderDataType::Float2, "inTexPos"},
			{axt::ShaderDataType::Float, "inTexId"}
		};
		gData->mVertexBuffer->SetLayout(lBufferLayout);
		gData->mVertexArray->AddVertexBuffer(gData->mVertexBuffer);

		gData->mQuadStart = new QuadVert[gData->mMaxQuads];

		Unique<uint32_t[]> fIndexData{ NewUnique<uint32_t[]>(gData->mMaxIndices) };
		uint32_t fIndexOffset{ 0 };
		for (uint32_t i{ 0 }; i < gData->mMaxIndices; i += 6) {
			fIndexData[i] = fIndexOffset;
			fIndexData[i + 1] = fIndexOffset + 1;
			fIndexData[i + 2] = fIndexOffset + 2;

			fIndexData[i + 3] = fIndexOffset + 2;
			fIndexData[i + 4] = fIndexOffset + 3;
			fIndexData[i + 5] = fIndexOffset + 0;

			fIndexOffset += 4;
		}
		Ref<IndexBuffer> fIndexBuffer{ IndexBuffer::Create(fIndexData.get(), gData->mMaxIndices)};
		gData->mVertexArray->AddIndexBuffer(fIndexBuffer);

		//gData->mShader = axt::Shader::Create("Shader1", "shaders/bruh.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);
		gData->mShader = axt::Shader::Create("Shader1", "shaders/aio2_vp.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);

		// creating a white texture
		gData->mWhiteTexture = Texture2D::Create(1, 1);
		uint32_t fWhite{ 0xffffffff }; // 1 in all 4 channels
		gData->mWhiteTexture->SetData(&fWhite, sizeof(uint32_t));

		gData->mTextureArray[0] = gData->mWhiteTexture;
		gData->mTextureLibrary.Add("White", gData->mWhiteTexture);
		Ref<Texture2D> fBruh{ Texture2D::Create("textures/si.png") };
		gData->mTextureLibrary.Add("Bruh", fBruh);
	}

	void Render2D::Shutdown() {

		delete gData;
	}

	void Render2D::SceneStart(const OrthoCamera& camera) {
		AXT_PROFILE_FUNCTION();
		gData->mCurrentQuadVertex = gData->mQuadStart;
		gData->mIndexCount = 0;

		gData->mShader->Bind();
		gData->mShader->SetValue("uViewProjection", camera.GetViewProjection());

		int fShaderSampler2D[MAX_TEXTURE_UNITS];
		for (int i{ 0 }; i < MAX_TEXTURE_UNITS; i++) {
			fShaderSampler2D[i] = i;
		}
		gData->mShader->SetValue("uTextures", fShaderSampler2D, MAX_TEXTURE_UNITS);

		gData->mTexturesUsed = 1;
		gData->mTextureArray[0] = gData->mWhiteTexture; // always set 0 to default texture
	}

	void Render2D::SceneEnd() {
		AXT_PROFILE_FUNCTION();

		uint32_t fDataSize{ static_cast<uint32_t>((char*)gData->mCurrentQuadVertex - (char*)gData->mQuadStart) };
		gData->mVertexBuffer->SubmitData(gData->mQuadStart, fDataSize);

		Flush();
	}

	void Render2D::Flush() {

		// loop through textures, bind them to their slot (0-31)
		for (int i{ 0 }; i < gData->mTexturesUsed; i++) {
			gData->mTextureArray[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(gData->mVertexArray, gData->mIndexCount);
	}

	void Render2D::DrawQuad(const QuadProperties&& fQuad) {
		AXT_PROFILE_FUNCTION();

		if (gData->mIndexCount + 4 > gData->mMaxQuads) {
			AXT_CORE_ASSERT(false, "Render2D::DrawQuad QuadAmount limit overflow!");
			return;
		}

		float fTexId{ 0 };
		if (gData->mTextureLibrary.Contains(fQuad.texName)) {
			bool fTexFound{ false };
			const Ref<Texture>& fTextureRequest{ gData->mTextureLibrary.Get(fQuad.texName) };
			// check if the texture is in the used textures array
			for (int i{ 0 }; i < gData->mTexturesUsed; i++) {
				if (gData->mTextureArray[i].get() == fTextureRequest.get()) {
					fTexId = static_cast<float>(i);
					fTexFound = true;
					break;
				}
			}
			// requested texture exists but isn't loaded
			if (!fTexFound) {
				gData->mTextureArray[gData->mTexturesUsed++] = gData->mTextureLibrary.Get(fQuad.texName);
				fTexId = static_cast<float>(gData->mTexturesUsed - 1);
			}
		}
		// if the texture doesn't exits it just uses index 0 - the white texture

		// model space transform
		const glm::mat4 fIdMat{ 1.f };
		const glm::mat4 fModelTransform{
			glm::translate(fIdMat, fQuad.position) *
			glm::rotate(fIdMat, fQuad.rotation, glm::vec3{0.f,0.f,1.f}) *
			glm::scale(fIdMat, glm::vec3{fQuad.size.x, fQuad.size.y, 1.f})
		};

		for (const glm::vec4& fCurrentPosition : gQuadPositions) {
			gData->mCurrentQuadVertex->position = fModelTransform * fCurrentPosition;;
			gData->mCurrentQuadVertex->color = fQuad.color;
			gData->mCurrentQuadVertex->textureCoordinate = { fCurrentPosition.x + 0.5f, fCurrentPosition.y + 0.5f };
			gData->mCurrentQuadVertex->textureId = fTexId;
			gData->mCurrentQuadVertex++;
		}

		gData->mIndexCount += 6;
	}

}