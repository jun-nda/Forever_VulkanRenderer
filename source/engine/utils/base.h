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

}