
#include "log.h"

#include <vma/vk_mem_alloc.h>
#include <cstdint>

namespace engine
{
	class GpuResource : NonCopyable
	{
	protected:
		std::string m_name;
		VkDeviceSize m_size;
		UUID64u m_runtimeUUID;

	public:
		GpuResource(const std::string& name, VkDeviceSize size);
		virtual ~GpuResource();

	public:
		VkDeviceSize getSize() const { return m_size; }

		const std::string& getName() const { return m_name; }

		const UUID64u& getRuntimeUUID() const { return m_runtimeUUID; }
	};

	class VulkanBuffer : public GpuResource
	{
	protected:
		VkBuffer m_buffer = VK_NULL_HANDLE;

		VmaAllocator m_vma;
		VmaAllocation m_allocation = nullptr;
		VkDeviceMemory m_memory = VK_NULL_HANDLE;

		// Mapped pointer for buffer.
		void* m_mapped = nullptr;

		VkDescriptorBufferInfo m_defaultInfo;

		bool m_bSupportDeviceAddress = false;
		uint64_t m_deviceAddress = 0;
	public:
		explicit VulkanBuffer(
			VmaAllocator vma,
			const std::string& name,
			VkBufferUsageFlags usageFlags,
			VmaAllocationCreateFlags vmaUsage,
			VkDeviceSize size,
			void* data = nullptr);

		explicit VulkanBuffer(
			VkBufferUsageFlags usageFlags,
			VkMemoryPropertyFlags memoryPropertyFlags,
			const std::string& name,
			VkDeviceSize size,
			void* data = nullptr,
			bool bBindAfterCreate = true);

		virtual ~VulkanBuffer();

	public:

	};

	struct ViewAndBindlessIndex
	{
		VkImageView view = VK_NULL_HANDLE;
		uint32_t srvBindless = ~0U;
	};

	class VulkanImage : public GpuResource
	{
	protected:
		// Image handle.
		VkImage m_image = VK_NULL_HANDLE; \

		VmaAllocator m_vma;
		VmaAllocation m_allocation = nullptr;

		// Cache image create info.
		VkImageCreateInfo m_createInfo = {};

		// Image resource queue family owner.
		struct SubResourceState
		{
			uint32_t ownerQueueFamilyIndex;
			VkImageLayout imageLayout;
		};  

		std::vector<SubResourceState> m_subresourceStates;

		// Cache create image views.
		std::unordered_map<uint64_t, ViewAndBindlessIndex> m_cacheImageViews{};

	public:
		explicit VulkanImage(VmaAllocator vma, const std::string& name, VkImageCreateInfo createInfo);
		virtual ~VulkanImage();

	// Get method
	public:
		VkImage getImage() const { return m_image; }
		VkFormat getFormat() const { return m_createInfo.format; }
		VkExtent3D getExtent() const { return m_createInfo.extent; }
		const VkImageCreateInfo& getInfo() const { return m_createInfo; }

		VkImageLayout getCurrentLayout(uint32_t layerIndex, uint32_t mipLevel) const;
		// Try get view and create if no exist.
		ViewAndBindlessIndex getOrCreateView(
			VkImageSubresourceRange range = buildBasicImageSubresource(), VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D);
	
	public:

	protected:
		uint32_t getSubresourceIndex(uint32_t layerIndex, uint32_t mipLevel) const;


	};
}