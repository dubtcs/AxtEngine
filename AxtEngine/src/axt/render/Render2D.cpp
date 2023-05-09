#include <pch.h>

#include "Render2D.h"

#include "RenderCommand.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// 32 is usually the most for desktop gpus
static constexpr int MAX_TEXTURE_UNITS = 32;

static constexpr uint32_t MAX_QUADS{ 1000 };
static constexpr uint32_t QUAD_VERTICES{ 4 };
static constexpr uint32_t QUAD_INDICES{ 6 };

static constexpr uint32_t MAX_QUAD_VERTICES{ MAX_QUADS * QUAD_VERTICES };
static constexpr uint32_t MAX_QUAD_INDICES{ MAX_QUADS * QUAD_INDICES };

static constexpr uint32_t MAX_CUBES{ 100 };
static constexpr uint32_t CUBE_VERTICES{ 24 };
static constexpr uint32_t CUBE_INDICES{ 36 };

static constexpr uint32_t MAX_CUBE_VERTICES{ MAX_CUBES * CUBE_VERTICES };
static constexpr uint32_t MAX_CUBE_INDICES{ MAX_CUBES * CUBE_INDICES };


/*
TODO:
Switch QuadVert textIndex to uint32_t
Make unique vertices for each face in cube to prevent texture stretching
*/

namespace axt 
{

	struct QuadVert 
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 textureCoordinate;
		int32_t textureId{ 0 };
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

	static constexpr glm::vec4 gQuadPositions[4]
	{
		glm::vec4{ -0.5f, -0.5f, 0.0f, 1.f },
		glm::vec4{  0.5f, -0.5f, 0.0f, 1.f },
		glm::vec4{  0.5f,  0.5f, 0.0f, 1.f },
		glm::vec4{ -0.5f,  0.5f, 0.0f, 1.f },
	};

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

