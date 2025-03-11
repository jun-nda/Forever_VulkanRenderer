#include "editor.h"
#include "engine.h"

using namespace engine;
using EWindowModeType = GLFWWindows::InitConfig::EWindowMode;

bool Editor::init()
{
    return true;
}

bool Editor::release()
{
    return false;
}

/// <summary>
/// 只创建一次
/// </summary>
/// <returns></returns>
Editor* Editor::get( ){
	static Editor editor;
	return &editor;
}

int Editor::run(int argc, char** argv)
{
    LoggerSystem::initBasicConfig(
        {
            .bOutputLog = true,
            .outputLogPath = "editor",
        }); // c++ 20 aggregate initialization

    LOG_TRACE("Init reflection compile trace uuid {}.", 2);

    // Init cvar configs.
    initBasicCVarConfigs();

	Engine::get( )->initGLFWWindowsHook(m_windows);

    GLFWWindows::InitConfig windowInitConfig =
    {
        .appName = "dark editor",
        .appIcon = "image/editorIcon.png",
        .windowShowMode = EWindowModeType::Free,
        .bResizable = false,
        .initWidth = 1600U,
        .initHeight = 900U,
    };

    // Windows init.
    if (m_windows.init(windowInitConfig))
    {
        // Editor init after windows init.
        ASSERT(this->init(), "Fail to init editor!");

        m_windows.loop();
    }

	return 0;
}
