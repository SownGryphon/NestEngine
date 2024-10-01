#pragma once

#include "Nest/Core/Core.h"
#include "Nest/renderer/VertexArray.h"
#include "Nest/renderer/IndexBuffer.h"

namespace Nest
{
	namespace RenderCommand
	{
		void clear();

		void setDepthEnabled(bool enabled);

		/**
		 * @brief Sets line width for subsequent draw calls
		 * @param width Line width in pixels
		 */
		void setLineWidth(float width);

		/**
		 * @brief Draws points using the bound vertex array and shader
		 * @param count How many point to draw
		*/
		void drawPoints(unsigned int count);

		/**
		 * @brief Draws lines using the bound vertex array and shader
		 * @param count How many lines to draw
		*/
		void drawLines(unsigned int count);

		/**
		 * @brief Draws lines using the bound vertex array and shader
		 * @param ib The index buffer specifying line vertices
		*/
		void drawLinesIndexed(Ref<const IndexBuffer> ib);

		/**
		 * @brief Draws triangles using the bound vertex array and shader
		 * @param count How many triangles to draw
		*/
		void drawTriangles(unsigned int count);

		/**
		 * @brief Draws triangles using the bound vertex array and shader
		 * @param ib The index buffer specifying triangle vertices
		*/
		void drawTrianglesIndexed(Ref<const IndexBuffer> ib);
	}
}