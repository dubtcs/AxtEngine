#pragma once

/*
* Be sure to include <axt/EntryPoint.h> in the main file you want the app to start
*/

#include "axt/Core.h"

#include "axt/core/App.h"
#include "axt/core/Log.h"
#include "axt/core/Layer.h"
#include "axt/core/Input.h"
#include "axt/core/CameraController.h"

#include "axt/core/Keycodes.h"
#include "axt/core/MousebuttonCodes.h"

#include "axt/render/OrthoCamera.h"
#include "axt/render/Renderer.h"
#include "axt/render/RenderCommand.h"
#include "axt/render/VertexArray.h"
#include "axt/render/Buffers.h"
#include "axt/render/FrameBuffer.h"
#include "axt/render/Shader.h"
#include "axt/render/RenderContext.h"
#include "axt/render/Texture.h"

#include "axt/world/UUID.h"
#include "axt/world/GameWorld.h"
#include "axt/world/RenderSystem.h"
#include "axt/world/CameraControlSystem.h"

// not needed. Include at main
//#include "axt/EntryPoint.h"
