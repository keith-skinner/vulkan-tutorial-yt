#pragma once
#include <glm/glm.hpp>

#include "lve_device.hpp"

namespace lve
{
    class LveModel
    {
    public:
        struct Vertex {
            glm::vec2 postiion;
            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        LveModel(LveDevice& device, const std::vector<Vertex> & vertices);
        ~LveModel();
    
        LveModel(const LveModel&) = delete;
        LveModel& operator=(const LveModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    
    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        LveDevice& lveDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}