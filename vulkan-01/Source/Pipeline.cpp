#include "Pipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace MyVulkan
{
Pipeline::Pipeline(Device& device, const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo)
    : m_Device {device}
{
    CreateGraphicsPipeline(vertFilepath, fragFilepath, configInfo);
}

std::vector<char> Pipeline::ReadFile(const std::string& filepath)
{
    std::ifstream file {filepath, std::ios::ate | std::ios::binary};

    if(!file.is_open())
    {
        throw std::runtime_error("failed to open file: " + filepath);
    }

    const size_t      fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

    file.close();
    return buffer;
}

void Pipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo) const
{
    const auto vertCode = ReadFile(vertFilepath);
    const auto fragCode = ReadFile(fragFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}

void Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) const
{
    VkShaderModuleCreateInfo createInfo {};
    createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode    = reinterpret_cast<const uint32_t*>(code.data());

    if(vkCreateShaderModule(m_Device.GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module");
    }
}

PipelineConfigInfo Pipeline::DefaultPipelineConfigInfo(uint32_t width, uint32_t height)
{
    PipelineConfigInfo configInfo {};
    return configInfo;
}

Pipeline::~Pipeline()
{
    vkDestroyShaderModule(m_Device.GetDevice(), m_VertexShaderModule, nullptr);
    vkDestroyShaderModule(m_Device.GetDevice(), m_FragmentShaderModule, nullptr);
    vkDestroyPipeline(m_Device.GetDevice(), m_GraphicsPipeline, nullptr);
}

}    // MyVulkan
