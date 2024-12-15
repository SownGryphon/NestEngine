#include "TTFLoader.h"

constexpr uint16_t TTF_ON_CURVE_BIT = BIT(0),
	TTF_X_SHORT_BIT = BIT(1),
	TTF_Y_SHORT_BIT = BIT(2),
	TTF_REPEAT_BIT = BIT(3),
	TTF_X_SAME_BIT = BIT(4),
	TTF_Y_SAME_BIT = BIT(5),
	TTF_ARG_12_WORD_BIT = BIT(0),
	TTF_ARG_XY_BIT = BIT(1),
	TTF_SCALE_BIT = BIT(3),
	TTF_MORE_COMP_BIT = BIT(5),
	TTF_XY_SCALE_BIT = BIT(6),
	TTF_2x2_BIT = BIT(7),
	TTF_INSTRUCTION_BIT = BIT(8),
	NE_ON_CURVE_BIT = BIT(0),
	NE_CONTOUR_END_BIT = BIT(1);

Nest::TTFLoader::TTFLoader(const std::string &m_fontFilename) :
	m_fontFile(m_fontFilename, chcl::Endianness::Big),
	m_locaDataType(DataType::None)
{
	m_result = createRef<Font>();

	if (!m_fontFile.is_open())
	{
		NE_ASSERT(0, "Invalid font file.");
		return;
	}

	uint32_t scalerType = m_fontFile.readInt<uint32_t>();
	uint16_t numTables = m_fontFile.readInt<uint16_t>(),
		searchRange = m_fontFile.readInt<uint16_t>(),
		entrySelector = m_fontFile.readInt<uint16_t>(),
		rangeShift = m_fontFile.readInt<uint16_t>();

	if (scalerType != 0x00010000)
		return;

	for (uint16_t i = 0; i < numTables; ++i)
	{
		uint32_t tag = 0;
		m_fontFile.read((char*)&tag, 4);
		uint32_t checksum = m_fontFile.readInt<uint32_t>(),
			offset = m_fontFile.readInt<uint32_t>(),
			length = m_fontFile.readInt<uint32_t>();

		switch (tag)
		{
			case 0x64616568: // head
				headInfo.offset = offset;
				headInfo.length = length;
				break;
			case 0x7078616d: // maxp
				maxpInfo.offset = offset;
				maxpInfo.length = length;
				break;
			case 0x61636f6c: // loca
				locaInfo.offset = offset;
				locaInfo.length = length;
				break;
			case 0x66796c67: // glyf
				glyfInfo.offset = offset;
				glyfInfo.length = length;
				break;
			case 0x70616d63: // cmap
				cmapInfo.offset = offset;
				cmapInfo.length = length;
				break;
			case 0x61656868: // hhea
				hheaInfo.offset = offset;
				hheaInfo.length = length;
				break;
			case 0x78746d68: // htmx
				htmxInfo.offset = offset;
				htmxInfo.length = length;
				break;
		}
	}

	processMaxp();

	processHead();
	processLoca();

	if (htmxInfo.length)
	{
		processHhea();
		processHtmx();
	}

	processGlyf();

	processCmap();

	m_fontFile.close();
}

void Nest::TTFLoader::processHead()
{
	constexpr size_t unitsPerEmOffset = 18;
	constexpr size_t indexToLocFormatOffset = 50;

	m_fontFile.seekg(headInfo.offset + unitsPerEmOffset);
	m_result->m_uPerEm = m_fontFile.readInt<uint16_t>();
	m_fontFile.seekg(headInfo.offset + indexToLocFormatOffset);
	m_locaDataType = m_fontFile.readInt<int16_t>() ? DataType::UnsignedInt : DataType::UnsignedShort;
}

void Nest::TTFLoader::processMaxp()
{
	m_fontFile.seekg(maxpInfo.offset);
	m_fontFile.seekg(4, std::ifstream::cur);

	m_numGlyphs = m_fontFile.readInt<uint16_t>();
	m_result->m_glyphs.reserve(m_numGlyphs);
}

void Nest::TTFLoader::processLoca()
{
	m_fontFile.seekg(locaInfo.offset);

	m_glyphOffsets.resize(m_numGlyphs + 1);
	for (size_t i = 0; i < m_numGlyphs + 1; ++i)
	{
		if (m_locaDataType == DataType::UnsignedShort)
		{
			m_glyphOffsets[i] = (uint32_t)m_fontFile.readInt<uint16_t>() * 2;
		}
		else
		{
			m_glyphOffsets[i] = m_fontFile.readInt<uint32_t>();
		}
	}

	return;
}

