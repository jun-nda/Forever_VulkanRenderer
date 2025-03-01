#pragma once

#include "utils/utils.h"
#include "utils/glfw.h"

namespace engine{

	class Engine{
	protected:

	public:
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
	};
}