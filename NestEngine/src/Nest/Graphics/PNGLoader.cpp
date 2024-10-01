#include "PNGLoader.h"

#include <dataStorage/BitStream.h>
#include <dataStorage/HuffmanTree.h>

static const uint8_t pngHeader[8] = { UINT8_C(137), UINT8_C(80), UINT8_C(78), UINT8_C(71), UINT8_C(13), UINT8_C(10), UINT8_C(26), UINT8_C(10) };
static const uint8_t ihdrType[4] = { 'I', 'H', 'D', 'R' };
static const uint8_t plteType[4] = { 'P', 'L', 'T', 'E' };
static const uint8_t idatType[4] = { 'I', 'D', 'A', 'T' };
static const uint8_t iendType[4] = { 'I', 'E', 'N', 'D' };

static bool g_staticTreesGenerated = false;
static chcl::HuffmanTree<uint16_t> g_staticLitTree;
static chcl::HuffmanTree<uint8_t> g_staticDistTree;

Nest::PNGLoader::PNGLoader(const std::string &filepath) :
	m_fileStream(filepath, std::ios_base::in | std::ios_base::binary)
{}

Nest::Image Nest::PNGLoader::operator()()
{
	uint8_t header[8];
	m_fileStream.read((char*)header, 8);

	if (!CompBytes(header, pngHeader, 8))
		return Image();

	Chunk ihdr = readChunk();

	if (!CompBytes(ihdr.chunkType, ihdrType, 4))
		return Image();

	struct ImageInfo {
		uint32_t imageWidth, imageHeight;
		uint8_t bitDepth, colorType, compressionType, filterType, interlaceType;
	} imageInfo = *(ImageInfo*)ihdr.chunkData.begin();
	ReverseByteOrder((uint8_t*)&imageInfo.imageWidth, 4);
	ReverseByteOrder((uint8_t*)&imageInfo.imageHeight, 4);

	if (imageInfo.colorType == 4)
	{
		NE_WARN("Nest does not support transparent grayscale images.");
		return Image();
	}

	if (imageInfo.interlaceType)
	{
		NE_ASSERT(0, "Nest does not currently support interlaced PNGs.");
	}

	Buffer palette;

	Buffer compressedData;

	while (!m_fileStream.eof())
	{
		Chunk chunk = readChunk();

		if (CompBytes(chunk.chunkType, iendType, 4))
			break;

		if (CompBytes(chunk.chunkType, plteType, 4))
		{
			palette.push_back(chunk.chunkData.begin(), chunk.chunkSize);
			continue;
		}

		if (CompBytes(chunk.chunkType, idatType, 4))
		{
			compressedData.reserve(compressedData.size() + chunk.chunkSize, true);
			compressedData.push_back(chunk.chunkData.begin(), chunk.chunkSize);
		}
	}

	uint8_t filteredDataChannels = 1;
	if (imageInfo.colorType != 3)
	{
		// Bit 1 is true if using color
		if (imageInfo.colorType & 0x2)
			filteredDataChannels += 2;
		// Bit 2 is true if using alpha
		if (imageInfo.colorType & 0x4)
			filteredDataChannels += 1;
	}
	size_t filteredScanlineLen = (filteredDataChannels * imageInfo.imageWidth * imageInfo.bitDepth + 7) / 8;
	size_t filteredDataSize = filteredScanlineLen * imageInfo.imageHeight;
	// The start of each scanline includes one byte for filter type
	filteredDataSize += imageInfo.imageHeight;
	Buffer filteredData{ filteredDataSize };

	Decompress(compressedData, filteredData);

	size_t imageSize;
	uint8_t pixelChannels = 1;
	if (imageInfo.colorType == 3)
	{
		imageSize = 3 * imageInfo.imageWidth * imageInfo.imageHeight;
		pixelChannels = 3;
	}
	else
	{
		// Bit 1 is true if using color
		if (imageInfo.colorType & 0x2)
			pixelChannels += 2;
		// Bit 2 is true if using alpha
		if (imageInfo.colorType & 0x4)
			pixelChannels += 1;
		imageSize = (imageInfo.bitDepth + 7) / 8 * pixelChannels * imageInfo.imageWidth * imageInfo.imageHeight;
	}

	uint8_t bpp = (pixelChannels * imageInfo.bitDepth + 7) / 8;
	if (imageInfo.colorType == 3)
		bpp = 1;

	Buffer imageData{ imageSize };

	for (size_t scanline = 0; scanline < imageInfo.imageHeight; ++scanline)
	{
		uint8_t *scanlineDataBegin = filteredData.begin() + scanline * (filteredScanlineLen + 1) + 1;
		uint8_t scanlineFilter = *(scanlineDataBegin - 1);

		size_t pixelsWritten = 0;
		for (size_t i = 0; i < filteredScanlineLen; ++i)
		{
			// Unfilter current byte
			Unfilter(scanlineDataBegin + i, scanlineFilter, i, scanline, bpp, filteredScanlineLen);

			if (imageInfo.bitDepth < 8)
			{
				// Pixels per byte
				uint8_t ppb = 8 / imageInfo.bitDepth;

				chcl::BitStream reversed;
				for (uint8_t j = 0; j < ppb; ++j)
				{
					reversed.pushBits(scanlineDataBegin + i, imageInfo.bitDepth, imageInfo.bitDepth * j);
				}

				chcl::BitStreamView reversedView(reversed.data(), 0);
				for (uint8_t j = 0; j < ppb && pixelsWritten < imageInfo.imageWidth; ++j, ++pixelsWritten)
				{
					if (imageInfo.colorType == 3)
					{
						uint16_t plteIndex = reversedView.readBits<uint16_t>(imageInfo.bitDepth) * 3;
						imageData.push_back(palette.begin() + plteIndex, 3);
					}
					else
						imageData.push_back((uint8_t)(reversedView.readBits<uint16_t>(imageInfo.bitDepth) * 255 / ((1 << imageInfo.bitDepth) - 1)));
				}
				continue;
			}
		
			if (imageInfo.colorType == 3)
			{
				uint16_t plteIndex = (uint16_t)scanlineDataBegin[i] * 3;
				imageData.push_back(palette.begin() + plteIndex, 3);
				continue;
			}
		}

		if (imageInfo.bitDepth >= 8 && imageInfo.colorType != 3)
			imageData.push_back(scanlineDataBegin, filteredScanlineLen);
	}

	DataType dataType = DataType::UnsignedByte;
	if (imageInfo.bitDepth == 16)
		dataType = DataType::UnsignedShort;
	m_fileStream.close();
	return Image(imageInfo.imageWidth, imageInfo.imageHeight, pixelChannels, dataType, imageData, ScanlineDirection::TopToBottom);
}

