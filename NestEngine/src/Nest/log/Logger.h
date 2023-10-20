#pragma once

#include <iostream>
#include <string>

#include "Formatter.h"

namespace Nest
{
	class Logger
	{
	public:
		enum class LogLevel
		{
			TRACE, INFO, WARNING, ERROR
		};

		Logger(LogLevel level) : m_logLevel(level) {}

		void setLevel(LogLevel level);

		template <typename ...Args>
		void log(LogLevel level, const std::string &msg, Args... args)
		{
			if (level >= m_logLevel)
				std::cout << GetLevelString(level) << s_formatter.format(msg, args...) << std::endl;
		}

	private:
		LogLevel m_logLevel;

		static Formatter s_formatter;

		static std::string GetLevelString(LogLevel level);
	};

	extern Logger logger;
}

#define NE_TRACE(msg, ...) Nest::logger.log(Nest::Logger::LogLevel::TRACE, msg, __VA_ARGS__)
#define NE_INFO(msg, ...) Nest::logger.log(Nest::Logger::LogLevel::INFO, msg, __VA_ARGS__)
#define NE_WARN(msg, ...) Nest::logger.log(Nest::Logger::LogLevel::WARNING, msg, __VA_ARGS__)
#define NE_ERROR(msg, ...) Nest::logger.log(Nest::Logger::LogLevel::ERROR, msg, __VA_ARGS__)