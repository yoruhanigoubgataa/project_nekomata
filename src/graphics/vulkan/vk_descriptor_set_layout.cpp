#include "vk_descriptor_set_layout.hpp"

namespace nekomata2 {

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(std::nullptr_t) {  }
VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(vk::raii::DescriptorSetLayout&& vkDescriptorSetLayout) : m_vkDescriptorSetLayout(std::move(vkDescriptorSetLayout)) {}

auto VulkanDescriptorSetLayout::builder() -> VulkanDescriptorSetLayoutBuilder {
    return {};
}

} // namespace nekomata2