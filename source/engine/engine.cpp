#include "engine.h"
#include "utils/utils.h"

#include <iostream>
#include <filesystem>

#define LOG_TRACE(...) { std::cout << (__VA_ARGS__) << std::endl; }

namespace engine{

	inline void tryImportOrCreateIni(const std::string& path)
	{
		if (std::filesystem::exists(path))
		{
			CVarSystem::get()->importConfig(path);
		}
		else
		{
			CVarSystem::get()->exportAllConfig(path);
		}
	};

	void initBasicCVarConfigs()
	{
		std::filesystem::create_directories("config");

		std::filesystem::create_directories(kLogCacheFolder);
		std::filesystem::create_directories(kShaderCacheFolder);
		std::filesystem::create_directories(kConfigCacheFolder);

		// Always override cvar configs.
		CVarSystem::get()->exportAllConfig("config/default.ini");

		// try import or create engine and editor ini.
		tryImportOrCreateIni("config/engine.ini");
		tryImportOrCreateIni("config/editor.ini");
	}

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

		m_windowsInfo.loopHookHandle = in.registerLoopBody([this](const GLFWWindows*, const ApplicationTickData& tickData)
		{ 
			return this->loop(tickData); 
		});
		m_windowsInfo.releaseHookHandle = in.registerReleaseBody([this](const GLFWWindows*) 
		{ 
			return this->release(); 
		});

		CHECK(m_windowsInfo.isCompleted());

		return false;
	}


	bool Engine::init( )
	{
		m_moduleState = EModuleState::Initing;

		bool bResult = true;

		m_timer.init(5.0f, 5.0f);
		m_threadPool = std::make_unique<ThreadPool>();

		// Register basic module of the engine.
		//bResult &= registerRuntimeModule<AssetManager>();
		
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

