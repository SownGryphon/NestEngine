#pragma once

#include <string>

#include <dataStorage/JSON_Integration.h>

struct EmbedderSettings {
	std::string inputDir, outputDir;
	unsigned int charsPerLine = 0;
	std::string countSuffix = "_count",
		dataSuffix = "_data";

	friend chcl::JSON_Stream& operator<<(chcl::JSON_Stream &stream, const EmbedderSettings &settings);
	friend chcl::JSON_Stream& operator>>(chcl::JSON_Stream &stream, EmbedderSettings &settings);
};