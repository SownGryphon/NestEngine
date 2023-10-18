#pragma once

#include <string>

namespace Nest
{
	class Logger
	{
	public:
		enum class LogLevel
		{
			TRACE, INFO, WARNING, ERROR
		};

	private:
		LogLevel m_logLevel;

	public:
		Logger(LogLevel level) : m_logLevel(level) {}

		void setLevel(LogLevel level);

		void log(LogLevel level, const std::string &msg);
	};

	extern Logger logger;
}

#define NE_TRACE(msg) Nest::logger.log(Nest::Logger::LogLevel::TRACE, msg)
#define NE_INFO(msg) Nest::logger.log(Nest::Logger::LogLevel::INFO, msg)
#define NE_WARN(msg) Nest::logger.log(Nest::Logger::LogLevel::WARNING, msg)
#define NE_ERROR(msg) Nest::logger.log(Nest::Logger::LogLevel::ERROR, msg)