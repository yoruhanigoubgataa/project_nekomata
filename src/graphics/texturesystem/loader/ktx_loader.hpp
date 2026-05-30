#pragma once
#include "graphics/vulkan/vk_image.hpp"
#include "graphics/vulkan/vk_queue.hpp"

#include <filesystem>
#include <ktx.h>

namespace nekomata2::texturesystem {

class KtxLoader {
public:
    static auto loadTextureAsync(const std::filesystem::path path) -> std::tuple<VulkanImage, GPUFuture>;

};

}