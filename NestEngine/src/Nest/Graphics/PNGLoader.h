#pragma once

#include <fstream>
#include <string>

#include <dataStorage/BitStreamView.h>
#include <dataStorage/HuffmanTree.h>

#include "Image.h"

namespace Nest
{

	class PNGLoader
	{
	public:
		PNGLoader(const std::string &path);

		Image operator()();

	private:
		std::ifstream m_fileStream;

		struct Chunk
		{
			uint32_t chunkSize = 0;
			uint8_t chunkType[4] = {};
			Buffer chunkData;
			uint32_t crc = 0;
		};

		static bool CompBytes(const uint8_t *lhs, const uint8_t *rhs, size_t bytes);
		static void ReverseByteOrder(uint8_t *data, size_t size);

		Chunk readChunk();

		template <typename T>
		static chcl::HuffmanTree<T> UnpackHuffmanTree(T numCodes, chcl::BitStreamView &view, const chcl::HuffmanTree<uint8_t> &codeLengthTree)
		{
			std::vector<T> outCodeLengths;
			outCodeLengths.reserve(numCodes);

			while (outCodeLengths.size() < numCodes)
			{
				chcl::BitStream lengthCode;
				do
				{
					lengthCode.pushBit(view.readBit());
				}
				while (!codeLengthTree.isLeaf(lengthCode));

				uint8_t codeLenHufVal = codeLengthTree.traverse(lengthCode);

				switch (codeLenHufVal)
				{
					case 16:
					{
						uint8_t repeatCount = view.readBits(2) + 3;
						outCodeLengths.insert(outCodeLengths.end(), repeatCount, outCodeLengths.back());
						break;
					}
					case 17:
					{
						uint8_t repeatCount = view.readBits(3) + 3;
						outCodeLengths.insert(outCodeLengths.end(), repeatCount, 0);
						break;
					}
					case 18:
					{
						uint8_t repeatCount = view.readBits(7) + 11;
						outCodeLengths.insert(outCodeLengths.end(), repeatCount, 0);
						break;
					}
					default:
					{
						outCodeLengths.push_back(codeLenHufVal);
						break;
					}
				}
			}

			return chcl::HuffmanTree<T>(outCodeLengths);
		}
	
		static void Decompress(const Buffer &compressedStream, Buffer &filteredData);
		static void Deflate(Buffer &filteredData, chcl::BitStreamView &chunkView, const chcl::HuffmanTree<uint16_t> &litLenTree, const chcl::HuffmanTree<uint8_t> &distTree);

		static void Unfilter(uint8_t *byte, uint8_t filter, size_t pixelX, size_t pixelY, uint8_t bpp, size_t scanlineLength);
	};
}