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
#include "Nest/Renderer/Shader.h"
#include "Nest/Renderer/VertexArray.h"
#include "Nest/Renderer/IndexBuffer.h"
#include "Nest/Renderer/Texture.h"

#include "Nest/Renderer/RenderCommand.h"
#include "Nest/Renderer/Renderer.h"
#include "Nest/Renderer/Renderer2D.h"
#include "Nest/Renderer/Renderer3D.h"

#include "Nest/Renderer/cameras/OrthographicCamera2D.h"
#include "Nest/Renderer/cameras/OrthoCamController2D.h"
#include "Nest/Renderer/cameras/PerspectiveCamera3D.h"
#include "Nest/Renderer/cameras/PerspectiveCamController3D.h"

#include "Nest/Renderer/StaticMesh.h"

// ----- Graphics -----

// - Images -

#include "Nest/Graphics/Image.h"
#include "Nest/Graphics/PNGLoader.h"

// - Text -

#include "Nest/Graphics/Font.h"

#include "Nest/Graphics/fontLoader/TTFLoader.h"

#include "Nest/Graphics/TextBox.h"

// ===== ECS =====

#include "Nest/ECS/Entity/Entity.h"

#include "Nest/ECS/Component/Component.h"
#include "Nest/ECS/Component/TextBoxComponent.h"
#include "Nest/ECS/Component/SpriteComponent.h"

#include "Nest/ECS/System/System.h"
#include "Nest/ECS/System/TextBoxSystem.h"
#include "Nest/ECS/System/SpriteSystem.h"

#include "Nest/ECS/Scene.h"