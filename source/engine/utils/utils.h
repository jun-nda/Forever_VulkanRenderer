#pragma once

#include <unordered_map>
#include <unordered_set>

#include "glfw.h"
#include "base.h"
#include "log.h"
#include "math.h"
#include "cvars.h"
#include "threadpool.h"
#include "uuid.h"
#include "noncopyable.h"


#include <filesystem>
#include <utfcpp/utf8.h>
#include <utfcpp/utf8/cpp17.h>

#define ENABLE_LOG

#ifdef ENABLE_LOG
#define LOG_TRACE(...) { ::engine::LoggerSystem::get()->getDefaultLogger()->trace   (__VA_ARGS__); }
#define LOG_INFO(...)  { ::engine::LoggerSystem::get()->getDefaultLogger()->info    (__VA_ARGS__); }
#define LOG_WARN(...)  { ::engine::LoggerSystem::get()->getDefaultLogger()->warn    (__VA_ARGS__); }
#define LOG_ERROR(...) { ::engine::LoggerSystem::get()->getDefaultLogger()->error   (__VA_ARGS__); }
#define LOG_FATAL(...) { ::engine::LoggerSystem::get()->getDefaultLogger()->critical(__VA_ARGS__); throw std::runtime_error("Utils fatal!"); }
#else
#define LOG_TRACE(...)   
#define LOG_INFO (...)    
#define LOG_WARN(...)   
#define LOG_ERROR(...)    
#define LOG_FATAL(...) { throw std::runtime_error("Utils fatal!"); }
#endif

#ifdef APP_DEBUG
#define CHECK(x) { if(!(x)) { LOG_FATAL("Check error, at line {0} on file {1}.", __LINE__, __FILE__); __debugbreak(); } }
#define ASSERT(x, ...) { if(!(x)) { LOG_FATAL("Assert failed: {2}, at line {0} on file {1}.", __LINE__, __FILE__, __VA_ARGS__); __debugbreak(); } }
#else
#define CHECK(x) { if(!(x)) { LOG_FATAL("Check error."); } }
#define ASSERT(x, ...) { if(!(x)) { LOG_FATAL("Assert failed: {0}.", __VA_ARGS__); } }
#endif

#define  CHECK_ENTRY() ASSERT(false, "Should not entry here, fix me!")
#define UN_IMPLEMENT() ASSERT(false, "Un-implement yet, fix me!")

struct __engine_ConstructOnceObject_Log
{
	enum class ELogType
	{
		Trace,
		Info,
		Warn,
		Error,
	};

	explicit __engine_ConstructOnceObject_Log(const std::string& in, ELogType type)
	{
		switch (type)
		{
		case ELogType::Trace:
		{
			LOG_TRACE(in);
			return;
		}
		case ELogType::Info:
		{
			LOG_INFO(in);
			return;
		}
		case ELogType::Warn:
		{
			LOG_WARN(in);
		}
		case ELogType::Error:
		{
			LOG_ERROR(in);
#ifdef APP_DEBUG
			__debugbreak();
#endif
			return;
		}
		}

		CHECK_ENTRY();
	}
};

#define LOG_TRACE_ONCE(str) { static __engine_ConstructOnceObject_Log __local_trace(str, __engine_ConstructOnceObject_Log::ELogType::Trace); }
#define LOG_INFO_ONCE(str)  { static __engine_ConstructOnceObject_Log __local_info(str,  __engine_ConstructOnceObject_Log::ELogType::Info);  }
#define LOG_WARN_ONCE(str)  { static __engine_ConstructOnceObject_Log __local_warn(str,  __engine_ConstructOnceObject_Log::ELogType::Warn);  }
#define LOG_ERROR_ONCE(str) { static __engine_ConstructOnceObject_Log __local_error(str, __engine_ConstructOnceObject_Log::ELogType::Error); }

namespace
{
	using u8str = std::string;



	inline static std::string buildRelativePathUtf8(
		const std::filesystem::path& shortPath,
		const std::filesystem::path& longPath)
	{
		const std::u16string shortPath16 = std::filesystem::absolute(shortPath).u16string();
		const std::u16string longPath16 = std::filesystem::absolute(longPath).u16string();

		auto result = utf8::utf16to8(longPath16.substr(shortPath16.size()));

		if (result.starts_with("\\") || result.starts_with("/"))
		{
			result = result.erase(0, 1);
		}

		return result;
	}

	// Boost hash combine.
	static inline size_t hashCombine(size_t lhs, size_t rhs)
	{
		lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
		return lhs;
	}

	// Boost hash combine.
	template <class T>
	inline void hashCombine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

}
