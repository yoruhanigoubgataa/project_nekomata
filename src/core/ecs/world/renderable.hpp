#pragma once
#include "graphics/meshsystem/mesh_asset_storage.hpp"
#include "graphics/texturesystem/texture_manager.hpp"

namespace nekomata2::ecs::components {

struct Renderable {
    meshsystem::MeshAsset meshAsset;
    graphics::texturesystem::Texture texture;

    Renderable() = default;
    Renderable(meshsystem::MeshAsset meshAsset, graphics::texturesystem::Texture texture) : meshAsset(meshAsset), texture(texture) {}
};

}