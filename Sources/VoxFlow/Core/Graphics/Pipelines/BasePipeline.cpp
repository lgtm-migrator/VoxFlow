// Author : snowapril

#include <spdlog/spdlog.h>

#include <VoxFlow/Core/Devices/LogicalDevice.hpp>
#include <VoxFlow/Core/Graphics/Pipelines/BasePipeline.hpp>
#include <VoxFlow/Core/Graphics/Pipelines/GlslangUtil.hpp>
#include <VoxFlow/Core/Graphics/Pipelines/PipelineLayout.hpp>
#include <VoxFlow/Core/Utils/Initializer.hpp>
#include <VoxFlow/Core/Utils/Logger.hpp>

namespace VoxFlow
{
PipelineCreateInfo PipelineCreateInfo::CreateDefault() noexcept
{
    return {
        .vertexInputState =
            Initializer::MakeInfo<VkPipelineVertexInputStateCreateInfo>(),
        .inputAssemblyState =
            Initializer::MakeInfo<VkPipelineInputAssemblyStateCreateInfo>(),
        .tessellationState =
            Initializer::MakeInfo<VkPipelineTessellationStateCreateInfo>(),
        .viewportState =
            Initializer::MakeInfo<VkPipelineViewportStateCreateInfo>(),
        .rasterizationState =
            Initializer::MakeInfo<VkPipelineRasterizationStateCreateInfo>(),
        .multisampleState =
            Initializer::MakeInfo<VkPipelineMultisampleStateCreateInfo>(),
        .depthStencilState =
            Initializer::MakeInfo<VkPipelineDepthStencilStateCreateInfo>(),
        .colorBlendState =
            Initializer::MakeInfo<VkPipelineColorBlendStateCreateInfo>(),
        .dynamicState =
            Initializer::MakeInfo<VkPipelineDynamicStateCreateInfo>(),
        .renderPass = VK_NULL_HANDLE,
        .subpass = 0
    };
}

BasePipeline::BasePipeline(const std::shared_ptr<LogicalDevice>& device,
                           const std::shared_ptr<PipelineLayout>& layout)
    : _device(device), _layout(layout)
{
    // Do nothing
}

BasePipeline::~BasePipeline()
{
    release();
}

BasePipeline::BasePipeline(BasePipeline&& other) noexcept
    : _device(std::move(other._device)), _pipeline(other._pipeline)
{
    // Do nothing
}

BasePipeline& BasePipeline::operator=(BasePipeline&& other) noexcept
{
    if (&other != this)
    {
        _device = std::move(other._device);
        _pipeline = other._pipeline;
    }
    return *this;
}

void BasePipeline::bindPipeline(const CommandBuffer& cmdBuffer) const noexcept
{
    vkCmdBindPipeline(cmdBuffer.get(), getBindPoint(), _pipeline);
}

void BasePipeline::release()
{
    std::for_each(_shaderStageInfos.begin(), _shaderStageInfos.end(),
                  [this](const auto& stageInfo) {
                      vkDestroyShaderModule(_device->get(), stageInfo.module,
                                            nullptr);
                  });
    _shaderStageInfos.clear();
    _layout.reset();
    if (_pipeline != VK_NULL_HANDLE)
    {
        vkDestroyPipeline(_device->get(), _pipeline, nullptr);
        _pipeline = VK_NULL_HANDLE;
    }
    _device.reset();
}

VkPipelineShaderStageCreateInfo BasePipeline::compileToShaderStage(
    const char* filename) const
{
    std::vector<char> shaderSource;
    assert(GlslangUtil::ReadShaderFile(filename, &shaderSource));

    const glslang_stage_t glslangStage =
        GlslangUtil::GlslangStageFromFilename(filename);
    std::vector<unsigned int> spirvBinary;
    assert(GlslangUtil::CompileShader(glslangStage, shaderSource.data(),
                                      &spirvBinary));

    [[maybe_unused]] auto moduleInfo =
        Initializer::MakeInfo<VkShaderModuleCreateInfo>();
    moduleInfo.codeSize = spirvBinary.size() * sizeof(unsigned int);
    moduleInfo.pCode = spirvBinary.data();

    VkShaderModule module = VK_NULL_HANDLE;
    VK_ASSERT(
        vkCreateShaderModule(_device->get(), &moduleInfo, nullptr, &module));

    auto stageCreateInfo =
        Initializer::MakeInfo<VkPipelineShaderStageCreateInfo>();
    stageCreateInfo.stage =
        GlslangUtil::GlslangStageToVulkanStage(glslangStage);
    stageCreateInfo.module = module;
    return stageCreateInfo;
}
}  // namespace VoxFlow