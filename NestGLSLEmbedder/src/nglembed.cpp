#include <fstream>
#include <string>

#include <iostream>
#include <unordered_map>

#include "EmbedRecord.h"
#include "UserInput.h"

std::unordered_map<char, char> specialChars = {
	{ '\\', '\\' },
	{ '\'', '\'' },
	{ '\"', '\"' },
	{ '\r', 'r' },
	{ '\n', 'n' },
	{ '\t', 't' },
	{ '\v', 'v' }
};

std::vector<EmbedConfig> g_previousEmbeds;

struct {
	std::string inputDir, outputDir;
	unsigned int charsPerLine = 0;
	std::string countSuffix = "_count",
		dataSuffix = "_data";
} configData;

static void embedFile(const EmbedConfig &config)
{

	std::ifstream sourceFile;
	sourceFile.open(configData.inputDir + '\\' + config.inFilename, std::ios::in | std::ios_base::ate);
	size_t fileSize = sourceFile.tellg();
	++fileSize;	// To accout for null terminator
	sourceFile.seekg(0);

	std::ofstream outputFile;
	outputFile.open(configData.outputDir + '\\' + config.outFilename + ".h");

	outputFile << "inline constexpr size_t " << config.varName << config.countSuffix << " = " << fileSize << ";\n";
	outputFile << "inline constexpr char " << config.varName << config.dataSuffix << "[" << fileSize << "] = {\n\t";

	unsigned int charsWritten = 0;
	for (char c = sourceFile.get(); !sourceFile.eof(); c = sourceFile.get())
	{
		outputFile << '\'';
		if (specialChars.contains(c))
			outputFile << '\\' << specialChars[c];
		else
			outputFile << c;
		outputFile << "\', ";

		++charsWritten;
		if (charsWritten == config.charsPerLine)
		{
			outputFile << "\n\t";
			charsWritten = 0;
		}
	}
	outputFile << "\'\\0\'\n};";

	sourceFile.close();
	outputFile.close();
}

static void embedFileMenu()
{
	std::cout << "Enter input file name (including extension): ";
	EmbedConfig fileEmbedCfg;
	while (fileEmbedCfg.inFilename == "")
	{
		std::getline(std::cin, fileEmbedCfg.inFilename);

		if (fileEmbedCfg.inFilename == "")
			return;

		if (!validatePath(configData.inputDir + '\\' + fileEmbedCfg.inFilename))
		{
			std::cout << "File does not exist. Please enter valid file: ";
			fileEmbedCfg.inFilename = "";
		}
	}

	std::vector<EmbedConfig> prevFileEmbeds;
	for (const EmbedConfig &record : g_previousEmbeds)
	{
		if (record.inFilename == fileEmbedCfg.inFilename)
			prevFileEmbeds.push_back(record);
	}

	if (!prevFileEmbeds.empty())
	{
		std::cout << "File was previously embedded. Embed again with saved settings?\n";
		for (size_t i = 0; i < prevFileEmbeds.size(); ++i)
		{
			std::cout << (i + 1) << ". File name: " << prevFileEmbeds[i].outFilename << '\n';
			std::cout << "\tVariable base name: " << prevFileEmbeds[i].varName << '\n';
		}
		std::cout << (prevFileEmbeds.size() + 1) << ". Embed with new settings.\n";

		int action = integerInput("Select embed settings: ", "", 1, prevFileEmbeds.size() + 1);

		if (action - 1 < prevFileEmbeds.size())
		{
			embedFile(prevFileEmbeds[action - 1]);
			return;
		}
	}

	std::cout << "Enter output file name: ";
	std::getline(std::cin, fileEmbedCfg.outFilename);
	if (fileEmbedCfg.outFilename == "")
		return;

	fileEmbedCfg.varName = codeElementInput("Enter variable base name: ", "Invalid base name: ", false, true);
	if (fileEmbedCfg.varName == "")
		return;

	fileEmbedCfg.charsPerLine = configData.charsPerLine;
	fileEmbedCfg.countSuffix = configData.countSuffix;
	fileEmbedCfg.dataSuffix = configData.dataSuffix;

	embedFile(fileEmbedCfg);

	g_previousEmbeds.push_back(fileEmbedCfg);
}

