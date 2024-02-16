#include "Pipeline.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace MyVulkan
{
Pipeline::Pipeline(const std::string& vertFilepath, const std::string& fragFilepath)
{
    CreateGraphicsPipeline(vertFilepath, fragFilepath);
}

std::vector<char> Pipeline::ReadFile(const std::string& filepath)
{
    std::ifstream file {filepath, std::ios::ate | std::ios::binary};

    if(!file.is_open())
    {
        throw std::runtime_error("failed to open file: " + filepath);
    }

    const size_t      fileSize = file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(fileSize));

    file.close();
    return buffer;
}

void Pipeline::CreateGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath) const
{
    const auto vertCode = ReadFile(vertFilepath);
    const auto fragCode = ReadFile(fragFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}
}    // MyVulkan
