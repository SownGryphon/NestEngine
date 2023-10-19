#include "Logger.h"

#include <iostream>

namespace Nest
{
	void Logger::setLevel(LogLevel level)
	{
		m_logLevel = level;
	}

	Formatter Logger::s_formatter;
}

Nest::Logger Nest::logger(Nest::Logger::LogLevel::TRACE);