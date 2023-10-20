#pragma once

#include "Nest/Core/Core.h"

#include "OrthograhicCamera.h"

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace Nest
{
	namespace Renderer
	{
		void init();
		void shutdown();

		void beginScene(const OrthograhicCamera &camera);

		void drawIndexed(Shader &shader, const VertexArray &va, const IndexBuffer &ib);
		void drawLinesIndexed(Shader &shader, const VertexArray &va, const IndexBuffer &ib);
		void drawPoints(Shader &shader, const VertexArray &va, unsigned int count);
		void drawLines(Shader &shader, const VertexArray &va, unsigned int vertices);
		void drawTriangles(Shader &shader, const VertexArray &va, unsigned int count);
	};
}