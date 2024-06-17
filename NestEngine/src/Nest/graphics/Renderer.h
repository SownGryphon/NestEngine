#pragma once

#include "Nest/Core/Core.h"

#include "OrthographicCamera.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace Nest
{
	namespace Renderer
	{
		void init();
		void shutdown();

		// Sets up a scene using the rovided view projection matrix
		void beginScene(const chcl::Mat4 &vpm);

		void drawIndexed(Shader &shader, const VertexArray &va, const IndexBuffer &ib);
		void drawLinesIndexed(Shader &shader, const VertexArray &va, const IndexBuffer &ib);
		void drawPoints(Shader &shader, const VertexArray &va, unsigned int count);
		void drawLines(Shader &shader, const VertexArray &va, unsigned int vertices);
		void drawTriangles(Shader &shader, const VertexArray &va, unsigned int count);
	};
}