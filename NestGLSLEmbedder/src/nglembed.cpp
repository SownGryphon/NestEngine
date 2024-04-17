#include <fstream>
#include <string>

#include <iostream>

/*
arg 1 - file path
arg 2 - output path
*/
int main(int argc, char *argv[])
{
	if (argc != 3)
		return 1;

	std::string sourcePath{ argv[1] };
	std::string outputPath{ argv[2] };

	size_t fileNameBegin = sourcePath.rfind('\\');
	if (fileNameBegin == std::string::npos)
		fileNameBegin = 0;
	else
		++fileNameBegin;

	std::string filename = sourcePath.substr(fileNameBegin, sourcePath.size() - fileNameBegin);

	std::cout << "File path: " << sourcePath << '\n';
	std::cout << "File name: " << filename << '\n';

	std::ifstream sourceFile;
	sourceFile.open(sourcePath);

	std::ofstream outputFile;
	outputFile.open(outputPath + filename + ".h");

	outputFile << "R\"(\n";

	std::string line;
	while (std::getline(sourceFile, line))
	{
		outputFile << line << '\n';
	}
	outputFile << ")\"";

	sourceFile.close();
	outputFile.close();
}