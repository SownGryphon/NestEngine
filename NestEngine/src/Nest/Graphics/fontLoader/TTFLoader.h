#pragma once

#include <string>
#include <unordered_map>

#include <dataStorage/BinaryFile.h>

#include "Nest/Core/Buffer.h"
#include "Nest/Core/DataType.h"
#include "Nest/graphics/Font.h"

namespace Nest
{
	class TTFLoader
	{
	public:

		TTFLoader(const std::string &filename);

		inline Ref<Font> get() { return m_result; }

	private:

		struct TableInfo
		{
			uint32_t offset = 0, length = 0;
		};

		struct ContourPoint
		{
			float x = 0.f, y = 0.f;
			int32_t flags = 0;
		};

		struct GlyphHorMetrics
		{
			uint16_t advanceWidth;
			int16_t leftSideBearing;
		};

		Ref<Font> m_result;
		chcl::BinaryFile m_fontFile;

		TableInfo headInfo;
		TableInfo maxpInfo;
		TableInfo locaInfo;
		TableInfo glyfInfo;
		TableInfo cmapInfo;
		TableInfo hheaInfo;
		TableInfo htmxInfo;

		uint16_t m_numGlyphs = 0;
		std::vector<uint32_t> m_glyphOffsets;
		DataType m_locaDataType;

		uint16_t m_hheaNumHorMetrs = 0;
		std::vector<GlyphHorMetrics> m_glyphHorMetrics;

		void processHead();
		void processMaxp();
		void processLoca();
		void processGlyf();
		void processCmap();
		void processHhea();
		void processHtmx();

		void optimiseFont();

		std::vector<ContourPoint> readGlyph(std::vector<ContourPoint> &allContourPoints);
		std::vector<ContourPoint> readSimpleGlyph(int16_t numContours);
		std::vector<ContourPoint> readCompoundGlyph(std::vector<ContourPoint> &allContourPoints);
	};
}