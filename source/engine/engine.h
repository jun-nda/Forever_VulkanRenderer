#pragma once

#include "utils/utils.h"
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

		template <typename T>
		T* getRuntimeModule() const
		{
			checkRuntimeModule<T>();

			auto name = typeid(T).name();
			assert(m_registeredModulesIndexMap.contains(name) && "Forget registerRuntimeModule?");
			size_t moduleIndex = m_registeredModulesIndexMap.at(name);
			return static_cast<T*>(m_runtimeModules.at(moduleIndex).get());
		}
	// Get method
	public:
		const GLFWWindows* getGLFWWindows() const { return m_windowsInfo.windows; }
		GLFWWindows* getGLFWWindows() { return m_windowsInfo.windows; }

		// You should call this function after initGLFWWindowsHook called.
		bool isWindowApplication() const { return m_windowsInfo.isCompleted(); }

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
