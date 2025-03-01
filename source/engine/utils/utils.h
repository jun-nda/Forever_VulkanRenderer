#pragma once

#include "base.h"
#include "log.h"
#include "math.h"

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