void Nest::TTFLoader::processCmap()
{
	m_fontFile.seekg(cmapInfo.offset);

	uint16_t version = m_fontFile.readInt<uint16_t>();
	uint16_t numSubtables = m_fontFile.readInt<uint16_t>();

	struct CmapSubtable
	{
		uint16_t platformID,
			platformSpecificID;
		uint32_t offset;
	};

	std::vector<CmapSubtable> subtables;

	for (uint16_t i = 0; i < numSubtables; ++i)
	{
		subtables.push_back({
			m_fontFile.readInt<uint16_t>(),	// platformID
			m_fontFile.readInt<uint16_t>(),	// platformSpecificID
			m_fontFile.readInt<uint32_t>()	// offset
		});
	}

	CmapSubtable usedSubtable = subtables[0];

	m_fontFile.seekg(cmapInfo.offset + usedSubtable.offset);

	uint16_t format = m_fontFile.readInt<uint16_t>();

	switch (format)
	{
		case 0:
			m_fontFile.seekg(4, chcl::BinaryFile::cur);
			for (unsigned int i = 0; i < 256; ++i)
			{
				m_result->m_charMapping[i] = m_fontFile.readInt<uint8_t>();
			}
			break;
		case 4:
		{
			uint16_t length = m_fontFile.readInt<uint16_t>();
			uint16_t lang = m_fontFile.readInt<uint16_t>();
			uint16_t segCount = m_fontFile.readInt<uint16_t>() / 2;
			uint16_t searchRange = m_fontFile.readInt<uint16_t>();
			uint16_t entrySelector = m_fontFile.readInt<uint16_t>();
			uint16_t rangeShift = m_fontFile.readInt<uint16_t>();

			std::vector<uint16_t> endCode, startCode, idRangeOffset;
			std::vector<int16_t> idDelta;
			endCode.reserve(segCount);
			startCode.reserve(segCount);
			idDelta.reserve(segCount);
			idRangeOffset.reserve(segCount);

			for (uint16_t i = 0; i < segCount; ++i)
			{
				endCode.push_back(m_fontFile.readInt<uint16_t>());
			}

			uint16_t cmapReservePad = m_fontFile.readInt<uint16_t>();

			for (uint16_t i = 0; i < segCount; ++i)
			{
				startCode.push_back(m_fontFile.readInt<uint16_t>());
			}

			for (uint16_t i = 0; i < segCount; ++i)
			{
				idDelta.push_back(m_fontFile.readInt<uint16_t>());
			}

			for (uint16_t i = 0; i < segCount; ++i)
			{
				idRangeOffset.push_back(m_fontFile.readInt<uint16_t>());
				
				if (endCode[i] == 0xffff && startCode[i] == 0xffff)
					break;

				for (uint16_t j = startCode[i]; j <= endCode[i] && j < Font::s_MaxChars; ++j)
				{

					if (idRangeOffset[i] == 0)
						m_result->m_charMapping[(char)j] = (size_t)j + idDelta[i];
					else
					{
						size_t byteOffset = ((size_t)idRangeOffset[i] / 2 + j - startCode[i]) * 2;

						std::streamoff prevPos = m_fontFile.tellg();
						m_fontFile.seekg(byteOffset - 2, std::ifstream::cur);
						uint16_t glyphID = m_fontFile.readInt<uint16_t>();
						if (glyphID != 0)
							m_result->m_charMapping[(char)j] = (size_t)glyphID + idDelta[i];
						m_fontFile.seekg(prevPos);
					}
				}
			}

			break;
		}
		default:
			NE_ASSERT(0, "Unknown cmap format (or havent implemented it :) ).")
			break;
	}
}

