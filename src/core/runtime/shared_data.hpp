#pragma once
#include "core/containers/double_buffer.hpp"
#include "core/ecs/component_pool.hpp"
#include "core/ecs/world/transform.hpp"
#include "core/math/matrix_types.hpp"
#include "core/platform/sdl.hpp"
#include "vulkan/vulkan.hpp"
#include <atomic>
#include <barrier>

#include "core/ecs/world/camera.hpp"
#include "core/ecs/world/renderable.hpp"

namespace nekomata2 {

struct MRThreadsSharedDataLeaf {
    vk::Extent2D m_currentWindowExtent;
    ecs::ComponentSetSnapshot<ecs::components::Renderable> m_renderables;
    ecs::ComponentSetSnapshot<ecs::components::Transform> m_transforms;
    ecs::ComponentSetSnapshot<ecs::components::Camera> m_cameras;
    std::vector<u32> m_textureToImageShaderIndexSnapshot;
    std::vector<u32> m_textureToSamplerShaderIndexSnapshot;
};

class MRThreadsSharedData {
public:
    MRThreadsSharedData(vk::Extent2D windowCurrentRes);

    MRThreadsSharedData(const MRThreadsSharedData&) = delete;
    MRThreadsSharedData(MRThreadsSharedData&&) = delete;
    MRThreadsSharedData& operator=(const MRThreadsSharedData&) = delete;
    MRThreadsSharedData& operator=(MRThreadsSharedData&&) = delete;

    DoubleBuffer<MRThreadsSharedDataLeaf> m_leafs;
    std::barrier<> m_syncpointBarrier;

    std::atomic<bool> m_shouldQuit;
};

}
