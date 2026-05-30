#include "ktx_loader.hpp"

#include "core/log/log.hpp"
#include "core/platform/int_def.hpp"
#include "graphics/cmd_alloc/cmd_alloc.hpp"
#include "graphics/vulkan/vk_buffer.hpp"
#include "graphics/vulkan/vk_commands_barriers.hpp"
#include "graphics/vulkan/vk_image.hpp"
#include "ktxvulkan.h"

namespace nekomata2::texturesystem {

auto KtxLoader::loadTextureAsync(const std::filesystem::path path) -> std::tuple<VulkanImage, GPUFuture> {
    ktxTexture2* textureObject;
    KTX_error_code result = ktxTexture2_CreateFromNamedFile(path.c_str(), KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT, &textureObject);

    if (result != KTX_SUCCESS) {
        log::error("Failed to load texture: {}", path.string());
        throw std::runtime_error("failed to load texture");
    }

    if (ktxTexture2_NeedsTranscoding(textureObject)) {
        log::warn("Texture {} needs transcoding!", path.string());
        ktx_transcode_fmt_e targetFormat = KTX_TTF_BC7_RGBA;

        KTX_error_code transcodeResult = ktxTexture2_TranscodeBasis(textureObject, targetFormat, 0);
        if (transcodeResult != KTX_SUCCESS) {
            log::error("Failed to transcode texture: {}", path.string());
            throw std::runtime_error("failed to transcode texture");
        }
    }

    u32 textureWidth = textureObject->baseWidth;
    u32 textureHeight = textureObject->baseHeight;
    u32 textureDepth = textureObject->baseDepth;
    u32 textureArrayLayers = textureObject->numLayers;
    u32 textureMipLevels = textureObject->numLevels;
    u32 textureByteSize = textureObject->dataSize;
    u8* textureData = textureObject->pData;
    auto textureFormat = static_cast<vk::Format>(ktxTexture2_GetVkFormat(textureObject));
    auto textureUsage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    auto textureTiling = vk::ImageTiling::eOptimal;

    // TODO: change the thing: GPU staging buffer -> CPU staging buffer, async compute copy -> transfer copy
    log::info("Loaded texture: {} ({}x{}x{}x{}) ByteSize: {} Format: {}", path.string(), textureWidth, textureHeight, textureDepth, textureArrayLayers, textureByteSize, vk::to_string(textureFormat));

    auto buffer = VulkanBuffer::create(textureByteSize, vk::BufferUsageFlagBits::eTransferSrc, VulkanBufferMemoryMapping::MapForSequentialWrite, vma::MemoryUsage::eAutoPreferDevice, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, VulkanContext::get().vkPhysicalDeviceProps().m_queueFamilies[QueueFamily::AsyncCompute]);
    memcpy(buffer.memoryHostPtr(), textureData, textureByteSize);

    auto image = VulkanImage::create(vk::ImageType::e2D, vk::Extent3D { textureWidth, textureHeight, textureDepth }, 1, 1, textureFormat, textureUsage, textureTiling, vma::MemoryUsage::eAutoPreferDevice, {}, VulkanContext::get().vkPhysicalDeviceProps().m_queueFamilies[QueueFamily::Graphics | QueueFamily::AsyncCompute], vk::ImageLayout::eUndefined);

    auto cmd = cmdalloc::VulkanCommandPoolsList::getAssignedAsyncComputeCommandPool().allocateCommandBuffer(vk::CommandBufferLevel::ePrimary);

    auto& cb = cmd.vkCommandBuffer();

    auto cbBeginInfo = vk::CommandBufferBeginInfo{}
        .setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

    cb.begin(cbBeginInfo);
    VulkanPipelineBarriers::builder()
        .insertImageMemoryBarrier(
            image,
            vk::ImageLayout::eUndefined, vk::PipelineStageFlagBits2::eTransfer, vk::AccessFlagBits2::eNone,
            vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits2::eTransfer, vk::AccessFlagBits2::eTransferWrite
        )
        .flush(cmd);

    auto subresLayers = vk::ImageSubresourceLayers{}
        .setAspectMask(vk::ImageAspectFlagBits::eColor)
        .setBaseArrayLayer(0)
        .setLayerCount(1)
        .setMipLevel(0);

    auto copyRegion = vk::BufferImageCopy2{}
        .setImageExtent(image.extent())
        .setImageOffset(vk::Offset3D{0, 0, 0})
        .setImageSubresource(subresLayers)
        .setBufferOffset(0)
        .setBufferImageHeight(0)
        .setBufferRowLength(0);

    auto copyInfo = vk::CopyBufferToImageInfo2{}
        .setDstImage(image.vkImage())
        .setDstImageLayout(vk::ImageLayout::eTransferDstOptimal)
        .setSrcBuffer(buffer.vkBuffer())
        .setRegions(copyRegion);

    cb.copyBufferToImage2(copyInfo);

    VulkanPipelineBarriers::builder()
        .insertImageMemoryBarrier(
            image,
            vk::ImageLayout::eTransferDstOptimal, vk::PipelineStageFlagBits2::eTransfer, vk::AccessFlagBits2::eTransferWrite,
            vk::ImageLayout::eShaderReadOnlyOptimal, vk::PipelineStageFlagBits2::eNone, vk::AccessFlagBits2::eNone
        )
        .flush(cmd);
    cb.end();

    auto asyncOp = VulkanContext::get().vkQueueAsyncCompute().submitOneCommandBuffer(cb, {}, {}, {});
    ktxTexture2_Destroy(textureObject);
    return std::make_tuple(std::move(image), std::move(asyncOp));
}

} // namespace nekomata2::texturesystem