	static constexpr glm::vec4 gCubePositionsUnique[24]
	{
		// Front
		glm::vec4{ -0.5f, -0.5f, 0.5f, 1.f }, // 0
		glm::vec4{  0.5f, -0.5f, 0.5f, 1.f }, // 1
		glm::vec4{  0.5f,  0.5f, 0.5f, 1.f }, // 2
		glm::vec4{ -0.5f,  0.5f, 0.5f, 1.f }, // 3

		// Top
		glm::vec4{ -0.5f,  0.5f,  0.5f, 1.f }, // 4
		glm::vec4{  0.5f,  0.5f,  0.5f, 1.f }, // 5
		glm::vec4{  0.5f,  0.5f, -0.5f, 1.f }, // 6
		glm::vec4{ -0.5f,  0.5f, -0.5f, 1.f }, // 7

		// Back
		glm::vec4{ -0.5f, -0.5f, -0.5f, 1.f }, // 8
		glm::vec4{  0.5f, -0.5f, -0.5f, 1.f }, // 9
		glm::vec4{  0.5f,  0.5f, -0.5f, 1.f }, // 10
		glm::vec4{ -0.5f,  0.5f, -0.5f, 1.f }, // 11

		// Bottom
		glm::vec4{  0.5f,  -0.5f,  0.5f, 1.f }, // 12
		glm::vec4{ -0.5f,  -0.5f,  0.5f, 1.f }, // 13
		glm::vec4{ -0.5f,  -0.5f, -0.5f, 1.f }, // 14
		glm::vec4{  0.5f,  -0.5f, -0.5f, 1.f }, // 15

		// Right
		glm::vec4{  0.5f,  -0.5f,  0.5f, 1.f }, // 16
		glm::vec4{  0.5f,  -0.5f, -0.5f, 1.f }, // 17
		glm::vec4{  0.5f,   0.5f, -0.5f, 1.f }, // 18
		glm::vec4{  0.5f,   0.5f,  0.5f, 1.f }, // 19

		// Left
		glm::vec4{ -0.5f,  -0.5f, -0.5f, 1.f }, // 20
		glm::vec4{ -0.5f,  -0.5f,  0.5f, 1.f }, // 21
		glm::vec4{ -0.5f,   0.5f,  0.5f, 1.f }, // 22
		glm::vec4{ -0.5f,   0.5f, -0.5f, 1.f }, // 23

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
		gData->mQuadVertexBuffer = VertexBuffer::Create(MAX_QUAD_VERTICES * sizeof(QuadVert));

		gData->mCubeVertexBuffer= VertexBuffer::Create(MAX_CUBE_VERTICES * sizeof(QuadVert));

		// QuadVert structure
		axt::BufferLayout lBufferLayout{
			{axt::ShaderDataType::Float3, "inPos"},
			{axt::ShaderDataType::Float4, "inColor"},
			{axt::ShaderDataType::Float2, "inTexPos"},
			{axt::ShaderDataType::Int, "inTexIndex"},

			{axt::ShaderDataType::UInt, "inEntityId"}
		};

		gData->mQuadVertexBuffer->SetLayout(lBufferLayout);
		gData->mVertexArray->AddVertexBuffer(gData->mQuadVertexBuffer);

		gData->mCubeVertexBuffer->SetLayout(lBufferLayout);
		gData->mVertexArray->AddVertexBuffer(gData->mCubeVertexBuffer);

		gData->mQuadStart = new QuadVert[MAX_QUAD_VERTICES]; // forgot to multiply this by 4 lol
		gData->mCubeStart = new QuadVert[MAX_CUBE_VERTICES];

		Unique<uint32_t[]> fIndexData{ NewUnique<uint32_t[]>(MAX_QUAD_INDICES) };
		uint32_t fIndexOffset{ 0 };
		for (uint32_t i{ 0 }; i < MAX_QUAD_INDICES; i += 6) 
		{
			fIndexData[i] = fIndexOffset;
			fIndexData[i + 1] = fIndexOffset + 1;
			fIndexData[i + 2] = fIndexOffset + 2;

			fIndexData[i + 3] = fIndexOffset + 2;
			fIndexData[i + 4] = fIndexOffset + 3;
			fIndexData[i + 5] = fIndexOffset + 0;

			fIndexOffset += 4;
		}

		Ref<IndexBuffer> fIndexBuffer{ IndexBuffer::Create(fIndexData.get(), MAX_QUAD_INDICES)};
		//gData->mVertexArray->AddIndexBuffer(fIndexBuffer);
		gData->mQuadIBuffer = fIndexBuffer;
		fIndexBuffer->Unbind();

		fIndexOffset = 0;
		uint32_t* indexData{ new uint32_t[MAX_CUBE_INDICES] };
		/*
		for (uint32_t i{ 0 }; i < MAX_CUBE_INDICES; i += 36)
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

			fIndexOffset += 8; //  Fixed drawing only 1 cube
			// lmfao I had this set to 36
		}
		*/

		for (uint32_t i{ 0 }; i < MAX_CUBE_INDICES; i += 36)
		{
			// CCW Rotation

			// Front
			indexData[i] = fIndexOffset;
			indexData[i + 1] = fIndexOffset + 1;
			indexData[i + 2] = fIndexOffset + 2;

			indexData[i + 3] = fIndexOffset;
			indexData[i + 4] = fIndexOffset + 2;
			indexData[i + 5] = fIndexOffset + 3;

			// Top
			indexData[i + 6] = fIndexOffset + 4;
			indexData[i + 7] = fIndexOffset + 5;
			indexData[i + 8] = fIndexOffset + 6;

			indexData[i + 9]  = fIndexOffset + 4;
			indexData[i + 10] = fIndexOffset + 6;
			indexData[i + 11] = fIndexOffset + 7;

			// Back
			indexData[i + 12] = fIndexOffset + 8;
			indexData[i + 13] = fIndexOffset + 9;
			indexData[i + 14] = fIndexOffset + 10;

			indexData[i + 15] = fIndexOffset + 8;
			indexData[i + 16] = fIndexOffset + 10;
			indexData[i + 17] = fIndexOffset + 11;

			// Bottom
			indexData[i + 18] = fIndexOffset + 12;
			indexData[i + 19] = fIndexOffset + 13;
			indexData[i + 20] = fIndexOffset + 14;

			indexData[i + 21] = fIndexOffset + 12;
			indexData[i + 22] = fIndexOffset + 14;
			indexData[i + 23] = fIndexOffset + 15;

			// Right
			indexData[i + 24] = fIndexOffset + 16;
			indexData[i + 25] = fIndexOffset + 17;
			indexData[i + 26] = fIndexOffset + 18;

			indexData[i + 27] = fIndexOffset + 16;
			indexData[i + 28] = fIndexOffset + 18;
			indexData[i + 29] = fIndexOffset + 19;

			// Left
			indexData[i + 30] = fIndexOffset + 20;
			indexData[i + 31] = fIndexOffset + 21;
			indexData[i + 32] = fIndexOffset + 22;

			indexData[i + 33] = fIndexOffset + 20;
			indexData[i + 34] = fIndexOffset + 22;
			indexData[i + 35] = fIndexOffset + 23;

			fIndexOffset += CUBE_VERTICES;
		}

		gData->mCubeIBuffer = IndexBuffer::Create(indexData, MAX_CUBE_INDICES);
		gData->mCubeIBuffer->Unbind();

		delete[] indexData;

		gData->mShader = axt::Shader::Create("Shader1", "shaders/aio3_vp.glsl", axt::ShaderType::Vertex | axt::ShaderType::Pixel);

		// creating a white texture
		gData->mWhiteTexture = Texture2D::Create(1, 1);
		uint32_t fWhite{ 0xffffffff }; // 1 in all 4 channels
		gData->mWhiteTexture->SetData(&fWhite, sizeof(uint32_t));

		gData->mTextureArray[0] = gData->mWhiteTexture;
		gData->mTextureLibrary.Add("White", gData->mWhiteTexture);
		Ref<Texture2D> fBruh{ Texture2D::Create("textures/si.png") };
		gData->mTextureLibrary.Add("Bruh", fBruh);
		Ref<Texture2D> fCheck{ Texture2D::Create("textures/checkerboard.png") };
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

		int fShaderSampler2D[MAX_TEXTURE_UNITS]{};
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
			AXT_TRACE("Texture {0}", i);
			gData->mTextureArray[i]->Bind(i);
		}

