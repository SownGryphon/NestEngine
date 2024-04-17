#pragma once

#include "Nest/Core/Core.h"
#include "Nest/Graphics/VertexArray.h"
#include "Nest/Graphics/IndexBuffer.h"

namespace Nest
{
	namespace RenderCommand
	{
		void clear();

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
		void drawLinesIndexed(Ref<IndexBuffer> &ib);

		/**
		 * @brief Draws triangles using the bound vertex array and shader
		 * @param count How many triangles to draw
		*/
		void drawTriangles(unsigned int count);

		/**
		 * @brief Draws triangles using the bound vertex array and shader
		 * @param ib The index buffer specifying triangle vertices
		*/
		void drawTrianglesIndexed(Ref<IndexBuffer> &ib);
	}
}