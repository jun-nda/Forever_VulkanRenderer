#include "log.h"
#include "glfw.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace engine
{
	// 定义输出字符串格式
	static const std::string kLogPrintFormat   = "%^[%H:%M:%S][%l] %n: %v%$";
	static const std::string kLogFileFormat    =   "[%H:%M:%S][%l] %n: %v";
	static const std::string kCachePrintFormat = "%^[%H:%M:%S][%l] %n: %v%$";
	static const std::string kSaveLogFilePath  = "save/log";

	// Custom log cache sink, use for editor/hub/custom console output. etc.
	template<typename Mutex>
	class LogCacheSink : public spdlog::sinks::base_sink <Mutex>
	{
		friend LoggerSystem;
	private:
		MulticastDelegate<const std::string&, ELogType> m_callbacks;

		// 枚举类型转换
		static ELogType toLogType(spdlog::level::level_enum level)
		{
			switch( level )
			{
				case spdlog::level::trace:
					return ELogType::Trace;
				case spdlog::level::info:
					return ELogType::Info;
				case spdlog::level::warn:
					return ELogType::Warn;
				case spdlog::level::err:
					return ELogType::Error;
				case spdlog::level::critical:
					return ELogType::Fatal;
				default:
					return ELogType::Other;
			}
		}

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

			m_callbacks.broadcast(fmt::to_string(formatted), toLogType(msg.level));
		}

		void flush_() override
		{

		}
	};

	LoggerSystem::InitConfig LoggerSystem::m_initConfigs = {};

	LoggerSystem::LoggerSystem(bool bOutputFile, const std::string& saveFile)
	{
		// the "sink" can be understood as the output target.
		// basic sinks
		{
			auto basicSinkIndex = logSinks.size( );
			logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>( ));

			// set print format.
			logSinks[ basicSinkIndex ]->set_pattern(kLogPrintFormat);
		}

		// cache sinks.
		{
			auto cacheSinkIndex = logSinks.size( );
			m_loggerCache = std::make_shared<LogCacheSink<std::mutex>>();
			logSinks.emplace_back(m_loggerCache);

			logSinks[ cacheSinkIndex ]->set_pattern(kCachePrintFormat);
		}

		if( bOutputFile )
		{
			using TimePoint = std::chrono::system_clock::time_point;
			auto serializeTimePoint = [](const TimePoint& time, const std::string& format)
			{
				std::time_t tt = std::chrono::system_clock::to_time_t(time);
				std::tm tm = *std::localtime(&tt);
				std::stringstream ss;
				ss << std::put_time(&tm, format.c_str( )); // 用于将时间格式化为指定的字符串格式
				return ss.str( );
			};

			TimePoint input = std::chrono::system_clock::now( );
			if( !std::filesystem::exists(kSaveLogFilePath) ){
				std::filesystem::create_directories(kSaveLogFilePath);
			}

			{
				auto saveFolderPath = std::filesystem::path(kSaveLogFilePath);
				auto saveFilePath = saveFile + serializeTimePoint(input, "%Y-%m-%d %H_%M_%S") + ".log";
				auto finalPath = saveFolderPath / saveFilePath; // 应该是重载了 / 运算符

				auto fileSinkIndex = logSinks.size();
				logSinks.emplace_back( std::make_shared<spdlog::sinks::basic_file_sink_mt>(finalPath.string().c_str(), true) );
				logSinks[fileSinkIndex]->set_pattern(kLogFileFormat);
			}
		}


		// sink都弄好了，最后创建logger
		m_defaultLogger = registerLogger("Default");
	}

	std::shared_ptr<spdlog::logger> LoggerSystem::registerLogger(const char* name)
	{
		auto logger = std::make_shared<spdlog::logger>(name, begin(logSinks), end(logSinks));
		spdlog::register_logger(logger);

		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);

		return logger;
	}

	LoggerSystem* LoggerSystem::get( )
	{
		static LoggerSystem defaultLogger(m_initConfigs.bOutputLog, m_initConfigs.outputLogPath + " - ");
		return &defaultLogger;
	}

	void LoggerSystem::initBasicConfig(const InitConfig& in)
	{
		m_initConfigs = in;
	}
}

