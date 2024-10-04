#pragma once

#include <maths/Mat4.h>

#include "Nest/Core/Core.h"

#include "Nest/Renderer/Shader.h"
#include "Nest/Renderer/VertexArray.h"
#include "Nest/Renderer/IndexBuffer.h"

namespace Nest
{
	class GraphicsElement
	{
	public:
		//virtual Ref<Shader> getShader() const = 0;
		virtual Ref<VertexArray> getVA() const = 0;
		virtual Ref<IndexBuffer> getIB() const = 0;

		virtual chcl::Mat4 getTransform() const { return chcl::Mat4::Identity(); }
	};
}