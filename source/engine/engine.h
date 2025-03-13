#pragma once

#include "utils/utils.h"
#include "utils/glfw.h"
#include "utils/timer.h"

namespace engine{
	static const std::string kShaderCacheFolder = "save/shader/";
	static const std::string kLogCacheFolder = "save/log/";
	static const std::string kConfigCacheFolder = "save/config/";


	void initBasicCVarConfigs();

	class Engine{
	protected:

	public:
		enum class EModuleState
		{
			None = 0,

			Initing,
			Ticking,
			Releasing,
		};

		static Engine* get( );

		bool init( );
		bool loop(const ApplicationTickData& tickData);
		bool release( );

		bool initGLFWWindowsHook(GLFWWindows& in);


		// Runtime module will tick sequence by register time, all run within main thread.
		template<typename T>
		[[nodiscard]] bool registerRuntimeModule()
		{
			checkRuntimeModule<T>();

			const std::string className = typeid(T).name();
			if (!m_registeredModulesIndexMap.contains(className))
			{
				auto newModule = std::make_unique<T>( this );
				newModule->registerCheck(this);

				m_registeredModulesIndexMap[className] = m_runtimeModules.size();

				LOG_TRACE("Register module: [{0}] at position [{1}].", className, m_runtimeModules.size());

				// Push back to module vector.
				m_runtimeModules.push_back( std::move(newModule) );
				if (!m_runtimeModules.back()->init( ))
				{
					LOG_ERROR("Fail to init module: {}.", className);
					return false;
				}
				return true;
			}

			return false;
		}
		
	private:
		struct GLFWWindowsInfo{
			bool isCompleted( ) const; 

			// Windows info.
			GLFWWindows* windows = nullptr;

			// Handles of hook.
			DelegateHandle initHookHandle;
			DelegateHandle loopHookHandle;
			DelegateHandle releaseHookHandle;
		} m_windowsInfo; 

		// Engine Timer
		Timer m_timer;

		std::unique_ptr<ThreadPool> m_threadPool;
		
		std::vector<std::unique_ptr<IRuntimeModule>> m_runtimeModules;
		std::map<std::string, size_t> m_registeredModulesIndexMap;
		
		
		//std::unique_ptr<ThreadPool>

		EModuleState m_moduleState = EModuleState::None;
	};
}
