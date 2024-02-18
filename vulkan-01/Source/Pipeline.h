#pragma once

#include "Device.h"

#include <string>
#include <vector>

namespace MyVulkan
{
struct PipelineConfigInfo
{
    VkViewport                             Viewport;
    VkRect2D                               Scissor;
    VkPipelineInputAssemblyStateCreateInfo InputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo RasterizationInfo;
    VkPipelineMultisampleStateCreateInfo   MultisampleInfo;
    VkPipelineColorBlendAttachmentState    ColorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo    ColorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo  DepthStencilInfo;
    VkPipelineLayout                       PipelineLayout = nullptr;
    VkRenderPass                           RenderPass     = nullptr;
    uint32_t                               Subpass        = 0;
};

class Pipeline
{
public:
    Pipeline(Device& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
    ~Pipeline();

    Pipeline(const Pipeline&)            = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void Bind(VkCommandBuffer commandBuffer) const;
    static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);

private:
    static std::vector<char> ReadFile(const std::string& filepath);

    void CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

    void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const;

    Device&        m_Device;
    VkPipeline     m_GraphicsPipeline;
    VkShaderModule m_VertexShaderModule;
    VkShaderModule m_FragmentShaderModule;
};
}    // MyVulkan
