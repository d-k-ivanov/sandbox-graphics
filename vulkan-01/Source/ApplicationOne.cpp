#include "ApplicationOne.h"

#include <stdexcept>

namespace MyVulkan
{
ApplicationOne::ApplicationOne()
{
    CreatePipelineLayout();
    CreatePipeline();
    CreateCommandBuffers();
}

ApplicationOne::~ApplicationOne()
{
    vkDestroyPipelineLayout(m_Device.GetDevice(), m_PipelineLayout, nullptr);
}

void ApplicationOne::Run() const
{
    while(!m_Window.ShouldClose())
    {
        glfwPollEvents();
    }
}

void ApplicationOne::CreatePipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
    pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount         = 0;
    pipelineLayoutInfo.pSetLayouts            = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges    = nullptr;

    if(vkCreatePipelineLayout(m_Device.GetDevice(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create pipeline layout");
    }
}

void ApplicationOne::CreatePipeline()
{
    auto pipelineConfig           = Pipeline::DefaultPipelineConfigInfo(m_SwapChain.Width(), m_SwapChain.Height());
    pipelineConfig.RenderPass     = m_SwapChain.GetRenderPass();
    pipelineConfig.PipelineLayout = m_PipelineLayout;

    m_Pipeline = std::make_unique<Pipeline>(m_Device,
                                            ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.vert.spv",
                                            ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.frag.spv",
                                            pipelineConfig);
}

void ApplicationOne::CreateCommandBuffers() {}
void ApplicationOne::DrawFrame() const {}
}    // MyVulkan
