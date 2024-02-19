#include "ApplicationOne.h"

#include <array>
#include <stdexcept>

namespace MyVulkan
{

namespace
{
    void GenerateSierpinskiTriangle(std::vector<Model::Vertex>& vertices, const Model::Vertex& v1, const Model::Vertex& v2, const Model::Vertex& v3, const int depth)
    {
        if(depth == 0)
        {
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);
        }
        else
        {
            const Model::Vertex mid1 = {{(v1.Position.x + v2.Position.x) / 2, (v1.Position.y + v2.Position.y) / 2}};
            const Model::Vertex mid2 = {{(v2.Position.x + v3.Position.x) / 2, (v2.Position.y + v3.Position.y) / 2}};
            const Model::Vertex mid3 = {{(v3.Position.x + v1.Position.x) / 2, (v3.Position.y + v1.Position.y) / 2}};

            GenerateSierpinskiTriangle(vertices, v1, mid1, mid3, depth - 1);
            GenerateSierpinskiTriangle(vertices, mid1, v2, mid2, depth - 1);
            GenerateSierpinskiTriangle(vertices, mid3, mid2, v3, depth - 1);
        }
    }

}

ApplicationOne::ApplicationOne()
{
    LoadModels();
    CreatePipelineLayout();
    CreatePipeline();
    CreateCommandBuffers();
}

ApplicationOne::~ApplicationOne()
{
    vkDestroyPipelineLayout(m_Device.GetDevice(), m_PipelineLayout, nullptr);
}

void ApplicationOne::Run()
{
    while(!m_Window.ShouldClose())
    {
        glfwPollEvents();
        DrawFrame();
    }

    vkDeviceWaitIdle(m_Device.GetDevice());
}

void ApplicationOne::LoadModels()
{
    // std::vector<Model::Vertex> vertices = {{{0.0f, -0.9f}}, {{0.9f, 0.9f}}, {{-0.9f, 0.9f}}};
    // m_Model                             = std::make_unique<Model>(m_Device, vertices);

    std::vector<Model::Vertex> vertices;

    constexpr Model::Vertex v1 = {{0.0f, -0.9f}};
    constexpr Model::Vertex v2 = {{0.9f, 0.9f}};
    constexpr Model::Vertex v3 = {{-0.9f, 0.9f}};

    constexpr int depth = 8;    // Set the desired depth of the Sierpinski triangle

    GenerateSierpinskiTriangle(vertices, v1, v2, v3, depth);

    m_Model = std::make_unique<Model>(m_Device, vertices);
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
                                            ThisExecutableLocation() + "/Resources/Shaders/SimpleShader2.vert.spv",
                                            ThisExecutableLocation() + "/Resources/Shaders/SimpleShader2.frag.spv",
                                            pipelineConfig);
}

void ApplicationOne::CreateCommandBuffers()
{
    m_CommandBuffers.resize(m_SwapChain.ImageCount());

    VkCommandBufferAllocateInfo allocInfo {};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool        = m_Device.GetCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(m_CommandBuffers.size());

    if(vkAllocateCommandBuffers(m_Device.GetDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffers");
    }

    for(size_t i = 0; i < m_CommandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = m_SwapChain.GetRenderPass();
        renderPassInfo.framebuffer       = m_SwapChain.GetFrameBuffer(static_cast<int>(i));
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = m_SwapChain.GetSwapChainExtent();

        std::array<VkClearValue, 2> clearValues {};
        clearValues[0].color           = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil    = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues    = clearValues.data();

        vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        m_Pipeline->Bind(m_CommandBuffers[i]);
        m_Model->Bind(m_CommandBuffers[i]);
        m_Model->Draw(m_CommandBuffers[i]);

        vkCmdEndRenderPass(m_CommandBuffers[i]);
        if(vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to record command buffer");
        }
    }
}

void ApplicationOne::DrawFrame()
{
    uint32_t imageIndex;
    auto     result = m_SwapChain.AcquireNextImage(&imageIndex);
    if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    result = m_SwapChain.SubmitCommandBuffers(&m_CommandBuffers[imageIndex], &imageIndex);
    if(result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swap chain image");
    }
}
}    // MyVulkan
