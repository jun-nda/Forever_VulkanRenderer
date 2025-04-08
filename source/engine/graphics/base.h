#pragma once

#include "../utils/utils.h"

#include <vulkan/vulkan.h>
#include <cstdint>
#include <algorithm>

namespace engine
{


	// Simple collect of command buffer
	struct RHICommandBufferBase
	{
		VkCommandBuffer cmd;
		VkCommandPool pool;
		uint32_t queueFamily;
	};

	struct GPUCommandPool
	{
		VkQueue queue = VK_NULL_HANDLE;
		VkCommandPool pool = VK_NULL_HANDLE;
	};

	// 格式有很多，常用的就这俩
	// Swapchain backbuffer format select.
	enum class EBackBufferFormat
	{
		SRGB_NonLinear,
		HDR10_2084,
		Max,
	};


	static inline VkImageSubresourceRange buildBasicImageSubresource()
	{
		VkImageSubresourceRange range{};

		range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		range.baseMipLevel = 0;
		range.levelCount = VK_REMAINING_MIP_LEVELS;
		range.baseArrayLayer = 0;
		range.layerCount = VK_REMAINING_ARRAY_LAYERS;

		return range;
	}
}