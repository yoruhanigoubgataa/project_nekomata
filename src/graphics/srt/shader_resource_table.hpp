#pragma once
#include "graphics/vulkan/vk_commands.hpp"
#include "graphics/vulkan/vk_descriptor_set.hpp"
#include "graphics/vulkan/vk_descriptor_set_layout.hpp"
#include "graphics/vulkan/vk_image.hpp"
#include "graphics/vulkan/vk_pipeline_layout.hpp"
#include "graphics/vulkan/vk_sampler.hpp"

namespace nekomata2::graphics::srt {

struct SRTResourceIndex {
    u32 imageIndex;
};

class IShaderResourceTable {
public:
    virtual ~IShaderResourceTable() = default;

    virtual auto allocateImageIndex() -> SRTResourceIndex = 0;
    virtual auto allocateImageIndices(u32 count, std::span<SRTResourceIndex> dstIndices) -> void = 0;
    virtual auto freeImageIndex(SRTResourceIndex index) -> void = 0;
    virtual auto freeImageIndices(std::span<SRTResourceIndex> indices) -> void = 0;

    virtual auto allocateSamplerIndex() -> SRTResourceIndex = 0;
    virtual auto allocateSamplerIndices(u32 count, std::span<SRTResourceIndex> dstIndices) -> void = 0;

    virtual auto bindImage(const VulkanImage& image, SRTResourceIndex index) -> void = 0;
    virtual auto bindSampler(const VulkanSampler& sampler, SRTResourceIndex index) -> void = 0;

    virtual auto bindToCommandBuffer(const VulkanCommandBuffer& cmd, const VulkanPipelineLayout& pipelineLayout, vk::PipelineBindPoint pipelineBindPoint) -> void = 0;

    // temporary
    virtual auto descriptorSetLayout() const -> const VulkanDescriptorSetLayout& = 0;
};

}