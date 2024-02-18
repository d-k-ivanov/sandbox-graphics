#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace MyVulkan
{
class Window
{
public:
    Window(int w, int h, std::string name);
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    bool       ShouldClose() const;
    VkExtent2D GetExtent() const { return {static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height)}; }

    void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;

private:
    void InitWindow();

    int m_Width;
    int m_Height;

    std::string m_WindowName;
    GLFWwindow* m_Window;
};

}
