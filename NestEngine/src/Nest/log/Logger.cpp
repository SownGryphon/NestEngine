#include "Logger.h"

#include <iostream>

namespace Nest
{
	void Logger::setLevel(LogLevel level)
	{
		m_logLevel = level;
	}

	void Logger::log(LogLevel level, const std::string &msg)
	{
		if (level >= m_logLevel)
			std::cout << msg;
	}
}

Nest::Logger Nest::logger(Nest::Logger::LogLevel::TRACE);