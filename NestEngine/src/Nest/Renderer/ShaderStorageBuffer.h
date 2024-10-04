#pragma once

#include "Nest/Renderer/GraphicsBuffer.h"

namespace Nest
{
	/// Class for OpenGL SSBOs
	using ShaderStorageBuffer = GraphicsBuffer<GL_SHADER_STORAGE_BUFFER>;
}