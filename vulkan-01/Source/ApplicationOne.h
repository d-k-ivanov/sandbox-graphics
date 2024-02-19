#pragma once

#include "Device.h"
#include "Filesystem.h"
#include "Model.h"
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
    // int Width  = 1600;
    // int Height = 1200;
    int Width  = 2560;
    int Height = 1920;

    ApplicationOne();
    ~ApplicationOne();

    ApplicationOne(const ApplicationOne&)            = delete;
    ApplicationOne& operator=(const ApplicationOne&) = delete;

    void Run();

private:
    void LoadModels();
    void LoadSerpinskiTriangle();
    void CreatePipelineLayout();
    void CreatePipeline();
    void RecreateSwapChain();
    void CreateCommandBuffers();
    void FreeCommandBuffers();
    void RecordCommandBuffer(uint32_t imageIndex) const;
    void DrawFrame();

    Window                     m_Window {Width, Height, "ApplicationOne"};
    Device                     m_Device {m_Window};
    std::unique_ptr<SwapChain> m_SwapChain;
    std::unique_ptr<Pipeline>  m_Pipeline;
    // Pipeline  m_Pipeline {m_Device,
    //                      ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.vert.spv",
    //                      ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.frag.spv",
    //                      Pipeline::DefaultPipelineConfigInfo(Width, Height)};
    VkPipelineLayout             m_PipelineLayout;
    std::vector<VkCommandBuffer> m_CommandBuffers;
    std::unique_ptr<Model>       m_Model;
};

}    // MyVulkan