void Nest::TTFLoader::processGlyf()
{
	m_fontFile.seekg(glyfInfo.offset);

	std::vector<ContourPoint> allContourPoints;

	for (uint16_t i = 0; i < m_numGlyphs; ++i)
	{
		uint16_t glyphLen = m_glyphOffsets[i + 1] - m_glyphOffsets[i];

		if (glyphLen == 0)
		{
			m_result->m_glyphs.push_back({
				0, 0, 0, 0,
				m_glyphHorMetrics[i].advanceWidth,
				m_glyphHorMetrics[i].leftSideBearing,
				(uint32_t)allContourPoints.size(), 0
			});

			continue;
		}

		m_fontFile.seekg(glyfInfo.offset + m_glyphOffsets[i]);

		int16_t numContours = m_fontFile.readInt<int16_t>();
		int16_t xMin = m_fontFile.readInt<int16_t>(),
			yMin = m_fontFile.readInt<int16_t>(),
			xMax = m_fontFile.readInt<int16_t>(),
			yMax = m_fontFile.readInt<int16_t>();

		if (numContours == 0)
		{
			m_result->m_glyphs.push_back({
				xMin, yMin, xMax, yMax,
				m_glyphHorMetrics[i].advanceWidth,
				m_glyphHorMetrics[i].leftSideBearing,
				(uint32_t)allContourPoints.size(), 0
			});

			continue;
		}

		// Break at a specific glyph
		NE_ASSERT(i != -1, "DEBUG :]");

		std::vector<ContourPoint> contourPoints;
		if (numContours > 0)
		{
			contourPoints = std::move(readSimpleGlyph(numContours));
		}
		else
		{
			contourPoints = std::move(readCompoundGlyph(allContourPoints));
		}

		m_result->m_glyphs.push_back({
			xMin, yMin, xMax, yMax,
			m_glyphHorMetrics[i].advanceWidth,
			m_glyphHorMetrics[i].leftSideBearing,
			(uint32_t)allContourPoints.size(), (uint32_t)contourPoints.size()
		});
		allContourPoints.insert(allContourPoints.end(), contourPoints.begin(), contourPoints.end());
	}

	m_result->m_contourPoints = createRef<ShaderStorageBuffer>(allContourPoints.size() * sizeof(ContourPoint), (const char*)allContourPoints.data());
}

void Nest::TTFLoader::processHhea()
{
	m_fontFile.seekg(hheaInfo.offset + 8, chcl::BinaryFile::beg);
	m_result->m_lineGap = m_fontFile.readInt<int16_t>();
	// Offset of numLongHorMetrics entry
	m_fontFile.seekg(hheaInfo.offset + 34, chcl::BinaryFile::beg);
	m_hheaNumHorMetrs = m_fontFile.readInt<uint16_t>();
}

void Nest::TTFLoader::processHtmx()
{
	m_fontFile.seekg(htmxInfo.offset);

	m_glyphHorMetrics.reserve(m_numGlyphs);

	for (uint16_t i = 0; i < m_hheaNumHorMetrs; ++i)
	{
		m_glyphHorMetrics.push_back({
			m_fontFile.readInt<uint16_t>(),
			m_fontFile.readInt<int16_t>()
		});
	}

	uint16_t monoAdvanceWidth = m_glyphHorMetrics.back().advanceWidth;

	for (uint16_t i = m_hheaNumHorMetrs; i < m_numGlyphs; ++i)
	{
		m_glyphHorMetrics.push_back({
			monoAdvanceWidth,
			m_fontFile.readInt<int16_t>()
		});
	}
}

std::vector<Nest::TTFLoader::ContourPoint> Nest::TTFLoader::readGlyph(std::vector<ContourPoint> &allContourPoints)
{
	// All glyphs are word-aligned
	if (std::streamoff(m_fontFile.tellg()) % 2 == 1)
		m_fontFile.seekg(1, std::ifstream::cur);

	int16_t numContours = m_fontFile.readInt<int16_t>();
	int16_t xMin = m_fontFile.readInt<int16_t>(),
		yMin = m_fontFile.readInt<int16_t>(),
		xMax = m_fontFile.readInt<int16_t>(),
		yMax = m_fontFile.readInt<int16_t>();

	if (numContours == 0)
	{
		return {};
	}

	std::vector<ContourPoint> contourPoints;
	if (numContours > 0)
	{
		return readSimpleGlyph(numContours);
	}
	
	return readCompoundGlyph(allContourPoints);
}

