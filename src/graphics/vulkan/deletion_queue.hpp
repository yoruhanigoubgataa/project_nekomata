#pragma once
#include <barrier>
#include <condition_variable>
#include <print>
#include <thread>
#include <variant>
#include <vulkan/vulkan_raii.hpp>
#include "vulkan_prelude.hpp"
#include "graphics/vulkan/vk_gpu_obrm_structs.hpp"
#include "core/platform/int_def.hpp"
#include "core/containers/mpsc_queue.hpp"

namespace nekomata2 {

using AnyVulkanObject = std::variant<vk::raii::CommandPool, vk::raii::CommandBuffer, vk::raii::Image, vk::raii::ImageView, vk::raii::SwapchainKHR,
                                     vk::raii::Semaphore, vk::raii::Fence, vma::raii::Allocation, vk::raii::PipelineLayout, vk::raii::Pipeline,
                                     vk::raii::Buffer, vma::raii::VirtualBlock, vk::raii::DescriptorSetLayout, vk::raii::DescriptorSet,
                                     vk::raii::DescriptorPool, vk::raii::Sampler>;

struct ResourceDeletionQueueEntry {
    u64 m_graphicsQueueRetireValue;
    u64 m_asyncComputeQueueRetireValue;
    AnyVulkanObject m_vkObject;
};

class VulkanResourceDeletionQueue {
public:
    ~VulkanResourceDeletionQueue();
    auto run() -> void;
    static auto get() -> VulkanResourceDeletionQueue&;
    auto pushObject(const GpuResourceRetireTimelineValues& marker, AnyVulkanObject&& obj) -> void;

private:
    auto workerRoutine() -> void;

    std::thread m_workerThread;
    std::barrier<> m_endSyncBarrier = std::barrier(2);
    std::atomic<bool> m_shouldRun = std::atomic(true);
    AtomicMpscQueue<ResourceDeletionQueueEntry> m_objectsMpscQueue;

    std::mutex m_cvLock;
    std::condition_variable m_queueCv;
};

inline VulkanResourceDeletionQueue* g_vkResourceDeletionQueue = nullptr;

}