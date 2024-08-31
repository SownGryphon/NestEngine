#pragma once

// For use by Nest Engine clients

#include "Nest/Core/Core.h"

#include "Nest/Core/DataType.h"

#include "Nest/Core/Window.h"
#include "Nest/Core/Application.h"
#include "Nest/Core/Layer.h"
#include "Nest/Core/LayerStack.h"

#include "Nest/Events/Event.h"
#include "Nest/Events/KeyboardEvent.h"
#include "Nest/Events/MouseEvent.h"

#include "Nest/Core/KeyCodes.h"
#include "Nest/Core/Input.h"

// ------- Utils -------
#include "Nest/Utils/Random.h"
#include "Nest/Utils/Time.h"

// ----- Renderer ------
#include "Nest/renderer/Shader.h"
#include "Nest/renderer/VertexArray.h"
#include "Nest/renderer/IndexBuffer.h"
#include "Nest/renderer/Texture.h"

#include "Nest/renderer/RenderCommand.h"
#include "Nest/renderer/Renderer.h"
#include "Nest/renderer/Renderer2D.h"
#include "Nest/renderer/Renderer3D.h"

#include "Nest/renderer/cameras/OrthographicCamera2D.h"
#include "Nest/renderer/cameras/OrthoCamController2D.h"
#include "Nest/renderer/cameras/PerspectiveCamera3D.h"
#include "Nest/renderer/cameras/PerspectiveCamController3D.h"

#include "Nest/renderer/StaticMesh.h"

// ----- Graphics -----

// - Images -

#include "Nest/graphics/Image.h"
#include "Nest/graphics/PNGLoader.h"

// - Fonts -

#include "Nest/graphics/Font.h"

#include "Nest/graphics/fontLoader/TTFLoader.h"