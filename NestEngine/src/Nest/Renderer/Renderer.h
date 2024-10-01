#pragma once

#include "Nest/Core/Core.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include "maths/Mat4.h"

namespace Nest
{
	namespace Renderer
	{
		void init();
		void shutdown();

		// Sets up a scene using the rovided view projection matrix
		void beginScene(const chcl::Mat4 &vpm);

		void drawIndexed(Ref<Shader> shader, Ref<const VertexArray> va, const IndexBuffer &ib);
		void drawLinesIndexed(Ref<Shader> shader, Ref<const VertexArray> va, const IndexBuffer &ib);
		void drawPoints(Ref<Shader> shader, Ref<const VertexArray> va, unsigned int count);
		void drawLines(Ref<Shader> shader, Ref<const VertexArray> va, unsigned int vertices);
		void drawTriangles(Ref<Shader> shader, Ref<const VertexArray> va, unsigned int count);
	};
}