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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(m_Window, this);
    glfwSetFramebufferSizeCallback(m_Window, FramebufferResizeCallback);
}

void Window::FramebufferResizeCallback(GLFWwindow* window, const int width, const int height)
{
    const auto appWindow            = static_cast<Window*>(glfwGetWindowUserPointer(window));
    appWindow->m_FrameBufferResized = true;
    appWindow->m_Width              = width;
    appWindow->m_Height             = height;
}

}    // MyVulkan
