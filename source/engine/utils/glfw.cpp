#include "glfw.h"
#include "utils/utils.h"

namespace engine{

	bool GLFWWindows::init(InitConfig info)
	{
		m_name = info.appName;

		if( glfwInit( ) != GLFW_TRUE )
		{
			LOG_ERROR("Fail to init glfw, application will pre-exit!");
			return false;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, info.bResizable ? GL_TRUE : GL_FALSE);

		// Get monitor information.
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		//m_data.monitorMaxSize.x = mode->width;
		//m_data.monitorMaxSize.y = mode->height;
		

		return false;
	}

	void GLFWWindows::loop( )
	{
	}

	bool GLFWWindows::release( )
	{
		return false;
	}

	DelegateHandle engine::GLFWWindows::registerInitBody(std::function<bool(const GLFWWindows*)> function)
	{
		return m_initBodies.addLambda([function](const GLFWWindows* windows, bool& bResult)
		{
			bResult &= function(windows);
		});
	}
}

