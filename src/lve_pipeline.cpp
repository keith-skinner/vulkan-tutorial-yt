#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

lve::LvePipeline::LvePipeline(LveDevice &device, const std::string &vertFilepath, const std::string &fragFilePath, const PipelineConfigInfo configInfo)
:   lveDevice{device}
{
    createGraphicsPipeline(vertFilepath, fragFilePath, configInfo);
}

lve::LvePipeline::~LvePipeline()
{
}

lve::PipelineConfigInfo lve::LvePipeline::defaultPipelineConfigInfo([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height)
{
    lve::PipelineConfigInfo configInfo{};
    return configInfo;
}

std::vector<char> lve::LvePipeline::readFile(const std::string &filepath)
{
    std::ifstream file{filepath, std::ios::ate | std::ios::binary};

    if (!file) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    auto fileSize = file.tellg();
    std::vector<char> buffer(static_cast<size_t>(fileSize));
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

void lve::LvePipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, [[maybe_unused]] const PipelineConfigInfo configInfo)
{
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    using std::cout, std::endl;
    cout << "Vertex Shader Code Size: " << vertCode.size() << endl;
    cout << "Fragment Shader Code Size: " << fragCode.size() << endl;
}

void lve::LvePipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
{
    VkShaderModuleCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data()),
    };

    if (vkCreateShaderModule(lveDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        throw std::runtime_error("Failed to create shader module");
    
}
