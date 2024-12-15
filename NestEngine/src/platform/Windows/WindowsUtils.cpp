#include "WindowsUtils.h"

std::wstring Nest::WindowsUtils::toWstring(const std::string &str)
{
	return std::wstring(str.begin(), str.end());
}