#include "context.h"
#include "log.h"
#include "../engine.h"

namespace engine
{
    static AutoCVarBool cVarRHIOpenValidation(
        "r.RHI.OpenValidation",
        "Enable rhi validation info output or not.",
        "RHI",
        true,
        CVarFlags::ReadOnly
    );

    static AutoCVarBool cVarRHIDebugUtilsEnable(
        "r.RHI.DebugUtilsEnable",
        "Debug utils enable or not.",
        "RHI",
        true,
        CVarFlags::ReadOnly
    );

    static AutoCVarInt32 cVarRHIDebugUtilsLevel(
        "r.RHI.DebugUtilsLevel",
        "Debug utils output info levels, 0 is off, 1 is only error, 2 is warning|error, 3 is info|warning|error, 4 is all message capture from validation.",
        "RHI",
        1,
        CVarFlags::ReadAndWrite
    );

    static inline bool enableDebugUtilsCallback()
    {
        return cVarRHIDebugUtilsEnable.get();
    }

    static inline bool openValidation()
    {
        return cVarRHIOpenValidation.get() != 0;
    }

    void VulkanContext::initInstance()
    {
        // 实例扩展是“系统功能”， 设备扩展是”显卡功能“
        // Query all useful instance extensions.
        uint32_t instanceExtensionCount;
        RHICheck(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr));
        std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
        RHICheck(vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableInstanceExtensions.data()));

        // Fill instance extensions.
        std::vector<const char*> enableExtensions{};
        {
            if (enableDebugUtilsCallback())
                enableExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

    }
}