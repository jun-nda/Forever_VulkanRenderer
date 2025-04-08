#include "resource.h"
#include "log.h"
#include "context.h"
#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

namespace engine
{
	static AutoCVarBool cVarRHIDebugMarkerEnable(
		"r.RHI.LogGpuResourceCreateAndDestroyInfo",
		"Enable debug gpu resource create and destroy infos.",
		"RHI",
		false,
		CVarFlags::ReadOnly
	);

	static VkDeviceSize sTotalGpuDeviceSize = 0;
	VkDeviceSize getAllocateGpuResourceSize()
	{
		return sTotalGpuDeviceSize;
	}

	GpuResource::GpuResource(const std::string& name, VkDeviceSize size)
		: m_name(name), m_size(size), m_runtimeUUID(buildRuntimeUUID64u())
	{

	}

	GpuResource::~GpuResource()
	{
	}
	VulkanBuffer::VulkanBuffer(
		VmaAllocator vma,
		const std::string& name,
		VkBufferUsageFlags usageFlags,
		VmaAllocationCreateFlags vmaUsage,
		VkDeviceSize size,
		void* data)
		: GpuResource(name, size), m_vma(vma)

	{

	}
	VulkanBuffer::VulkanBuffer(
		VkBufferUsageFlags usageFlags,
		VkMemoryPropertyFlags memoryPropertyFlags,
		const std::string& name,
		VkDeviceSize size,
		void* data,
		bool bBindAfterCreate)
		: GpuResource(name, size)
	{

	}
	VulkanBuffer::~VulkanBuffer()
	{
	}
	VulkanImage::VulkanImage(VmaAllocator vma, const std::string& name, VkImageCreateInfo createInfo)
		:GpuResource(name, 0), m_createInfo(createInfo), m_vma(vma)
	{
		// Create image to query memory size.
		VkMemoryRequirements memRequirements;
		{
			RHICheck(vkCreateImage(getContext()->getDevice(), &m_createInfo, nullptr, &m_image));
			vkGetImageMemoryRequirements(getContext()->getDevice(), m_image, &memRequirements);
			vkDestroyImage(getContext()->getDevice(), m_image, nullptr);
			m_image = VK_NULL_HANDLE;

			// Get final image memory size.
			this->m_size = memRequirements.size;
		}

		// Init some subresources
		size_t subresourceNum = m_createInfo.arrayLayers * m_createInfo.mipLevels;
		m_subresourceStates.resize(subresourceNum);
		for (size_t i = 0; i < subresourceNum; i++)
		{
			m_subresourceStates[i].imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			m_subresourceStates[i].ownerQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		}


		// Finally we create image.
		{
			VmaAllocationCreateInfo imageAllocCreateInfo = {};
			imageAllocCreateInfo.usage = VMA_MEMORY_USAGE_AUTO;
			imageAllocCreateInfo.flags = VMA_ALLOCATION_CREATE_USER_DATA_COPY_STRING_BIT;
			imageAllocCreateInfo.pUserData = (void*)m_name.c_str();
			VmaAllocationInfo gpuImageAllocInfo = {};

			RHICheck(vmaCreateImage(m_vma, &m_createInfo, &imageAllocCreateInfo, &m_image, &m_allocation, &gpuImageAllocInfo));
		}

		getContext()->setResourceName(VK_OBJECT_TYPE_IMAGE, (uint64_t)m_image, m_name.c_str());

		if (cVarRHIDebugMarkerEnable.get())
		{
			LOG_RHI_TRACE("Create gpu image {0} with size {1} KB.", m_name, float(m_size) / 1024.0f);
		}
		sTotalGpuDeviceSize += m_size;
	}

	VulkanImage::~VulkanImage()
	{
	}
	VkImageLayout VulkanImage::getCurrentLayout(uint32_t layerIndex, uint32_t mipLevel) const
	{
		return VkImageLayout();
	}
	ViewAndBindlessIndex VulkanImage::getOrCreateView(VkImageSubresourceRange range, VkImageViewType viewType)
	{
		return ViewAndBindlessIndex();
	}
}

