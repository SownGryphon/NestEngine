#pragma once

#include <dataStorage/JSON_Parser.h>

struct EmbedConfig
{
	std::string inFilename, outFilename, varName;
	unsigned int charsPerLine;
	std::string countSuffix, dataSuffix;

	friend chcl::JSON_Stream& operator<<(chcl::JSON_Stream &stream, const EmbedConfig &record);
	friend chcl::JSON_Stream& operator>>(chcl::JSON_Stream &stream, EmbedConfig &record);
};