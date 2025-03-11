#pragma once
#include "noncopyable.h"
#include <map>

namespace engine
{
	struct ApplicationTickData
	{
		// Application type.
		enum class ApplicationType
		{
			Windows,
			Console,
		} applicationType;

		// Useful when application is windows.
		struct WindowInfo
		{
			uint32_t wdith;
			uint32_t height;
			bool bIsMinimized;
			bool bLoseFocus;
		} windowInfo;
	};


	// Per module tick data, time in seconds unit, used in engine module.
	struct RuntimeModuleTickData
	{
		// Application tick data.
		ApplicationTickData applicationInfo;

		// Engine realtime delta time.
		float deltaTime = 0.0f;

		// Engine delta time with smooth process. It diff from smoothFps, it update every frame with lerp smooth.
		float smoothDeltaTime = 0.0f;

		// Realtime fps.
		float fps = 0.0f;

		// Smooth fps which update per seconds.
		float smoothFps = 0.0f;

		// Engine run time.
		float runTime = 0.0f;

		// Total tick frame count of engine.
		uint64_t tickCount = 0;

		// Is smooth fps update this frame?
		bool bSmoothFpsUpdate = false;

		// Is game runing or not?
		bool bGameRuning = false;

		// Is game in pause state?
		bool bGamePause = false;

		// Game run time.
		float gameTime = 0.0f;

		// Total tick frame count of game.
		uint64_t gameTickCount = 0;
	};

}