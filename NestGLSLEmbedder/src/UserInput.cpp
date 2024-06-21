#include "UserInput.h"

#include <filesystem>
#include <iostream>

static const std::string badCodeChars = " \t\n!@#$%^&*()-=+[]{}<>\\|:;\'\",./?`~";

bool validatePath(const std::string &path)
{
	std::filesystem::path filePath{ path };
	return std::filesystem::exists(filePath);
}

std::string directoryInput(const std::string &prompt, const std::string &invalidPrompt)
{
	std::cout << prompt;
	std::string path = "";
	do
	{
		if (path != "")
			std::cout << invalidPrompt;
		std::getline(std::cin, path);
	}
	while (!validatePath(path));
	return path;
}

int integerInput(const std::string &prompt, const std::string &invalidPrompt, int min, int max)
{
	std::string userInput = "";
	int numInput;
	std::cout << prompt;
	do
	{
		if (userInput != "")
			std::cout << invalidPrompt;
		std::getline(std::cin, userInput);
		try
		{
			numInput = std::stoi(userInput);
			userInput = "";
		}
		catch (std::invalid_argument)
		{
		}
	}
	while (numInput < min || numInput > max || userInput != "");
	return numInput;
}

std::string codeElementInput(const std::string &prompt, const std::string &invalidPrompt, bool allowFirstNum, bool allowBlank)
{
	std::cout << prompt;
	std::string input = "";
	bool first = true;
	do
	{
		if (!first)
			std::cout << invalidPrompt;
		std::getline(std::cin, input);
		first = false;
	}
	while ((input == "" && !allowBlank) || (!allowFirstNum && std::isdigit(input[0])) || input.find_first_of(badCodeChars) != std::string::npos);
	return input;
}