Nest::PNGLoader::Chunk Nest::PNGLoader::readChunk()
{
	Chunk result;
	m_fileStream.read((char*)&result.chunkSize, 4);
	ReverseByteOrder((uint8_t*)&result.chunkSize, 4);
	m_fileStream.read((char*)result.chunkType, 4);
	if (result.chunkSize)
	{
		result.chunkData.reserve(result.chunkSize, false);
		m_fileStream.read((char*)result.chunkData.begin(), result.chunkSize);
	}
	m_fileStream.read((char*)&result.crc, 4);
	return result;
}

bool Nest::PNGLoader::CompBytes(const uint8_t *lhs, const uint8_t *rhs, size_t bytes)
{
	for (size_t i = 0; i < bytes; ++i)
		if (lhs[i] != rhs[i])
			return false;
	return true;
}

void Nest::PNGLoader::ReverseByteOrder(uint8_t *data, size_t size)
{
	for (size_t i = 0; i < size / 2; ++i)
	{
		std::swap(*(data + i), *(data + size - i - 1));
	}
}

void Nest::PNGLoader::Decompress(const Buffer &compressedStream, Buffer &filteredData)
{
	chcl::BitStreamView chunkView((const uint8_t*)compressedStream.begin(), 0);

	uint8_t cmf = chunkView.readBits(4);
	if (cmf != 8)
		NE_WARN("PNG has invalid compression method.");

	uint8_t windowSizeLog = chunkView.readBits(4) + 8;
	uint16_t windowSize = 1 << windowSizeLog;

	// Skip over the FCHECK part of the FLG byte
	chunkView.jump(5);

	bool fdict = chunkView.readBit();
	uint8_t flevel = chunkView.readBits(2);

	if (fdict)
		chunkView.jump(32);

	bool finalBlock = false;
	while (!finalBlock)
	{
		finalBlock = chunkView.readBit();
		uint8_t btype = chunkView.readBits(2);

		switch (btype)
		{
			// No compression
			case 0:
			{
				chunkView.skipTrailingBits();

				uint16_t len = chunkView.readBits<uint16_t>(16);
				uint16_t nlen = chunkView.readBits<uint16_t>(16);
				ReverseByteOrder((uint8_t*)&len, 2);

				filteredData.push_back(chunkView.getPos(), len);
				break;
			}
			// Static Huffman codes
			case 1:
			{
				if (!g_staticTreesGenerated)
				{
					std::vector<uint16_t> staticLitLengths;
					staticLitLengths.reserve(288);
					staticLitLengths.insert(staticLitLengths.end(), 144, 8);
					staticLitLengths.insert(staticLitLengths.end(), 112, 9);
					staticLitLengths.insert(staticLitLengths.end(), 24, 7);
					staticLitLengths.insert(staticLitLengths.end(), 8, 8);

					g_staticLitTree = chcl::HuffmanTree<uint16_t>(staticLitLengths);

					std::vector<uint8_t> staticDistLengths;
					staticDistLengths.insert(staticDistLengths.end(), 31, 5);
					g_staticDistTree = chcl::HuffmanTree<uint8_t>(staticDistLengths);

					g_staticTreesGenerated = true;
				}

				Deflate(filteredData, chunkView, g_staticLitTree, g_staticDistTree);

				break;
			}
			// Dynamic Huffman codes
			case 2:
			{
				uint16_t hlit = (uint16_t)chunkView.readBits(5) + 257;
				uint8_t hdist = chunkView.readBits(5) + 1;
				uint8_t hclen = chunkView.readBits(4) + 4;

				static uint8_t codeLengthCodeOrder[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
				std::vector<uint8_t> codeLengthCodeLengths;
				codeLengthCodeLengths.resize(19);

				for (uint8_t i = 0; i < hclen; ++i)
				{
					chunkView.readBits(&codeLengthCodeLengths[codeLengthCodeOrder[i]], 3);
				}

				chcl::HuffmanTree<uint8_t> codeLengthTree{ codeLengthCodeLengths };

				chcl::HuffmanTree<uint16_t> literalTree = UnpackHuffmanTree(hlit, chunkView, codeLengthTree);
				chcl::HuffmanTree<uint8_t> distanceTree = UnpackHuffmanTree(hdist, chunkView, codeLengthTree);

				Deflate(filteredData, chunkView, literalTree, distanceTree);

				break;
			}
		}
	}
	chunkView.skipTrailingBits();
	// Skip over ADLER32
	chunkView.jump(32);
}

void Nest::PNGLoader::Deflate(Buffer &filteredData, chcl::BitStreamView &chunkView, const chcl::HuffmanTree<uint16_t> &litLenTree, const chcl::HuffmanTree<uint8_t> &distTree)
{
	bool blockEnd = false;
	while (!blockEnd)
	{
		chcl::BitStream litLenHufCode;
		do
		{
			litLenHufCode.pushBit(chunkView.readBit());
		}
		while (!litLenTree.isLeaf(litLenHufCode));

		uint16_t litLenCode = litLenTree.traverse(litLenHufCode);

		if (litLenCode < 256)
		{
			filteredData.push_back((uint8_t)litLenCode);
			continue;
		}

		if (litLenCode == 256)
		{
			blockEnd = true;
			break;
		}

		uint8_t extraLenBits = litLenCode < 265 ? 0 : (uint8_t)(litLenCode - 261) / 4;

		if (litLenCode == 285)
			extraLenBits = 0;

		uint16_t len = chunkView.readBits<uint16_t>(extraLenBits) + 3;
		if (extraLenBits == 0)
			len += (uint8_t)(litLenCode - 257);
		else
		{
			len += 0x4 << extraLenBits;
			len += ((litLenCode - 257) & 0x3) * (1 << extraLenBits);
		}
		if (litLenCode == 285)
			len = 258;

		chcl::BitStream distHufCode;
		do
		{
			distHufCode.pushBit(chunkView.readBit());
		}
		while (!distTree.isLeaf(distHufCode));
		uint8_t distCode = distTree.traverse(distHufCode);
		uint8_t extraDistBits = distCode < 2 ? 0 : (distCode - 2) / 2;

		uint32_t dist = chunkView.readBits<uint32_t>(extraDistBits) + 1;
		if (extraDistBits == 0)
			dist += distCode;
		else
		{
			dist += 0x2 << extraDistBits;
			dist += (distCode & 0x1) * (0x1 << extraDistBits);
		}

		for (size_t i = 0; i < len / dist; ++i)
		{
			filteredData.push_back(filteredData.end() - dist, dist);
		}
		filteredData.push_back(filteredData.end() - dist, len % dist);
	}
}

void Nest::PNGLoader::Unfilter(uint8_t *byte, uint8_t filter, size_t pixelX, size_t pixelY, uint8_t bpp, size_t scanlineLength)
{
	switch (filter)
	{
		// None
		case 0:
			break;
		// Sub
		case 1:
		{
			if (pixelX >= bpp)
				*byte += *(byte - bpp);
			return;
		}
		// Up
		case 2:
		{
			if (pixelY > 0)
				*byte += *(byte - scanlineLength - 1);
			return;
		}
		// Average
		case 3:
		{
			uint8_t left = 0, up = 0;

			if (pixelX >= bpp)
				left = *(byte - bpp);
			if (pixelY > 0)
				up = *(byte - scanlineLength - 1);

			*byte += (left + up) / 2;
			return;
		}
		// Path
		case 4:
		{
			uint8_t left = 0, up = 0, ul = 0;

			if (pixelX >= bpp)
				left = *(byte - bpp);
			if (pixelY > 0)
				up = *(byte - scanlineLength - 1);
			if (pixelY > 0 && pixelX >= bpp)
				ul = *(byte - scanlineLength - bpp - 1);

			int16_t p = (int16_t)left + up - ul;
			int16_t pa = abs(p - left);
			int16_t pb = abs(p - up);
			int16_t pc = abs(p - ul);

			uint8_t paeth = ul;
			if (pa <= pb && pa <= pc)
				paeth = left;
			else if (pb <= pc)
				paeth = up;

			*byte += paeth;
			return;
		}
	}
}