#include <string>
#include <functional>
#include <GLFW/glfw3.h>

#include "delegate.h"
#include "base.h"
#include "math.h"

namespace engine{



	struct GLFWWindowData
	{
		// Cache windows pointer
		GLFWwindow* window = nullptr; // 注意GLFW库的window是小写的~ 

		GLFWimage icon;

		bool bFocus = true; // Current windows under focus state;
		bool bShouldRun = true; // Windows app should run or not?

		// Windows state cache which will update by callback.
		uint32_t width = 64U;
		uint32_t height = 64U;

		// Scroll offset, no delta!!!
		math::vec2 scrollOffset = { 0.0f, 0.0f };
		math::vec2 mousePosition = { 0.0f, 0.0f };

		// Get key state.
	};






	class GLFWWindows{
	public:
		// default 表示还想保留默认构造
		GLFWWindows( ) = default;

		struct InitConfig{
			std::string appName;
			std::string appIcon;

			enum class EWindowMode{
				FullScreenWithoutTile = 0,
				FullScreenWithTile,
				Free,
			} windowShowMode = EWindowMode::FullScreenWithTile;

			bool bResizable = true;
			uint32_t initWidth = 800U;
			uint32_t initHeight = 480U;
		};


		[[nodiscard]] bool init(InitConfig initConfig);
		void loop( );
		[[nodiscard]] bool release( );


		// Hooks for init functions.
		[[nodiscard]] DelegateHandle registerInitBody(std::function<bool(const GLFWWindows*)> function);


	private:
		std::string m_name;
		GLFWWindowData m_data;

		MulticastDelegate<const GLFWWindows*, bool&> m_initBodies;
		MulticastDelegate<const GLFWWindows*, const ApplicationTickData&, bool&> m_loopBodies;
		MulticastDelegate<const GLFWWindows*, bool&> m_releaseBodies;

		MulticastDelegate<const GLFWWindows*, bool&> m_closedEventBodies;

	};
}