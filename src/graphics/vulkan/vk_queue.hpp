#pragma once
#include "core/platform/int_def.hpp"
#include "vulkan_prelude.hpp"

#include <atomic>
#include <cstddef>
#include <memory>
#include <mutex>
#include <span>

namespace nekomata2 {

class GPUFuture {
public:
    GPUFuture(const vk::raii::Semaphore& timelineSemaphore, u64 timelineSemaphoreTargetValue)
        : m_timelineSemaphore(timelineSemaphore), m_timelineSemaphoreTargetValue(timelineSemaphoreTargetValue) {}

    [[nodiscard]] auto timelineSemaphore() const -> const vk::raii::Semaphore& { return m_timelineSemaphore; }
    [[nodiscard]] auto timelineSemaphoreTargetValue() const -> u64 { return m_timelineSemaphoreTargetValue; }

    auto await(u64 timeout = UINT64_MAX) const -> void;

private:
    std::reference_wrapper<const vk::raii::Semaphore> m_timelineSemaphore;
    u64 m_timelineSemaphoreTargetValue;
};

class VulkanSwapchain;
class VulkanFence;
class VulkanBinarySemaphore;
class VulkanQueue {
public:
    VulkanQueue(std::nullptr_t);
    VulkanQueue(vk::raii::Queue vkQueue, vk::raii::Semaphore vkTimelineSemaphore, u64 lastTimelineSubmissionValue);
    ~VulkanQueue();

    VulkanQueue(const VulkanQueue&) = delete;
    VulkanQueue(VulkanQueue&&) = delete;
    VulkanQueue& operator=(const VulkanQueue&) = delete;
    VulkanQueue& operator=(VulkanQueue&&) = delete;

    auto submitOneCommandBuffer(const vk::raii::CommandBuffer& buf, const std::span<GPUFuture>& asyncWaits,
                                   const std::span<vk::PipelineStageFlags2>& asyncWaitStages, const std::optional<std::reference_wrapper<VulkanFence>>& fence)
        -> GPUFuture;
    auto submitOneCommandBufferWithBinarySemaphores(const vk::raii::CommandBuffer& buf, const std::span<GPUFuture>& asyncWaits,
                                              const std::span<vk::PipelineStageFlags2>& asyncWaitStages, const VulkanBinarySemaphore& binaryWait,
                                              const VulkanBinarySemaphore& binarySignal, const vk::PipelineStageFlags2& binaryWaitStage,
                                              const vk::PipelineStageFlags2& binarySignalStage,
                                              const std::optional<std::reference_wrapper<VulkanFence>>& fence) -> GPUFuture;
    auto submitPresent(const VulkanSwapchain& swapchain, const VulkanBinarySemaphore& waitSemaphore, u32 imageIndex) -> vk::Result;
    auto waitIdle() -> void;

    auto timelineSemaphore() -> const vk::raii::Semaphore& { return m_timelineSemaphore; }
    auto lastTimelineSubmissionValue() const -> u64 {
        return m_lastTimelineSubmissionValue.load(std::memory_order_relaxed);
    }
    auto currentTimelineValue() const -> u64 {
        return m_timelineSemaphore.getCounterValue();
    }

private:
    mutable std::mutex m_queueMutex;
    vk::raii::Queue m_vkQueue = nullptr;
    vk::raii::Semaphore m_timelineSemaphore = nullptr;
    std::atomic<u64> m_lastTimelineSubmissionValue = 0;
};

} // namespace nekomata2
