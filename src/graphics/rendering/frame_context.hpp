#pragma once
#include "core/runtime/shared_data.hpp"
#include "frame_rendering_resources.hpp"
#include "graphics/vulkan/vk_swapchain.hpp"
#include "shared_rendering_resources.hpp"
#include "transient_rendering_resources.hpp"

namespace nekomata2::graphics {

struct FrameResult {
    bool shouldRecreateSwapchain = false;
    bool stepPerFrameResources = false;
};

class FrameContext {
public:
    FrameContext(std::nullptr_t);
    FrameContext();

    [[nodiscard]] auto execute(TransientRenderingResources& transientRenderingResources, SharedRenderingResources& sharedRenderingResources,
                               VulkanSwapchain& swapchain, MRThreadsSharedDataLeaf& renderingData, float timeSinceStart) -> FrameResult;

private:
    FrameRenderingResources m_frameRenderingResources = nullptr;

};

}