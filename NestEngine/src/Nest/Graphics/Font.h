#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <geometry/Vector2.h>

#include "Nest/renderer/ShaderStorageBuffer.h"
#include "Nest/renderer/VertexArray.h"
#include "Nest/renderer/VertexBufferLayout.h"
#include "Nest/renderer/IndexBuffer.h"

#include "Nest/renderer/Shader.h"

namespace Nest
{
	class Font
	{
	public:

		constexpr static uint16_t s_MaxChars = 256;

		Font();

		void renderText(const std::string &text, chcl::Vector2<float> pos);

		Ref<VertexArray> generateVertexArray(const std::string &text);
		Ref<IndexBuffer> generateIndexBuffer(const std::string &text);

		Ref<VertexBufferLayout> getLayout();
		inline Ref<ShaderStorageBuffer> getFontBuffer() { return m_contourPoints; }
		inline uint16_t getUnitsPerEm() const { return m_uPerEm; }

	private:

		struct SimpleGlyph
		{
			int16_t xMin, yMin, xMax, yMax;
			uint16_t advanceWidth;
			int16_t leftSideBearing;
			uint32_t contourBegin;
			uint32_t numContourPoints;
		};

		/**
		 * @brief Buffer containing points of all contours of glyphs
		 * 
		 * Each point is comprised of an x and y coordinate stored as 16-bit integers, and a four-byte integer of flags (for alignment purposes).
		 * The contents of the flags byte is as follows:
		 * - bit 0: if set, the point is on the curve
		 * - bit 1: if set, this is the last point of a contour
		 */
		Ref<ShaderStorageBuffer> m_contourPoints;

		int16_t m_lineGap = 0;
		uint16_t m_uPerEm = 0;
		// Map of character to glyph index
		std::unordered_map<char, size_t> m_charMapping;
		std::vector<SimpleGlyph> m_glyphs;

		static Ref<VertexBufferLayout> s_VBLayout;
		static Ref<Shader> s_fontShader;

		friend class TTFLoader;
	};
}