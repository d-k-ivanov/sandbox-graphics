#pragma once

#include "Device.h"
#include "Filesystem.h"
#include "Pipeline.h"
#include "SwapChain.h"
#include "Window.h"

#include <memory>
#include <vector>

namespace MyVulkan
{

class ApplicationOne
{
public:
    int Width  = 800;
    int Height = 600;

    ApplicationOne();
    ~ApplicationOne();

    ApplicationOne(const ApplicationOne&)            = delete;
    ApplicationOne& operator=(const ApplicationOne&) = delete;

    void Run();

private:
    void CreatePipelineLayout();
    void CreatePipeline();
    void CreateCommandBuffers();
    void DrawFrame();

    Window                    m_Window {Width, Height, "ApplicationOne"};
    Device                    m_Device {m_Window};
    SwapChain                 m_SwapChain {m_Device, m_Window.GetExtent()};
    std::unique_ptr<Pipeline> m_Pipeline;
    // Pipeline  m_Pipeline {m_Device,
    //                      ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.vert.spv",
    //                      ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.frag.spv",
    //                      Pipeline::DefaultPipelineConfigInfo(Width, Height)};
    VkPipelineLayout             m_PipelineLayout;
    std::vector<VkCommandBuffer> m_CommandBuffers;
};

}    // MyVulkan
