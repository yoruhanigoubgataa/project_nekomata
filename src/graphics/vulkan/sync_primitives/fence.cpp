#include "fence.hpp"
#include "graphics/vulkan/context.hpp"
#include <cstddef>
#include "core/platform/int_def.hpp"

namespace nekomata2 {

VulkanFence::VulkanFence(std::nullptr_t) {}
VulkanFence::VulkanFence(vk::raii::Fence&& vkFence) : m_vkFence(std::move(vkFence)) {}

auto VulkanFence::create(bool signaled) -> VulkanFence {
    auto createInfo = vk::FenceCreateInfo{};

    if (signaled) {
        createInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);
    }
    
    auto handle = VulkanContext::get().vkDevice().createFence(createInfo);
    return VulkanFence(std::move(handle));
}

auto VulkanFence::waitForSignal(u64 timeoutNanos) const -> void {
    VulkanContext::get().vkDevice().waitForFences(*m_vkFence.vkHandle(), true, timeoutNanos);
}

auto VulkanFence::reset() const -> void {
    VulkanContext::get().vkDevice().resetFences(*m_vkFence.vkHandle());
}

}