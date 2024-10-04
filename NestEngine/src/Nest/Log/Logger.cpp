#include "Logger.h"

#include <iostream>

#include "Nest/Core/Core.h"

namespace Nest
{
	void Logger::setLevel(LogLevel level)
	{
		m_logLevel = level;
	}

	std::string Logger::GetLevelString(LogLevel level)
	{
		switch (level)
		{
			case LogLevel::TRACE: return "[TRACE] ";
			case LogLevel::INFO: return "[INFO] ";
			case LogLevel::WARNING: return "[WARNING] ";
			case LogLevel::ERROR: return "[ERROR] ";
		}

		NE_ASSERT(0, "Invalid log level {}.", int(level));
		return "";
	}

	Formatter Logger::s_formatter;
}

Nest::Logger Nest::logger(Nest::Logger::LogLevel::TRACE);