#pragma once

#include "Device.h"
#include "Filesystem.h"
#include "Pipeline.h"
#include "Window.h"

namespace MyVulkan
{

class ApplicationOne
{
public:
    int Width  = 800;
    int Height = 600;

    void Run() const;

private:
    Window   m_Window {Width, Height, "ApplicationOne"};
    Device   m_Device {m_Window};
    Pipeline m_Pipeline {m_Device,
                         ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.vert.spv",
                         ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.frag.spv",
                         Pipeline::DefaultPipelineConfigInfo(Width, Height)};
};

}    // MyVulkan
