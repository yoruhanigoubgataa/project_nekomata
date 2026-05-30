#pragma once
#include "core/ecs/component_pool.hpp"
#include "core/ecs/world/camera.hpp"
#include "core/ecs/world/transform.hpp"
#include "core/runtime/shared_data.hpp"
#include "graphics/vulkan/sync_primitives/binary_semaphore.hpp"
#include "graphics/vulkan/sync_primitives/fence.hpp"
#include "graphics/vulkan/vk_buffer.hpp"
#include "graphics/vulkan/vk_commands.hpp"
#include "graphics/vulkan/vk_image.hpp"

namespace nekomata2::graphics {

/// Per-frame rendering resources used exclusively by each frame and only by that frame.
///
/// # Access Contingency
/// | CPU Read/Write | GPU Access Scope across Queue | GPU Visibility Scope across Queue |
/// |----------------|-------------------------------|-----------------------------------|
/// | Yes            | Exclusive                     | Exclusive                         |
///
class FrameRenderingResources {
public:
    FrameRenderingResources(std::nullptr_t);
    FrameRenderingResources(u32 initialMaxObjects);


    auto commandPool() -> VulkanCommandPool& { return m_commandPool; }
    auto commandBuffer() -> VulkanCommandBuffer& { return m_commandBuffer; }

    auto transformsBuffer() -> VulkanBuffer& { return m_transformsBuffer; }
    auto glyphInstanceBuffer() -> VulkanBuffer& { return m_glyphInstanceBuffer; }

    auto frameDoneFence() -> VulkanFence& { return m_frameDoneFence; }
    auto imageAcquiredSemaphore() -> VulkanBinarySemaphore& { return m_imageAcquiredSemaphore; }

    auto prepareTransformsBuffer(MRThreadsSharedDataLeaf& renderingData, ecs::components::Camera camera, const ecs::components::Transform& cameraTransform, float renderAspectRatio) -> void;

private:
    // --------------------------------------------------------------------------------------------------------------------------------------------------------
    // Commands
    VulkanCommandPool m_commandPool = nullptr;
    VulkanCommandBuffer m_commandBuffer = nullptr;

    // --------------------------------------------------------------------------------------------------------------------------------------------------------
    // Buffers
    VulkanBuffer m_transformsBuffer = nullptr;

    // temporary, for font rendering demo
    VulkanBuffer m_glyphInstanceBuffer = nullptr;

    // --------------------------------------------------------------------------------------------------------------------------------------------------------
    // Synchronization

    /// Signaled after the frame has completed rendering
    VulkanFence m_frameDoneFence = nullptr;

    /// Signaled after a successful swapchain image acquire
    VulkanBinarySemaphore m_imageAcquiredSemaphore = nullptr;
};

}