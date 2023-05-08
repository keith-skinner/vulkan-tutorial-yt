#include "lve_pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>

#include "lve_model.hpp"

lve::LvePipeline::LvePipeline(LveDevice &device, const std::string &vertFilepath, const std::string &fragFilePath, const PipelineConfigInfo configInfo)
:   lveDevice{device}
{
    createGraphicsPipeline(vertFilepath, fragFilePath, configInfo);
}

lve::LvePipeline::~LvePipeline()
{
    vkDestroyShaderModule(lveDevice.device(), vertShaderModule, nullptr);
    vkDestroyShaderModule(lveDevice.device(), fragShaderModule, nullptr);
    vkDestroyPipeline(lveDevice.device(), graphicsPipeline, nullptr);
}

void lve::LvePipeline::bind(VkCommandBuffer commandBuffer)
{
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}

lve::PipelineConfigInfo lve::LvePipeline::defaultPipelineConfigInfo([[maybe_unused]] uint32_t width, [[maybe_unused]] uint32_t height)
{
    // super long value didn't feel like formatting it.
    constexpr VkColorComponentFlags colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    
    lve::PipelineConfigInfo configInfo{
        .viewport{
            .x = 0.0f
            ,.y = 0.0f
            ,.width = static_cast<float>(width)
            ,.height = static_cast<float>(height)
            ,.minDepth = 0.0f
            ,.maxDepth = 1.0f
        },
        .scissor{
            .offset = {0, 0}
            ,.extent = {width, height}
        },
        .inputAssemblyInfo {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO
            ,.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
            ,.primitiveRestartEnable = VK_FALSE
        },
        .rasterizationInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
            ,.depthClampEnable = VK_FALSE
            ,.rasterizerDiscardEnable = VK_FALSE
            ,.polygonMode = VK_POLYGON_MODE_FILL
            ,.cullMode = VK_CULL_MODE_NONE
            ,.frontFace = VK_FRONT_FACE_CLOCKWISE
            ,.depthBiasEnable = VK_FALSE
            ,.depthBiasConstantFactor = 0.0f
            ,.depthBiasClamp = 0.0f
            ,.depthBiasSlopeFactor = 0.0f
            ,.lineWidth = 1.0f
        },
        .multisampleInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO
            ,.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT
            ,.sampleShadingEnable = VK_FALSE
            ,.minSampleShading = 1.0f
            ,.pSampleMask = nullptr
            ,.alphaToCoverageEnable = VK_FALSE
            ,.alphaToOneEnable = VK_FALSE
        },
        .colorBlendAttachment{
            .blendEnable = VK_FALSE
            ,.srcColorBlendFactor = VK_BLEND_FACTOR_ONE
            ,.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO
            ,.colorBlendOp = VK_BLEND_OP_ADD
            ,.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE
            ,.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO
            ,.alphaBlendOp = VK_BLEND_OP_ADD
            ,.colorWriteMask = colorWriteMask
        },
        .colorBlendInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
            ,.logicOpEnable = VK_FALSE
            ,.logicOp = VK_LOGIC_OP_COPY
            ,.attachmentCount = 1
            ,.pAttachments = &configInfo.colorBlendAttachment
            ,.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
        },
        .depthStencilInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
            ,.depthTestEnable = VK_TRUE
            ,.depthWriteEnable = VK_TRUE
            ,.depthCompareOp = VK_COMPARE_OP_LESS
            ,.depthBoundsTestEnable = VK_FALSE
            ,.stencilTestEnable = VK_FALSE
            ,.front = {}
            ,.back = {}
            ,.minDepthBounds = 0.0f
            ,.maxDepthBounds = 1.0f
        },
    };
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

void lve::LvePipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo configInfo)
{
    assert(configInfo.pipelineLayout != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no pipelineLayout provided in configInfo");
    assert(configInfo.renderPass != VK_NULL_HANDLE && "Cannot create graphics pipeline:: no renderPass provided in configInfo");
    
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    createShaderModule(vertCode, &vertShaderModule);
    createShaderModule(fragCode, &fragShaderModule);
    
    VkPipelineShaderStageCreateInfo shaderStages[2];
    // Vertex
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;
    // Fragment
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto bindingDescriptions = LveModel::Vertex::getBindingDescriptions();
    auto attributeDescriptions = LveModel::Vertex::getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO
        ,.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size())
        ,.pVertexBindingDescriptions = bindingDescriptions.data()
        ,.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size())
        ,.pVertexAttributeDescriptions = attributeDescriptions.data()
    };

    VkPipelineViewportStateCreateInfo viewportInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO
        ,.viewportCount = 1
        ,.pViewports = &configInfo.viewport
        ,.scissorCount = 1
        ,.pScissors = &configInfo.scissor
    };

    VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType                 = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO
        ,.stageCount           = 2
        ,.pStages              = shaderStages
        ,.pVertexInputState    = &vertexInputInfo
        ,.pInputAssemblyState  = &configInfo.inputAssemblyInfo
        ,.pViewportState       = &viewportInfo
        ,.pRasterizationState  = &configInfo.rasterizationInfo
        ,.pMultisampleState    = &configInfo.multisampleInfo
        ,.pDepthStencilState   = &configInfo.depthStencilInfo
        ,.pColorBlendState     = &configInfo.colorBlendInfo
        ,.pDynamicState        = nullptr

        ,.layout               = configInfo.pipelineLayout
        ,.renderPass           = configInfo.renderPass
        ,.subpass              = configInfo.subpass

        ,.basePipelineHandle   = VK_NULL_HANDLE
        ,.basePipelineIndex    = -1
    };

    if (vkCreateGraphicsPipelines(lveDevice.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        throw std::runtime_error("Failed to create graphics pipeline");
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