std::vector<Nest::TTFLoader::ContourPoint> Nest::TTFLoader::readSimpleGlyph(int16_t numContours)
{
	std::vector<uint16_t> contourEnds;
	for (int16_t i = 0; i < numContours; ++i)
	{
		contourEnds.push_back(m_fontFile.readInt<uint16_t>());
	}
	uint16_t numPoints = contourEnds.back() + 1;

	std::vector<ContourPoint> contourPoints;
	contourPoints.resize(numPoints);

	uint16_t instructionsLen = m_fontFile.readInt<uint16_t>();

	m_fontFile.seekg(instructionsLen, std::ifstream::cur);

	std::vector<uint8_t> flags;

	for (uint16_t numFlags = 0; numFlags < numPoints; ++numFlags)
	{
		uint8_t flag = m_fontFile.readInt<uint8_t>();

		flags.push_back(flag);

		if (flag & TTF_REPEAT_BIT)
		{
			uint8_t repeatCount = m_fontFile.readInt<uint8_t>();
			flags.insert(flags.end(), repeatCount, flag);
			numFlags += repeatCount;
		}
	}

	uint16_t currentContour = 0;

	for (size_t i = 0; i < flags.size(); ++i)
	{
		if (flags[i] & TTF_ON_CURVE_BIT)
			contourPoints[i].flags |= NE_ON_CURVE_BIT;

		if (i == contourEnds[currentContour])
		{
			++currentContour;
			contourPoints[i].flags |= NE_CONTOUR_END_BIT;
		}

		bool xShort = flags[i] & TTF_X_SHORT_BIT;
		bool xSame = flags[i] & TTF_X_SAME_BIT;

		if (xShort)
		{
			int16_t xOff = m_fontFile.readInt<uint8_t>();
			if (!xSame)
				xOff *= -1;

			if (i > 0)
				contourPoints[i].x = contourPoints[i - 1].x + xOff;
			else
				contourPoints[i].x = xOff;
		}
		else if (xSame)
		{
			if (i > 0)
				contourPoints[i].x = contourPoints[i - 1].x;
			else
				contourPoints[i].x = 0;
		}
		else
		{
			int16_t xOff = m_fontFile.readInt<int16_t>();
			if (i > 0)
				contourPoints[i].x = contourPoints[i - 1].x + xOff;
			else
				contourPoints[i].x = xOff;
		}
	}

	for (size_t i = 0; i < flags.size(); ++i)
	{
		bool yShort = flags[i] & TTF_Y_SHORT_BIT;
		bool ySame = flags[i] & TTF_Y_SAME_BIT;

		if (yShort)
		{
			int16_t yOff = m_fontFile.readInt<uint8_t>();
			if (!ySame)
				yOff *= -1;

			if (i > 0)
				contourPoints[i].y = contourPoints[i - 1].y + yOff;
			else
				contourPoints[i].y = yOff;
		}
		else if (ySame)
		{
			if (i > 0)
				contourPoints[i].y = contourPoints[i - 1].y;
			else
				contourPoints[i].y = 0;
		}
		else
		{
			int16_t yOff = m_fontFile.readInt<int16_t>();
			if (i > 0)
				contourPoints[i].y = contourPoints[i - 1].y + yOff;
			else
				contourPoints[i].y = yOff;
		}
	}

	auto tryCreateMidpoint = [&contourPoints](size_t &pointIndex, const ContourPoint &p1, const ContourPoint &p2)
	{
		// Check if both point are on or both off the curve
		if (~(p1.flags ^ p2.flags) & NE_ON_CURVE_BIT)
		{
			ContourPoint midpoint;
			midpoint.x = p1.x / 2 + p2.x / 2;
			midpoint.y = p1.y / 2 + p2.y / 2;
			// Use previous point, since that one can't be the end of a contour
			midpoint.flags = p1.flags ^ NE_ON_CURVE_BIT;
			contourPoints.insert(contourPoints.begin() + pointIndex, midpoint);
			++pointIndex;
		}
	};

	size_t contourBeginIndex = 0;
	for (size_t i = 1; i < contourPoints.size(); ++i)
	{
		ContourPoint point = contourPoints[i];

		bool newContour = point.flags & NE_CONTOUR_END_BIT;

		if (newContour)
		{
			contourPoints[i].flags ^= NE_CONTOUR_END_BIT;
		}

		tryCreateMidpoint(i, contourPoints[i - 1], point);

		if (newContour)
		{
			++i;
			tryCreateMidpoint(i, contourPoints[contourBeginIndex], point);

			// Manually loop around the glyph contour
			contourPoints.insert(contourPoints.begin() + i, contourPoints[contourBeginIndex]);
			contourPoints[i].flags |= NE_CONTOUR_END_BIT;

			// Set up the loop for the next contour
			contourBeginIndex = i + 1;
			++i;
		}
	}

	//for (ContourPoint &point : contourPoints)
	//{
	//	point.flags &= NE_CONTOUR_END_BIT;
	//}

	return contourPoints;
}

