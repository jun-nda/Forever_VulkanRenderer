#pragma once

#include "base.h"
#include "log.h"
#include "math.h"
#include "cvars.h"
#include "threadpool.h"
#include "uuid.h"

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