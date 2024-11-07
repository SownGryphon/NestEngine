#include "EmbedderSettings.h"

chcl::JSON_Stream& operator<<(chcl::JSON_Stream &stream, const EmbedderSettings &settings)
{
	chcl::JSON_Object settingsObj;
	settingsObj.writeElement("inputDir", settings.inputDir);
	settingsObj.writeElement("outputDir", settings.outputDir);
	settingsObj.writeElement("charsPerLine", settings.charsPerLine);
	settingsObj.writeElement("countSuffix", settings.countSuffix);
	settingsObj.writeElement("dataSuffix", settings.dataSuffix);

	stream << settingsObj;
	return stream;
}

chcl::JSON_Stream& operator>>(chcl::JSON_Stream &stream, EmbedderSettings &settings)
{
	chcl::JSON_Object settingsObj;
	stream >> settingsObj;
	settings.inputDir = settingsObj.readElement<std::string>("inputDir");
	settings.outputDir = settingsObj.readElement<std::string>("outputDir");
	settings.charsPerLine = settingsObj.readElement<unsigned int>("charsPerLine");
	settings.countSuffix = settingsObj.readElement<std::string>("countSuffix");
	settings.dataSuffix = settingsObj.readElement<std::string>("dataSuffix");

	return stream;
}