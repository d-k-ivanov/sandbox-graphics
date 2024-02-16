#pragma once

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
    Pipeline m_Pipeline {ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.vert.spv",
                         ThisExecutableLocation() + "/Resources/Shaders/SimpleShader.frag.spv"};
};

}