		gStats.drawCalls++;
		gStats.textures += gData->mTexturesUsed;

		// "2D" Rendering
		gData->mQuadVertexBuffer->Bind();
		gData->mQuadIBuffer->Bind();
		gData->mVertexArray->ToggleVertexBuffer(gData->mQuadVertexBuffer);
		RenderCommand::DrawIndexed(gData->mVertexArray, gData->mIndexCount);

		gData->mCubeVertexBuffer->Bind();
		gData->mCubeIBuffer->Bind();
		gData->mVertexArray->ToggleVertexBuffer(gData->mCubeVertexBuffer);
		RenderCommand::DrawIndexed(gData->mVertexArray, gData->mCubeIndexCount);
	}

	// reset variables
	// wipe vertex and index data
	void Render2D::StageForOverflow() 
	{
		SceneEnd();
		AXT_CORE_ASSERT(false, "Render2D Overflow");
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

		if (gData->mIndexCount + 6 > MAX_QUAD_INDICES) 
		{
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
			//glm::rotate(fIdMat, fQuad.rotation, glm::vec3{0.f,0.f,1.f}) *

			glm::rotate(fIdMat, glm::radians(fQuad.rotation.x), glm::vec3{1.f, 0.f, 0.f})*
			glm::rotate(fIdMat, glm::radians(fQuad.rotation.y), glm::vec3{0.f, 1.f, 0.f})*
			glm::rotate(fIdMat, glm::radians(fQuad.rotation.z), glm::vec3{0.f, 0.f, 1.f})*

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

	/*
	void Render2D::DrawCube(const QuadProperties&& props)
	{
		AXT_PROFILE_FUNCTION();

		if (gData->mCubeIndexCount + 36 > MAX_CUBE_INDICES)
		{
			StageForOverflow();
		}

		int32_t textureIndex{ 0 };
		if (gData->mTextureLibrary.Contains(props.texName))
		{
			bool found{ false };
			const Ref<Texture2D>& textureRequest{ gData->mTextureLibrary.Get(props.texName) };
			for (textureIndex; textureIndex < gData->mTexturesUsed; textureIndex++)
			{
				if (gData->mTextureArray[textureIndex].get() == textureRequest.get())
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				gData->mTextureArray[gData->mTexturesUsed++] = gData->mTextureLibrary.Get(props.texName);
			}
		}

		// model space transform
		const glm::mat4 ones{ 1.f };
		const glm::mat4 modelTransform{
			glm::translate(ones, props.position) * // can trim the rotation from this if it's not rotated
			
			glm::rotate(ones, glm::radians(props.rotation.x), glm::vec3{1.f, 0.f, 0.f}) *
			glm::rotate(ones, glm::radians(props.rotation.y), glm::vec3{0.f, 1.f, 0.f}) *
			glm::rotate(ones, glm::radians(props.rotation.z), glm::vec3{0.f, 0.f, 1.f}) *

			glm::scale(ones, glm::vec3{props.size.x, props.size.y, props.size.z })
		};
		for (const glm::vec4& fCurrentPosition : gCubePositions)
		{
			gData->mCubeCurrent->position = modelTransform * fCurrentPosition;
			gData->mCubeCurrent->color = props.color;
			gData->mCubeCurrent->textureCoordinate = { (fCurrentPosition.x + 0.5f) * props.textureTiling, (fCurrentPosition.y + 0.5f) * props.textureTiling };
			gData->mCubeCurrent->textureId = static_cast<float>(textureIndex);

			// EDITOR SHADER ONLY
			// REMOVE WHEN BETTER MOUSE SELECTION IS ADDED
			gData->mCubeCurrent->entityId = props.EntityId;

			gData->mCubeCurrent++;

			gData->mCubeVertexCount++;
		}

		gData->mCubeIndexCount += 36;
	}
	*/

	// this can be solved with normals
	static constexpr glm::vec2 gTextureCoords[4]
	{
		glm::vec2{0,0},
		glm::vec2{1,0},
		glm::vec2{1,1},
		glm::vec2{0,1}
	};

	void Render2D::DrawCube(const QuadProperties&& props)
	{
		AXT_PROFILE_FUNCTION();

		if (gData->mCubeIndexCount + CUBE_INDICES > MAX_CUBE_INDICES)
		{
			StageForOverflow();
		}

		int32_t textureIndex{ 0 };
		if (gData->mTextureLibrary.Contains(props.texName))
		{
			bool found{ false };
			const Ref<Texture2D>& textureRequest{ gData->mTextureLibrary.Get(props.texName) };
			for (textureIndex; textureIndex < gData->mTexturesUsed; textureIndex++)
			{
				if (gData->mTextureArray[textureIndex].get() == textureRequest.get())
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				gData->mTextureArray[gData->mTexturesUsed++] = gData->mTextureLibrary.Get(props.texName);
			}
		}

		// model space transform
		const glm::mat4 ones{ 1.f };
		const glm::mat4 modelTransform{
			glm::translate(ones, props.position) * // can trim the rotation from this if it's not rotated

			glm::rotate(ones, glm::radians(props.rotation.x), glm::vec3{1.f, 0.f, 0.f}) *
			glm::rotate(ones, glm::radians(props.rotation.y), glm::vec3{0.f, 1.f, 0.f}) *
			glm::rotate(ones, glm::radians(props.rotation.z), glm::vec3{0.f, 0.f, 1.f}) *

			glm::scale(ones, glm::vec3{props.size.x, props.size.y, props.size.z })
		};

		uint32_t normalIndex{ 0 };
		for (const glm::vec4& fCurrentPosition : gCubePositionsUnique)
		{
			gData->mCubeCurrent->position = modelTransform * fCurrentPosition;
			gData->mCubeCurrent->color = props.color;

			// this modulo can be removed with normals. bandaid fix
			gData->mCubeCurrent->textureCoordinate = { gTextureCoords[normalIndex % 4] * props.textureTiling };
			gData->mCubeCurrent->textureId = textureIndex;

			// EDITOR SHADER ONLY
			// REMOVE WHEN BETTER MOUSE SELECTION IS ADDED
			gData->mCubeCurrent->entityId = props.EntityId;

			gData->mCubeCurrent++;

			gData->mCubeVertexCount++;
			normalIndex++;
		}

		gData->mCubeIndexCount += CUBE_INDICES;
	}

	Render2D::RenderStats Render2D::GetStats() 
	{
		return gStats;
	}

}