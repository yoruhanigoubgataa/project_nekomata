#include "vk_pipeline_layout.hpp"
#include "graphics/vulkan/context.hpp"
#include "core/platform/int_def.hpp"
#include "vulkan/vulkan.hpp"
#include <cstddef>
#include <vulkan/vulkan_raii.hpp>

namespace nekomata2 {


VulkanPipelineLayout::VulkanPipelineLayout(std::nullptr_t) : m_vkPipelineLayout(nullptr) {}
VulkanPipelineLayout::VulkanPipelineLayout(vk::raii::PipelineLayout&& vkPipelineLayout) : m_vkPipelineLayout(std::move(vkPipelineLayout)) {}

auto VulkanPipelineLayout::builder() -> VulkanPipelineLayoutBuilder {
    return {};
}

}