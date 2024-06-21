#pragma once

#include <string>

bool validatePath(const std::string &path);
std::string directoryInput(const std::string &prompt, const std::string &invalidPrompt = "Directory invalid or doesn't exist: ");
int integerInput(const std::string &prompt, const std::string &invalidPrompt, int min = INT_MIN, int max = INT_MAX);
std::string codeElementInput(const std::string &prompt, const std::string &invalidPrompt = "Invalid input: ", bool allowFirstNum = false, bool allowBlank = false);