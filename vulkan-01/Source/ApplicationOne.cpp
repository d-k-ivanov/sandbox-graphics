#include "ApplicationOne.h"

namespace MyVulkan
{
void ApplicationOne::Run() const
{
    while(!m_Window.ShouldClose())
    {
        glfwPollEvents();

    }
}
}    // MyVulkan
