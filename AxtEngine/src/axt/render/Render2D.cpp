#include <pch.h>

#include "Render2D.h"

#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>

// 32 is usually the most for desktop gpus
constexpr int MAX_TEXTURE_UNITS = 32;
constexpr uint32_t MAX_QUADS = 1000;
constexpr uint32_t MAX_CUBES = 100;

/*
TODO:
Fix batch rendering.. again.
This doesn't really need a view matrix, I think. Can just use object position and camera position offsets to adjust viewspace.
*/

namespace axt 
{

	struct QuadVert 
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinate;
		float textureId = 0;
		uint32_t entityId{ necs::nil };
	};

	struct Render2DScene 
	{
		Ref<VertexArray> mVertexArray;
		Ref<Texture2D> mWhiteTexture;
		Ref<Shader> mShader;

		uint32_t mTexturesUsed{ 1 };
		uint32_t mVertexCount{ 0 };
		uint32_t mIndexCount{ 0 };
		const uint32_t mMaxVertices{ MAX_QUADS * 4 };
		const uint32_t mMaxIndices{ MAX_QUADS * 6 };

		const uint32_t mMaxCubeVertices{ MAX_CUBES * 8 };
		const uint32_t mMaxCubeIndices{ MAX_CUBES * 36 };

		QuadVert* mQuadStart{ nullptr }; // starting point of quad array
		QuadVert* mCurrentQuadVertex{ nullptr }; // current index in scene

		QuadVert* mCubeStart{ nullptr };
		QuadVert* mCubeCurrent{ nullptr };

		Ref<VertexBuffer> mQuadVertexBuffer{ nullptr };
		Ref<IndexBuffer> mQuadIBuffer{ nullptr };

		Ref<VertexBuffer> mCubeVertexBuffer{ nullptr };
		Ref<IndexBuffer> mCubeIBuffer{ nullptr };

		uint32_t mCubeVertexCount{ 0 };
		uint32_t mCubeIndexCount{ 0 };

		std::array<Ref<Texture2D>, MAX_TEXTURE_UNITS> mTextureArray;
		TextureLib mTextureLibrary;
	};

	static Render2DScene* gData;

	static Render2D::RenderStats gStats;

	static const glm::vec4 gQuadPositions[4]
	{
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

	void Render2D::Init() 
	{
		AXT_PROFILE_FUNCTION();

		gData = new Render2DScene{};
		gData->mVertexArray = VertexArray::Create();
		gData->mQuadVertexBuffer = VertexBuffer::Create(gData->mMaxVertices * sizeof(QuadVert));

		gData->mCubeVertexBuffer= VertexBuffer::Create(gData->mMaxCubeVertices * sizeof(QuadVert));

		// QuadVert structure
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
			{axt::ShaderDataType::Float2, "inTexPos"},
			{axt::ShaderDataType::Float, "inTexId"},

			{axt::ShaderDataType::UInt, "inEntityId"}
		};

		gData->mQuadVertexBuffer->SetLayout(lBufferLayout);
		gData->mVertexArray->AddVertexBuffer(gData->mQuadVertexBuffer);

		gData->mCubeVertexBuffer->SetLayout(lBufferLayout);
		gData->mVertexArray->AddVertexBuffer(gData->mCubeVertexBuffer);

		gData->mQuadStart = new QuadVert[MAX_QUADS * 4]; // forgot to multiply this by 4 lol
		gData->mCubeStart = new QuadVert[MAX_CUBES * 8];

		Unique<uint32_t[]> fIndexData{ NewUnique<uint32_t[]>(gData->mMaxIndices) };
		uint32_t fIndexOffset{ 0 };
		for (uint32_t i{ 0 }; i < gData->mMaxIndices; i += 6) 
		{
			fIndexData[i] = fIndexOffset;
			fIndexData[i + 1] = fIndexOffset + 1;
			fIndexData[i + 2] = fIndexOffset + 2;

			fIndexData[i + 3] = fIndexOffset + 2;
			fIndexData[i + 4] = fIndexOffset + 3;
			fIndexData[i + 5] = fIndexOffset + 0;

			fIndexOffset += 4;
		}

		Ref<IndexBuffer> fIndexBuffer{ IndexBuffer::Create(fIndexData.get(), gData->mMaxIndices)};
		//gData->mVertexArray->AddIndexBuffer(fIndexBuffer);
		gData->mQuadIBuffer = fIndexBuffer;
		fIndexBuffer->Unbind();

		fIndexOffset = 0;
		uint32_t* indexData{ new uint32_t[MAX_CUBES * 36] };
		for (uint32_t i{ 0 }; i < gData->mMaxCubeIndices; i += 36)
		{
			// CCW Rotation starting from bottom left

			// Front
			indexData[i]	 = fIndexOffset;
			indexData[i +  1] = fIndexOffset + 1;
			indexData[i +  2] = fIndexOffset + 2;
						  
			indexData[i +  3] = fIndexOffset;
			indexData[i +  4] = fIndexOffset + 2;
			indexData[i +  5] = fIndexOffset + 3;

			// Top
			indexData[i +  6] = fIndexOffset + 3;
			indexData[i +  7] = fIndexOffset + 2;
			indexData[i +  8] = fIndexOffset + 6;

			indexData[i +  9] = fIndexOffset + 3;
			indexData[i + 10] = fIndexOffset + 6;
			indexData[i + 11] = fIndexOffset + 7;

			// Back
			indexData[i + 12] = fIndexOffset + 5;
			indexData[i + 13] = fIndexOffset + 4;
			indexData[i + 14] = fIndexOffset + 7;

			indexData[i + 15] = fIndexOffset + 5;
			indexData[i + 16] = fIndexOffset + 7;
			indexData[i + 17] = fIndexOffset + 6;

			// Bottom
			indexData[i + 18] = fIndexOffset;
			indexData[i + 19] = fIndexOffset + 4;
			indexData[i + 20] = fIndexOffset + 1;

			indexData[i + 21] = fIndexOffset + 1;
			indexData[i + 22] = fIndexOffset + 4;
			indexData[i + 23] = fIndexOffset + 5;

			// Right
			indexData[i + 24] = fIndexOffset + 1;
			indexData[i + 25] = fIndexOffset + 5;
			indexData[i + 26] = fIndexOffset + 2;

			indexData[i + 27] = fIndexOffset + 2;
			indexData[i + 28] = fIndexOffset + 5;
			indexData[i + 29] = fIndexOffset + 6;

			// Left
			indexData[i + 30] = fIndexOffset + 4;
			indexData[i + 31] = fIndexOffset;
			indexData[i + 32] = fIndexOffset + 3;

			indexData[i + 33] = fIndexOffset + 4;
			indexData[i + 34] = fIndexOffset + 3;
			indexData[i + 35] = fIndexOffset + 7;

			fIndexOffset += 36;
		}

		gData->mCubeIBuffer = IndexBuffer::Create(indexData, gData->mMaxCubeIndices);
		gData->mCubeIBuffer->Unbind();

		delete[] indexData;

		gData->mShader = axt::Shader::Create("Shader1", "shaders/aio2_vp.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);

		// creating a white texture
		gData->mWhiteTexture = Texture2D::Create(1, 1);
		uint32_t fWhite{ 0xffffffff }; // 1 in all 4 channels
		gData->mWhiteTexture->SetData(&fWhite, sizeof(uint32_t));

		gData->mTextureArray[0] = gData->mWhiteTexture;
		gData->mTextureLibrary.Add("White", gData->mWhiteTexture);
		Ref<Texture2D> fBruh{ Texture2D::Create("textures/si.png") };
		gData->mTextureLibrary.Add("Bruh", fBruh);
		Ref<Texture2D> fCheck{ Texture2D::Create("textures/bruh.png") };
		gData->mTextureLibrary.Add("Check", fCheck);
	}

	void Render2D::Shutdown() 
	{

		delete gData;
	}

	void Render2D::SceneStart(const OrthoCamera& camera) 
	{
		AXT_PROFILE_FUNCTION();
		gData->mCurrentQuadVertex = gData->mQuadStart;
		gData->mIndexCount = 0;
		gData->mVertexCount = 0;

		// reset stats
		gStats.drawCalls = 0;
		gStats.quads = 0;
		gStats.textures = 0;

		gData->mShader->Bind();
		gData->mShader->SetValue("uViewProjection", camera.GetViewProjection());

		int fShaderSampler2D[MAX_TEXTURE_UNITS];
		for (int i{ 0 }; i < MAX_TEXTURE_UNITS; i++) 
		{
			fShaderSampler2D[i] = i;
		}
		gData->mShader->SetValue("uTextures", fShaderSampler2D, MAX_TEXTURE_UNITS);

		gData->mTexturesUsed = 1;
		gData->mTextureArray[0] = gData->mWhiteTexture; // always set 0 to default texture
	}

	// Runtime cameras
	void Render2D::SceneStart(const Camera& camera, const glm::mat4& viewProjection)
	{
		AXT_PROFILE_FUNCTION();
		gData->mCurrentQuadVertex = gData->mQuadStart;
		gData->mIndexCount = 0;
		gData->mVertexCount = 0;

		// reset stats
		gStats.drawCalls = 0;
		gStats.quads = 0;
		gStats.textures = 0;

		gData->mShader->Bind();
		gData->mShader->SetValue("uViewProjection", viewProjection);

		int fShaderSampler2D[MAX_TEXTURE_UNITS];
		for (int i{ 0 }; i < MAX_TEXTURE_UNITS; i++) 
		{
			fShaderSampler2D[i] = i;
		}
		gData->mShader->SetValue("uTextures", fShaderSampler2D, MAX_TEXTURE_UNITS);

		gData->mTexturesUsed = 1;
		gData->mTextureArray[0] = gData->mWhiteTexture; // always set 0 to default texture
	}

	// Editor camera
	void Render2D::SceneStart(const glm::mat4& viewProjection)
	{
		AXT_PROFILE_FUNCTION();
		gData->mCurrentQuadVertex = gData->mQuadStart;
		gData->mIndexCount = 0;
		gData->mVertexCount = 0;

		gData->mCubeCurrent = gData->mCubeStart;
		gData->mCubeIndexCount = 0;
		gData->mCubeVertexCount = 0;

		// reset stats
		gStats.drawCalls = 0;
		gStats.quads = 0;
		gStats.textures = 0;

		gData->mShader->Bind();
		gData->mShader->SetValue("uViewProjection", viewProjection);

		int fShaderSampler2D[MAX_TEXTURE_UNITS];
		for (int i{ 0 }; i < MAX_TEXTURE_UNITS; i++) 
		{
			fShaderSampler2D[i] = i;
		}
		gData->mShader->SetValue("uTextures", fShaderSampler2D, MAX_TEXTURE_UNITS);

		gData->mTexturesUsed = 1;
		gData->mTextureArray[0] = gData->mWhiteTexture; // always set 0 to default texture
	}

	// push data to gpu
	void Render2D::SceneEnd() 
	{
		AXT_PROFILE_FUNCTION();

		uint32_t fDataSize{ static_cast<uint32_t>((char*)gData->mCurrentQuadVertex - (char*)gData->mQuadStart) };
		gData->mQuadVertexBuffer->SubmitData(gData->mQuadStart, fDataSize);

		uint32_t cubeSize{ static_cast<uint32_t>((char*)gData->mCubeCurrent - (char*)gData->mCubeStart) };
		gData->mCubeVertexBuffer->SubmitData(gData->mCubeStart, cubeSize);

		Stage();
	}

	// draw data on gpu
	void Render2D::Stage() 
	{

		// loop through textures, bind them to their slot (0-31)
		for (uint32_t i{ 0 }; i < gData->mTexturesUsed; i++) 
		{
			gData->mTextureArray[i]->Bind(i);
		}

		gStats.drawCalls++;
		gStats.textures += gData->mTexturesUsed;

		// "2D" Rendering
		gData->mQuadVertexBuffer->Bind();
		gData->mQuadIBuffer->Bind();
		RenderCommand::DrawIndexed(gData->mVertexArray, gData->mIndexCount);

		gData->mCubeVertexBuffer->Bind();
		gData->mCubeIBuffer->Bind();
		RenderCommand::DrawIndexed(gData->mVertexArray, gData->mCubeIndexCount);
	}

	// reset variables
	// wipe vertex and index data
	void Render2D::StageForOverflow() 
	{
		SceneEnd();
		AXT_CORE_TRACE("Render2D Overflow");
		gData->mTexturesUsed = 1;
		gData->mTextureArray[0] = gData->mWhiteTexture; // always set 0 to default texture
		gData->mCurrentQuadVertex = gData->mQuadStart;
		gData->mIndexCount = 0;
		gData->mVertexCount = 0;

		gData->mCubeCurrent = gData->mCubeStart;
		gData->mCubeIndexCount = 0;
		gData->mCubeVertexCount = 0;
	}

	// add the quad to memory
	// stage the current quads if overflow
	// increment variables
	void Render2D::DrawQuad(const QuadProperties&& fQuad) 
	{
		AXT_PROFILE_FUNCTION();

		if (gData->mIndexCount + 6 > gData->mMaxIndices) 
		{
		//if( gData->mVertexCount + 4 > gData->mMaxVertices) {
			//AXT_CORE_ASSERT(false, "Render2D::DrawQuad QuadAmount limit overflow!");
			StageForOverflow();
		}

		float fTexId{ 0 };
		if (gData->mTextureLibrary.Contains(fQuad.texName)) 
		{
			bool fTexFound{ false };
			const Ref<Texture>& fTextureRequest{ gData->mTextureLibrary.Get(fQuad.texName) };
			// check if the texture is in the used textures array
			for (uint32_t i{ 0 }; i < gData->mTexturesUsed; i++) 
			{
				if (gData->mTextureArray[i].get() == fTextureRequest.get()) 
				{
					fTexId = static_cast<float>(i);
					fTexFound = true;
					break;
				}
			}
			// requested texture exists but isn't loaded
			if (!fTexFound) 
			{
				gData->mTextureArray[gData->mTexturesUsed] = gData->mTextureLibrary.Get(fQuad.texName);
				fTexId = static_cast<float>(gData->mTexturesUsed++);
			}
		}
		// if the texture doesn't exits it just uses index 0 - the white texture

		// model space transform
		const glm::mat4 fIdMat{ 1.f };
		const glm::mat4 fModelTransform{
			glm::translate(fIdMat, fQuad.position) * // can trim the rotation from this if it's not rotated
			glm::rotate(fIdMat, fQuad.rotation, glm::vec3{0.f,0.f,1.f}) *
			glm::scale(fIdMat, glm::vec3{fQuad.size.x, fQuad.size.y, 1.f})
		};

		for (const glm::vec4& fCurrentPosition : gQuadPositions) 
		{
			gData->mCurrentQuadVertex->position = fModelTransform * fCurrentPosition;;
			gData->mCurrentQuadVertex->color = fQuad.color;
			gData->mCurrentQuadVertex->textureCoordinate = { (fCurrentPosition.x + 0.5f) * fQuad.textureTiling, (fCurrentPosition.y + 0.5f) * fQuad.textureTiling };
			gData->mCurrentQuadVertex->textureId = fTexId;

			// EDITOR SHADER ONLY
			// REMOVE WHEN BETTER MOUSE SELECTION IS ADDED
			gData->mCurrentQuadVertex->entityId = fQuad.EntityId;

			gData->mCurrentQuadVertex++;

			gData->mVertexCount++;
		}

		gData->mIndexCount += 6;
		gStats.quads++;

	}

	static constexpr glm::vec4 gCubePositions[8]
	{
		// Front
		glm::vec4{ -0.5f, -0.5f, 0.5f, 1.f }, // 0
		glm::vec4{  0.5f, -0.5f, 0.5f, 1.f }, // 1
		glm::vec4{  0.5f,  0.5f, 0.5f, 1.f }, // 2
		glm::vec4{ -0.5f,  0.5f, 0.5f, 1.f }, // 3

		// Back
		glm::vec4{ -0.5f, -0.5f, -0.5f, 1.f }, // 4
		glm::vec4{  0.5f, -0.5f, -0.5f, 1.f }, // 5
		glm::vec4{  0.5f,  0.5f, -0.5f, 1.f }, // 6
		glm::vec4{ -0.5f,  0.5f, -0.5f, 1.f }, // 7
	};

	void Render2D::DrawCube(const QuadProperties&& props)
	{
		AXT_PROFILE_FUNCTION();

		if (gData->mCubeIndexCount + 36 > gData->mMaxCubeIndices)
		{
			//if( gData->mVertexCount + 4 > gData->mMaxVertices) {
				//AXT_CORE_ASSERT(false, "Render2D::DrawQuad QuadAmount limit overflow!");
			StageForOverflow();
		}

		float fTexId{ 0 };
		if (gData->mTextureLibrary.Contains(props.texName))
		{
			bool fTexFound{ false };
			const Ref<Texture>& fTextureRequest{ gData->mTextureLibrary.Get(props.texName) };
			// check if the texture is in the used textures array
			for (uint32_t i{ 0 }; i < gData->mTexturesUsed; i++)
			{
				if (gData->mTextureArray[i].get() == fTextureRequest.get())
				{
					fTexId = static_cast<float>(i);
					fTexFound = true;
					break;
				}
			}
			// requested texture exists but isn't loaded
			if (!fTexFound)
			{
				gData->mTextureArray[gData->mTexturesUsed] = gData->mTextureLibrary.Get(props.texName);
				fTexId = static_cast<float>(gData->mTexturesUsed++);
			}
		}
		// if the texture doesn't exits it just uses index 0 - the white texture

		// model space transform
		const glm::mat4 fIdMat{ 1.f };
		const glm::mat4 fModelTransform {
			glm::translate(fIdMat, props.position) * // can trim the rotation from this if it's not rotated
			//glm::rotate(fIdMat, props.rotation, glm::vec3{0.f, 0.f, 1.f}) *
			glm::scale(fIdMat, glm::vec3{props.size.x, props.size.y, 1.f})
		};

		for (const glm::vec4& fCurrentPosition : gCubePositions)
		{
			gData->mCubeCurrent->position = fModelTransform * fCurrentPosition;;
			gData->mCubeCurrent->color = props.color;
			gData->mCubeCurrent->textureCoordinate = { (fCurrentPosition.x + 0.5f) * props.textureTiling, (fCurrentPosition.y + 0.5f) * props.textureTiling };
			gData->mCubeCurrent->textureId = fTexId;

			// EDITOR SHADER ONLY
			// REMOVE WHEN BETTER MOUSE SELECTION IS ADDED
			gData->mCubeCurrent->entityId = props.EntityId;

			gData->mCubeCurrent++;

			gData->mCubeVertexCount++;
		}

		gData->mCubeIndexCount += 36;
		AXT_TRACE("Cube Indices: {0}", gData->mCubeIndexCount);
	}

	Render2D::RenderStats Render2D::GetStats() 
	{
		return gStats;
	}

}