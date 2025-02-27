// Author : snowapril

#ifndef VOXEL_FLOW_GLSLANG_UTIL_HPP
#define VOXEL_FLOW_GLSLANG_UTIL_HPP

#include <glslang_c_shader_types.h>
#include <volk/volk.h>
#include <string_view>
#include <vector>

namespace VoxFlow
{
// TODO(snowapril): `CompileShader` needs to be moved into Instance or
// LogicalDevice as it only success when they are already constructed
class GlslangUtil
{
 public:
    GlslangUtil() = delete;

    [[nodiscard]] static glslang_stage_t GlslangStageFromFilename(
        const std::string_view filename);
    [[nodiscard]] static VkShaderStageFlagBits GlslangStageToVulkanStage(
        glslang_stage_t glslangStage);
    [[nodiscard]] static bool ReadShaderFile(const char* filename,
                                             std::vector<char>* dst);
    [[nodiscard]] static bool CompileShader(
        glslang_stage_t stage, const char* shaderSource,
        std::vector<unsigned int>* pSpirvBinary);
};
}  // namespace VoxFlow

#endif