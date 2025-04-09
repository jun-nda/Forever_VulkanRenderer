#pragma once

#include "base.h"
#include "swapchain.h"
#include "shader.h"
#include <vma/vk_mem_alloc.h>

namespace engine
{

	class VulkanContext : public IRuntimeModule
	{
	public:
		// some struct
		struct GPUQueuesInfo
		{
			uint32_t graphicsFamily = ~0;
			uint32_t copyFamily = ~0;
			uint32_t computeFamily = ~0;

			std::vector<VkQueue> graphcisQueues;
		};

		struct GPUCommandPools
		{
			GPUCommandPool majorGraphics;

			std::vector<GPUCommandPool> graphics;
		};

	public:
		explicit VulkanContext(Engine* engine) : IRuntimeModule(engine) { }
		virtual ~VulkanContext() = default;

		virtual void registerCheck(Engine* engine) override;
		virtual bool init() override;
		virtual bool tick(const RuntimeModuleTickData& tickData) override;
		virtual bool beforeRelease() override;
		virtual bool release() override;

	// Get method
	public:
		// window相关
		VkSurfaceKHR getSurface() const { return m_surface; }
		GLFWwindow* getWindow() const { return m_window; }
		const auto& getSwapchain() const { return m_swapchain; }
		uint32_t getBackBufferCount() const { return m_swapchain.getBackbufferCount(); }
		const EBackBufferFormat& getBackbufferFormatType() const { return m_backbufferFormat; }

		VkDevice         getDevice()   const { return m_device; }
		VkPhysicalDevice getGPU()      const { return m_gpu; }

		// Global VMA, common use for no temporal(frame frequency destroy) asset.
		VmaAllocator     getVMABuffer()      const { return m_vmaBuffer; }
		VmaAllocator     getVMAImage()      const { return m_vmaImage; }
		VmaAllocator     getVMAFrequencyBuffer()      const { return m_vmaFrequencyDestroyBuffer; }
		VmaAllocator     getVMAFrequencyImage() const { return m_vmaFrequencyDestroyImage; }
		VkInstance       getInstance() const { return m_instance; }

		// Get queue familys.
		const GPUQueuesInfo& getGPUQueuesInfo() const { return m_queues; }

		uint32_t getGraphiscFamily() const { return m_queues.graphicsFamily; }
		uint32_t getComputeFamily()  const { return m_queues.computeFamily; }
		uint32_t getCopyFamily()     const { return m_queues.copyFamily; }

		// Major graphics queue or command pool with 1.0f priority.
		VkQueue getMajorGraphicsQueue() const { return m_commandPools.majorGraphics.queue; }
		VkCommandPool getMajorGraphicsCommandPool() const { return m_commandPools.majorGraphics.pool; }

		// shader 
		const auto& getShaderCache() const { return *m_shaderCache; }
		auto& getShaderCache() { return *m_shaderCache; }
	public:
		// Perf maker
		void setResourceName(VkObjectType objectType, uint64_t handle, const char* name) const;
		void setPerfMarkerBegin(VkCommandBuffer cmdBuf, const char* name, const math::vec4& color) const;
		void setPerfMarkerEnd(VkCommandBuffer cmdBuf) const;
	public:
		// pipeline
		void pushDescriptorSet(
			VkCommandBuffer commandBuffer,
			VkPipelineBindPoint pipelineBindPoint,
			VkPipelineLayout layout,
			uint32_t set,
			uint32_t descriptorWriteCount,
			const VkWriteDescriptorSet* pDescriptorWrites
		);
	// Delegate, no args
	public:
		MulticastDelegate<> onBeforeSwapchainRecreate;
		MulticastDelegate<> onAfterSwapchainRecreate;

	protected:
		// Vulkan instance
		VkInstance m_instance = VK_NULL_HANDLE;

		// Vulkan debug utils handle.
		VkDebugUtilsMessengerEXT m_debugUtilsHandle = VK_NULL_HANDLE;

		// Vulkan device
		VkDevice m_device = VK_NULL_HANDLE;

		// Using gpu.
		VkPhysicalDevice m_gpu = VK_NULL_HANDLE;

		// AMD open-source memory allocator lib.
		VmaAllocator m_vmaBuffer = VK_NULL_HANDLE;
		VmaAllocator m_vmaImage = VK_NULL_HANDLE;
		VmaAllocator m_vmaFrequencyDestroyBuffer = VK_NULL_HANDLE;
		VmaAllocator m_vmaFrequencyDestroyImage = VK_NULL_HANDLE;

		// Cache device infos.
		VkPhysicalDeviceProperties m_deviceProperties;
		VkPhysicalDeviceMemoryProperties m_memoryProperties;
		VkPhysicalDeviceMemoryProperties2 m_deviceProperties2;
		VkPhysicalDeviceSubgroupProperties m_subgroupProperties;
		//VkPhysicalDeviceDescriptorIndexingPropertiesEXT


		GPUQueuesInfo m_queues;
		GPUCommandPools m_commandPools;

		// shader
		std::unique_ptr<ShaderCache>          m_shaderCache;

	protected:
		// Windows handle and surface handle, it can be nullptr when application run with console.
		GLFWwindow* m_window = nullptr;
		VkSurfaceKHR m_surface = VK_NULL_HANDLE;
		Swapchain m_swapchain;
		EBackBufferFormat m_backbufferFormat = EBackBufferFormat::SRGB_NonLinear;


	private:
		// vulkan繁杂的初始化
		void initInstance();
		
	};

	extern VulkanContext* getContext();
	extern VkDevice getDevice();

}

