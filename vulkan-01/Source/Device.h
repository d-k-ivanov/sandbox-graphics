#pragma once

#include "Window.h"

#include <vector>

namespace MyVulkan
{

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR        Capabilities;
    std::vector<VkSurfaceFormatKHR> Formats;
    std::vector<VkPresentModeKHR>   PresentModes;
};

struct QueueFamilyIndices
{
    uint32_t GraphicsFamily;
    uint32_t PresentFamily;
    bool     GraphicsFamilyHasValue = false;
    bool     PresentFamilyHasValue  = false;
    bool     isComplete() const { return GraphicsFamilyHasValue && PresentFamilyHasValue; }
};

class Device
{
public:
#ifdef NDEBUG
    const bool EnableValidationLayers = false;
#else
    const bool EnableValidationLayers = true;
#endif

    explicit Device(Window& window);
    ~Device();

    // Not copyable or movable
    Device(const Device&)         = delete;
    void operator=(const Device&) = delete;
    Device(Device&&)              = delete;
    Device& operator=(Device&&)   = delete;

    VkCommandPool GetCommandPool() const { return m_CommandPool; }
    VkDevice      GetDevice() const { return m_Device; }
    VkSurfaceKHR  Surface() const { return m_Surface; }
    VkQueue       GraphicsQueue() const { return m_GraphicsQueue; }
    VkQueue       PresentQueue() const { return m_PresentQueue; }

    SwapChainSupportDetails GetSwapChainSupport() const { return QuerySwapChainSupport(m_PhysicalDevice); }

    uint32_t           FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    QueueFamilyIndices FindPhysicalQueueFamilies() const { return FindQueueFamilies(m_PhysicalDevice); }
    VkFormat           FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

    // Buffer Helper Functions
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) const;

    VkCommandBuffer BeginSingleTimeCommands() const;

    void EndSingleTimeCommands(VkCommandBuffer commandBuffer) const;
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount) const;

    void CreateImageWithInfo(const VkImageCreateInfo& imageInfo, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) const;

    VkPhysicalDeviceProperties Properties;

private:
    void CreateInstance();
    void SetupDebugMessenger();
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateCommandPool();

    // helper functions
    bool                     isDeviceSuitable(VkPhysicalDevice device) const;
    std::vector<const char*> GetRequiredExtensions() const;
    bool                     CheckValidationLayerSupport() const;
    QueueFamilyIndices       FindQueueFamilies(VkPhysicalDevice device) const;
    void                     PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) const;
    void                     HasGflwRequiredInstanceExtensions() const;
    bool                     CheckDeviceExtensionSupport(VkPhysicalDevice device) const;
    SwapChainSupportDetails  QuerySwapChainSupport(VkPhysicalDevice device) const;

    VkInstance               m_Instance;
    VkDebugUtilsMessengerEXT m_DebugMessenger;
    VkPhysicalDevice         m_PhysicalDevice = VK_NULL_HANDLE;
    Window&                  m_Window;
    VkCommandPool            m_CommandPool;

    VkDevice     m_Device;
    VkSurfaceKHR m_Surface;
    VkQueue      m_GraphicsQueue;
    VkQueue      m_PresentQueue;

    const std::vector<const char*> m_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
    const std::vector<const char*> m_DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

}    // namespace MyVulkan