#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Graphics/VertexArray.h"
#include "Nest/Graphics/IndexBuffer.h"

namespace Nest
{
	namespace RenderCommand
	{
		void clear();

		void drawPoints(unsigned int count);
		void drawLines(unsigned int count);
		void drawLinesIndexed(Ref<IndexBuffer> &ib);
		void drawTriangles(unsigned int count);
		void drawTrianglesIndexed(Ref<IndexBuffer> &ib);
	}
}