std::vector<Nest::TTFLoader::ContourPoint> Nest::TTFLoader::readCompoundGlyph(std::vector<ContourPoint> &allContourPoints)
{
	std::vector<ContourPoint> contourPoints;

	bool hasInstructions = false;
	bool moreComponents = true;
	while (moreComponents)
	{
		uint16_t flags = m_fontFile.readInt<uint16_t>();

		if (!(flags & TTF_MORE_COMP_BIT))
			moreComponents = false;

		if (flags & TTF_INSTRUCTION_BIT)
			hasInstructions = true;

		uint16_t componentIndex = m_fontFile.readInt<uint16_t>();

		Font::SimpleGlyph componentGlyph = {};
		ContourPoint *componentPointBeg = nullptr;
		std::vector<ContourPoint> tempGlyphPoints;

		if (componentIndex < m_result->m_glyphs.size())
		{
			componentGlyph = m_result->m_glyphs[componentIndex];
			componentPointBeg = allContourPoints.data() + componentGlyph.contourBegin;
		}
		else
		{
			std::streampos prevPos = m_fontFile.tellg();
			m_fontFile.seekg(glyfInfo.offset + m_glyphOffsets[componentIndex]);

			tempGlyphPoints = std::move(readGlyph(allContourPoints));

			componentGlyph.numContourPoints = (uint32_t)tempGlyphPoints.size();
			componentPointBeg = tempGlyphPoints.data();

			m_fontFile.seekg(prevPos);
		}

		bool argsAreWords = flags & TTF_ARG_12_WORD_BIT;
		bool argsAreXY = flags & TTF_ARG_XY_BIT;

		int16_t xOff = 0, yOff = 0;
		uint16_t parentIndex = 0;

		if (argsAreXY)
		{
			if (argsAreWords)
			{
				xOff = m_fontFile.readInt<int16_t>();
				yOff = m_fontFile.readInt<int16_t>();
			}
			else
			{
				xOff = m_fontFile.readInt<int8_t>();
				yOff = m_fontFile.readInt<int8_t>();
			}
		}
		else
		{
			uint16_t childIndex = 0;
			if (argsAreWords)
			{
				parentIndex = m_fontFile.readInt<uint16_t>();
				childIndex = m_fontFile.readInt<uint16_t>();
			}
			else
			{
				parentIndex = m_fontFile.readInt<uint8_t>();
				childIndex = m_fontFile.readInt<uint8_t>();
			}

			xOff = int16_t(contourPoints[parentIndex].x - (componentPointBeg + childIndex)->x);
			yOff = int16_t(contourPoints[parentIndex].y - (componentPointBeg + childIndex)->y);

			//xOff = (componentPointBeg + childIndex)->x;
			//yOff = (componentPointBeg + childIndex)->y;
		}

		float xScale = 1.f, scale01 = 0.f, scale10 = 0.f, yScale = 1.f;

		bool hasScale = flags & TTF_SCALE_BIT;
		bool hasXYScale = flags & TTF_XY_SCALE_BIT;
		bool has2x2Scale = flags & TTF_2x2_BIT;

		if (hasScale)
		{
			xScale = yScale = m_fontFile.readInt<int16_t>() / float(0x4000);
		}
		else if (hasXYScale)
		{
			xScale = m_fontFile.readInt<int16_t>() / float(0x4000);
			yScale = m_fontFile.readInt<int16_t>() / float(0x4000);
		}
		else if (has2x2Scale)
		{
			xScale = m_fontFile.readInt<int16_t>() / float(0x4000);
			scale01 = m_fontFile.readInt<int16_t>() / float(0x4000);
			scale10 = m_fontFile.readInt<int16_t>() / float(0x4000);
			yScale = m_fontFile.readInt<int16_t>() / float(0x4000);
		}

		//if (!argsAreXY)
		//{
		//	float xTemp = xScale * xOff + scale10 * yOff;
		//	float yTemp = scale01 * xOff + yScale * yOff;

		//	xOff = contourPoints[parentIndex].x - xTemp;
		//	yOff = contourPoints[parentIndex].y - yTemp;
		//}

		contourPoints.reserve(contourPoints.size() + componentGlyph.numContourPoints);
		for (size_t i = 0; i < componentGlyph.numContourPoints; ++i)
		{
			ContourPoint tempPoint = *(componentPointBeg + i);
			float xTemp = xScale * tempPoint.x + scale10 * tempPoint.y + yOff,
				yTemp = scale01 * tempPoint.x + yScale * tempPoint.y + yOff;

			contourPoints.push_back({ xTemp, yTemp, tempPoint.flags });
		}
	}

	if (hasInstructions)
	{
		uint16_t numInstructions = m_fontFile.readInt<uint16_t>();
		m_fontFile.seekg(numInstructions, std::ifstream::cur);
	}

	return contourPoints;
}