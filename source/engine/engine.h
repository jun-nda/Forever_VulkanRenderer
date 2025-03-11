#pragma once

#include "utils/utils.h"
#include "utils/glfw.h"

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


	private:
		struct GLFWWindowsInfo{
			bool isCompleted( ) const; // const 在后面表示函数内不会修改对象状态，放在返回类型前面表示返回一个常量

			// Windows info.
			GLFWWindows* windows = nullptr;

			// Handles of hook.
			DelegateHandle initHookHandle;
			DelegateHandle loopHookHandle;
			DelegateHandle releaseHookHandle;
		} m_windowsInfo; 

		//std::unique_ptr<ThreadPool>

		EModuleState m_moduleState = EModuleState::None;
	};
}