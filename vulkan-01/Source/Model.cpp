#include "Model.h"

#include <cassert>
#include <cstring>

namespace MyVulkan
{
Model::Model(Device& device, const std::vector<Vertex>& vertices)
    : m_Device(device)
{
    CreateVertexBuffers(vertices);
}

Model::~Model()
{
    vkDestroyBuffer(m_Device.GetDevice(), m_VertexBuffer, nullptr);
    vkFreeMemory(m_Device.GetDevice(), m_VertexBufferMemory, nullptr);
}

void Model::CreateVertexBuffers(const std::vector<Vertex>& vertices)
{
    m_VertexCount = static_cast<uint32_t>(vertices.size());
    assert(m_VertexCount >= 3 && "Vertex count must be at least 3.");

    const VkDeviceSize bufferSize = sizeof(vertices[0]) * m_VertexCount;
    m_Device.CreateBuffer(bufferSize,
                          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                          m_VertexBuffer,
                          m_VertexBufferMemory);

    void* data;
    vkMapMemory(m_Device.GetDevice(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), bufferSize);
    vkUnmapMemory(m_Device.GetDevice(), m_VertexBufferMemory);
}

void Model::Bind(const VkCommandBuffer commandBuffer) const
{
    const VkBuffer         vertexBuffers[] = {m_VertexBuffer};
    constexpr VkDeviceSize offsets[]       = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

void Model::Draw(const VkCommandBuffer commandBuffer) const
{
    vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::GetBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescription(1);
    bindingDescription[0].binding   = 0;
    bindingDescription[0].stride    = sizeof(Vertex);
    bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::GetAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
    attributeDescriptions[0].binding  = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format   = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset   = 0;

    return attributeDescriptions;
}

}    // namespace MyVulkan
