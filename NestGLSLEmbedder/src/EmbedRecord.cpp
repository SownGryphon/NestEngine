#include "EmbedRecord.h"

chcl::JSON_Stream& operator<<(chcl::JSON_Stream &stream, const EmbedConfig &record)
{
	chcl::JSON_Object outObj;
	outObj.writeElement("inFile", record.inFilename);
	outObj.writeElement("outFilename", record.outFilename);
	outObj.writeElement("varName", record.varName);
	outObj.writeElement("charsPerLine", record.charsPerLine);
	outObj.writeElement("countSuffix", record.countSuffix);
	outObj.writeElement("dataSuffix", record.dataSuffix);

	stream << outObj;
	return stream;
}

chcl::JSON_Stream& operator>>(chcl::JSON_Stream &stream, EmbedConfig &record)
{
	chcl::JSON_Object inObj;
	stream >> inObj;

	record.inFilename = inObj.readElement<std::string>("inFile");
	record.outFilename = inObj.readElement<std::string>("outFilename");
	record.varName = inObj.readElement<std::string>("varName");
	record.charsPerLine = inObj.readElement<unsigned int>("charsPerLine");
	record.countSuffix = inObj.readElement<std::string>("countSuffix");
	record.dataSuffix = inObj.readElement<std::string>("dataSuffix");

	return stream;
}