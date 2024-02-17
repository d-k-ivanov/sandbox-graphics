#include "Window.h"

#include <stdexcept>

namespace MyVulkan
{

Window::Window(const int w, const int h, std::string name)
    : m_Width(w)
    , m_Height(h)
    , m_WindowName(std::move(name))
{
    InitWindow();
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_Window);
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const
{
    if(glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
    {
               throw std::runtime_error("Failed to create window surface");
    }
}

void Window::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
}

}    // MyVulkan
