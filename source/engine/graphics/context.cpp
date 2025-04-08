#include "context.h"

#include "../engine.h"

namespace engine
{
    static AutoCVarBool cVarRHIDebugMarkerEnable(
        "r.RHI.DebugMarkerEnable",
        "Enable debug marker or not.",
        "RHI",
        true,
        CVarFlags::ReadOnly
    );

    void VulkanContext::registerCheck(Engine* engine)
    {
    }

    bool VulkanContext::init()
    {
        return false;
    }

    bool VulkanContext::tick(const RuntimeModuleTickData& tickData)
    {
        return false;
    }

    bool VulkanContext::beforeRelease()
    {
        return false;
    }

    bool VulkanContext::release()
    {
        return false;
    }
    void VulkanContext::setResourceName(VkObjectType objectType, uint64_t handle, const char* name) const
    {
        if (!cVarRHIDebugMarkerEnable.get())
        {
            return;
        }

        static PFN_vkSetDebugUtilsObjectNameEXT setDebugUtilsObjectName = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(m_device, "vkSetDebugUtilsObjectNameEXT");
        static std::mutex gMutexForSetResource;

        if (setDebugUtilsObjectName && handle && name)
        {
            std::unique_lock<std::mutex> lock(gMutexForSetResource);

            VkDebugUtilsObjectNameInfoEXT nameInfo = {};
            nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            nameInfo.objectType = objectType;
            nameInfo.objectHandle = handle;
            nameInfo.pObjectName = name;

            setDebugUtilsObjectName(m_device, &nameInfo);
        }

    }
    void VulkanContext::setPerfMarkerBegin(VkCommandBuffer cmdBuf, const char* name, const math::vec4& color) const
    {
        if (!cVarRHIDebugMarkerEnable.get())
        {
            return;
        }

        static PFN_vkCmdBeginDebugUtilsLabelEXT cmdBeginDebugUtilsLabel =
            (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetDeviceProcAddr(m_device, "vkCmdBeginDebugUtilsLabelEXT");
        if (cmdBeginDebugUtilsLabel)
        {
            VkDebugUtilsLabelEXT label = {};
            label.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
            label.pLabelName = name;
            label.color[0] = color.r;
            label.color[1] = color.g;
            label.color[2] = color.b;
            label.color[3] = color.a;
            cmdBeginDebugUtilsLabel(cmdBuf, &label);
        }
    
    }
    void VulkanContext::setPerfMarkerEnd(VkCommandBuffer cmdBuf) const
    {
        if (cVarRHIDebugMarkerEnable.get())
        {
            return;
        }

        static PFN_vkCmdEndDebugUtilsLabelEXT cmdEndDebugUtilsLabel =
            (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetDeviceProcAddr(m_device, "vkCmdEndDebugUtilsLabelEXT");

        if (cmdEndDebugUtilsLabel)
        {
            cmdEndDebugUtilsLabel(cmdBuf);
        }
    }
    void VulkanContext::pushDescriptorSet(VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set, uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites)
    {
        // 可以认为是 vkCmdBindDescriptorSets的优化版本？
        static auto pushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)vkGetDeviceProcAddr(m_device, "vkCmdPushDescriptorSetKHR");
        pushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
    }
    VulkanContext* getContext()
    {
        return Engine::get()->getRuntimeModule<VulkanContext>();
    }
    VkDevice getDevice()
    {
        return VkDevice();
    }
}