static void configMenu()
{
	bool exitMenu = false;
	while (!exitMenu)
	{
		std::cout << "Current config items:\n";
		std::cout << "1. Input directory: " << configData.inputDir << '\n';
		std::cout << "2. Output directory: " << configData.outputDir << '\n';
		std::cout << "3. Maximum characters per line: ";
		if (configData.charsPerLine == 0)
			std::cout << "unlimited";
		else
			std::cout << configData.charsPerLine;
		std::cout << '\n';
		std::cout << "4. File size suffix: " << configData.countSuffix << '\n';
		std::cout << "5. File data suffix: " << configData.dataSuffix << '\n';

		int action = integerInput("Select config variable to edit (6 to exit): ", "", 1, 6);
		std::cout << '\n';

		switch (action)
		{
			case 1:
				configData.inputDir = directoryInput("Enter input directory: ");
				break;
			case 2:
				configData.outputDir = directoryInput("Enter output directory: ");
				break;
			case 3:
				configData.charsPerLine = integerInput("Set new maximum (0 to disable limit): ", "", 0);
				break;
			case 4:
				configData.countSuffix = "";
				while (configData.countSuffix == "")
				{
					configData.countSuffix = codeElementInput("Enter new suffix (must not be blank): ", "Invalid input: ", true);
				}
				break;
			case 5:
				configData.dataSuffix = "";
				while (configData.dataSuffix == "")
				{
					configData.dataSuffix = codeElementInput("Enter new suffix (must not be blank): ", "Invalid input: ", true);
				}
				break;
			case 6:
				exitMenu = true;
				break;
		}
	}
}

static void prevEmbedMenu()
{
	bool exitMenu = false;
	while (!exitMenu)
	{
		std::cout << "Previous file embeds:\n";

		for (size_t i = 0; i < g_previousEmbeds.size(); ++i)
		{
			std::cout << (i + 1) << ". File: " << g_previousEmbeds[i].inFilename << '\n';
			std::cout << "\tOutput file: " << g_previousEmbeds[i].outFilename << '\n';
		}
		std::cout << (g_previousEmbeds.size() + 1) << ". Back.\n";

		int choice = integerInput("Select previous embed: ", "", 1, g_previousEmbeds.size() + 1) - 1;
		std::cout << '\n';

		if (choice == g_previousEmbeds.size())
		{
			exitMenu = true;
			break;
		}

		std::cout << "What would you like to do?\n";
		std::cout << "1. Re-embed file\n";
		std::cout << "2. Delete embed record\n";
		std::cout << "3. Cancel\n";

		int action = integerInput("Select action: ", "", 1, 3);

		switch (action)
		{
			case 1:
				embedFile(g_previousEmbeds[choice]);
				break;
			case 2:
				g_previousEmbeds.erase(g_previousEmbeds.begin() + choice);
				break;
		}
		std::cout << '\n';
	}
}

static void mainMenu()
{
	bool exitMenu = false;
	while (!exitMenu)
	{
		std::cout << "Available actions:\n";
		std::cout << "1. Convert shader to embedded file\n";
		std::cout << "2. Change config\n";
		std::cout << "3. View previous file embeds (" << g_previousEmbeds.size() << ")\n";
		std::cout << "4. Exit application\n";
		int action = integerInput("Enter next action: ", "", 1, 4);
		std::cout << '\n';

		switch (action)
		{
			case 1:
				embedFileMenu();
				break;
			case 2:
				configMenu();
				break;
			case 3:
				prevEmbedMenu();
				break;
			case 4:
				exitMenu = true;
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	configData.inputDir = directoryInput("Enter input directory: ");

	std::string configFilePath = configData.inputDir + '\\' + "embedRecords.json";
	if (validatePath(configFilePath))
		g_previousEmbeds = chcl::JSON_Parser::ReadFile<std::vector<EmbedConfig>>(configFilePath);

	configData.outputDir = directoryInput("Enter output directory: ");

	mainMenu();

	chcl::JSON_Parser::SaveToFile(configFilePath, g_previousEmbeds);
}