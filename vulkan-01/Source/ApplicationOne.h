#pragma once

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
    Window m_Window {Width, Height, "ApplicationOne"};
};

}
