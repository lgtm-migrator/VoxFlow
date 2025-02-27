// Author : snowapril

#ifndef VOXEL_FLOW_COMPUTE_PIPELINE_HPP
#define VOXEL_FLOW_COMPUTE_PIPELINE_HPP

#include <VoxFlow/Core/Graphics/Pipelines/BasePipeline.hpp>

namespace VoxFlow
{
class LogicalDevice;

class ComputePipeline : public BasePipeline
{
 public:
    explicit ComputePipeline(const std::shared_ptr<LogicalDevice>& device,
                             const char* shaderFilename,
                             const std::shared_ptr<PipelineLayout>& layout);
    ~ComputePipeline() override;
    ComputePipeline(ComputePipeline&& other) noexcept;
    ComputePipeline& operator=(ComputePipeline&& other) noexcept;

    [[nodiscard]] VkPipelineBindPoint getBindPoint() const noexcept override
    {
        return VK_PIPELINE_BIND_POINT_COMPUTE;
    }
};
}  // namespace VoxFlow

#endif