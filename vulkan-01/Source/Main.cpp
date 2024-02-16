#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "ApplicationOne.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include <iostream>

int main()
{
    // uint32_t extensionCount = 0;
    // vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    //
    // std::cout << extensionCount << " extensions supported\n";
    //
    // glm::mat4 matrix;
    // glm::vec4 vec;
    // auto      test = matrix * vec;

    MyVulkan::ApplicationOne app {};
    try
    {
        app.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
