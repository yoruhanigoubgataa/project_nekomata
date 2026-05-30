#include "mainthread.hpp"
#include "../log/log.hpp"
#include "core/ecs/ecs.hpp"
#include "core/ecs/world/transform.hpp"
#include "core/math/matrix_types.hpp"
#include "core/math/transform3d.hpp"
#include "graphics/cmd_alloc/cmd_alloc.hpp"

#include <atomic>
#include <chrono>
#include <cmath>
#include <memory>
#include <utility>

namespace nekomata2 {

MainThread::MainThread(std::shared_ptr<MRThreadsSharedData> mrSharedData, std::unique_ptr<VulkanContext>&& vkContext, SdlWindow&& sdlWindow)
    : m_sdlWindow(std::move(sdlWindow)), m_mrSharedData(std::move(std::move(mrSharedData))), m_vkContext(std::move(vkContext)) {

    cmdalloc::VulkanCommandPoolsList::initThreadLocalCommandPools();

    m_currentWorld = std::make_unique<ecs::World>();
    m_meshAssetStorage = meshsystem::MeshAssetStorage::create();
    m_textureManager = graphics::texturesystem::TextureManager::create();
    m_fontManager = graphics::fonts::FontManager::create();
}

auto MainThread::runMainLoop(const std::function<void(std::unique_ptr<ecs::World>&)>& initFn) -> void {
    initFn(m_currentWorld);

    using clock = std::chrono::steady_clock;

    auto previousClk = clock::now();
    while (true) {
        m_mrSharedData->m_syncpointBarrier.arrive_and_wait();

        if (m_mrSharedData->m_shouldQuit.load(std::memory_order_acquire)) {
            break;
        }
        m_mrSharedData->m_leafs.swap();

        m_mrSharedData->m_syncpointBarrier.arrive_and_wait();

        auto currentClk = clock::now();
        std::chrono::duration<float> delta = currentClk - previousClk;
        previousClk = currentClk;

        loop(delta.count());
    }
    log::info("Main Thread exiting...");

    cmdalloc::VulkanCommandPoolsList::destroyThreadLocalCommandPools();
}

auto MainThread::loop(float dt) -> void {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            m_mrSharedData->m_shouldQuit.store(true, std::memory_order_release);
            return;
        }
    }

    m_currentWorld->scriptsUpdate(dt);

    m_mrSharedData->m_leafs.getPrimary().m_currentWindowExtent = m_sdlWindow.vulkanGetDrawableSize();

    if (m_currentWorld) {
        m_currentWorld->components<ecs::components::Renderable>().copyTo(m_mrSharedData->m_leafs.getPrimary().m_renderables);
        m_currentWorld->components<ecs::components::Transform>().copyTo(m_mrSharedData->m_leafs.getPrimary().m_transforms);
        m_currentWorld->components<ecs::components::Camera>().copyTo(m_mrSharedData->m_leafs.getPrimary().m_cameras);
    }
    m_textureManager.get()->textureToShaderIndexTable().snapshotTables(
        m_mrSharedData->m_leafs.getPrimary().m_textureToImageShaderIndexSnapshot,
        m_mrSharedData->m_leafs.getPrimary().m_textureToSamplerShaderIndexSnapshot
    );

}

}