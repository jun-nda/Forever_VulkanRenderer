#include "swapchain.h"
#include "context.h"
#include "../engine.h"

namespace engine
{

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes; // mailbox���ǣ�fifo�Ǿ���֡�ʸߣ���ҲҪһ��һ������(��Ϸ������ô�ð�)
	};

	static inline SwapchainSupportDetails querySwapchainSupportDetail()
	{
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(getContext()->getGPU(), getContext()->getSurface(), &details.capabilities);
		
		// Vulkan�����õ�ɶ��Ҫ�Ȼ�ȡ������������ȡ
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(getContext()->getGPU(), getContext()->getSurface(), &formatCount, nullptr);
		if (formatCount != 0)
		{
			// ���������Ҳ�Ǿ��������
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(getContext()->getGPU(), getContext()->getSurface(), &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(getContext()->getGPU(), getContext()->getSurface(), &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(getContext()->getGPU(), getContext()->getSurface(), &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	struct BackBufferFormatState
	{
		VkSurfaceFormatKHR format = {};
		bool bSupport = false;
		bool bSupportAlphaBlend = false;
	};

	inline static const std::vector<BackBufferFormatState>& getBackBufferFormatStateStatic()
	{
		auto getBackBufferFormatState = []()
		{
			size_t maxQueryNum = size_t(EBackBufferFormat::Max);
			std::vector<BackBufferFormatState> result(maxQueryNum);

			auto swapchainSupport = querySwapchainSupportDetail();

			for (const auto& availableFormat : swapchainSupport.formats)
			{
				// SRGB non - linear format check
				if ((availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
					&& (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM))
				{
					result.at(size_t(EBackBufferFormat::SRGB_NonLinear)) =
					{
						.format = availableFormat,
						.bSupport = true,
						.bSupportAlphaBlend = true
					};
				}

				// Hdr10 st2084 format check, alpha 2 bit not support alpha blend.
				// ���⴦�������ʽ��֧��͸�����
				if ((availableFormat.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT)
					&& ((availableFormat.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32) || (availableFormat.format == VK_FORMAT_A2R10G10B10_UNORM_PACK32)))
				{
					result.at(size_t(EBackBufferFormat::HDR10_2084)) =
					{
						.format = availableFormat,
						.bSupport = true,
						.bSupportAlphaBlend = false
					};
				}
			}

			return result;
		};

		// Init backbuffer state.
		static std::vector<BackBufferFormatState> backbufferFormatSate = getBackBufferFormatState();
		return backbufferFormatSate;
	}

	bool Swapchain::isBackBufferFormatSupport(EBackBufferFormat format)
	{
		return getBackBufferFormatStateStatic().at(size_t(format)).bSupport;
	}


	bool Swapchain::isBackBufferSupportAlphaBlend(EBackBufferFormat format)
	{
		return getBackBufferFormatStateStatic().at(size_t(format)).bSupportAlphaBlend;
	}
}

