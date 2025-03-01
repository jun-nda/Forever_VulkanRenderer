#include "engine.h"

#include <iostream>

#define LOG_TRACE(...) { std::cout << (__VA_ARGS__) << std::endl; }

namespace engine{
	Engine* engine::Engine::get( )
	{
		static Engine engine;
		return &engine;
	}
	bool Engine::initGLFWWindowsHook(GLFWWindows& in)
	{
		if( m_windowsInfo.isCompleted( ) ){

			LOG_TRACE("Can't init GLFW windows hook twice!");
			//std::cout <<  << endl;
		}
		LOG_TRACE("OK!");

		m_windowsInfo.windows = &in;

		m_windowsInfo.initHookHandle = in.registerInitBody([this](const GLFWWindows*)
		{
			return this->init( );
		});

		//m_windowsInfo.loopHookHandle = in.registerLoopBody([this](const GLFWWindows*, const ApplicationTickData& tickData)
		//{ 
		//	return this->loop(tickData); 
		//});
		//m_windowsInfo.releaseHookHandle = in.registerReleaseBody([this](const GLFWWindows*) 
		//{ 
		//	return this->release(); 
		//});

		return false;
	}


	bool Engine::init( )
	{
		return true;
	}

	bool Engine::loop(const ApplicationTickData& tickData)
	{
		return false;
	}

	bool Engine::release( )
	{
		return false;
	}


	bool Engine::GLFWWindowsInfo::isCompleted( ) const
	{
		return windows != nullptr
			&& initHookHandle.isValid()
			&& loopHookHandle.isValid()
			&& releaseHookHandle.isValid();
	}
}

