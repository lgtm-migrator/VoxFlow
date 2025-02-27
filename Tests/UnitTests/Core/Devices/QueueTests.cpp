// Author : snowapril

#include <VoxFlow/Core/Devices/Instance.hpp>
#include <VoxFlow/Core/Devices/LogicalDevice.hpp>
#include <VoxFlow/Core/Devices/PhysicalDevice.hpp>
#include <VoxFlow/Core/Devices/Queue.hpp>
#include "../../UnitTestUtils.hpp"

TEST_CASE("Vulkan Queue Initialization")
{
    VoxFlow::Instance instance(gVulkanContext);
    VoxFlow::PhysicalDevice physicalDevice(instance);
    VoxFlow::LogicalDevice logicalDevice(gVulkanContext, physicalDevice);

    std::shared_ptr<VoxFlow::Queue> gctQueue =
        logicalDevice.getQueuePtr("GCT").lock();
    CHECK_NE(gctQueue->get(), VK_NULL_HANDLE);
}