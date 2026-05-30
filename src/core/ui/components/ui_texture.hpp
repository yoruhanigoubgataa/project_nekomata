#pragma once
#include "core/math/matrix_types.hpp"
#include "graphics/texturesystem/texture_manager.hpp"

namespace nekomata2::ui {

struct UiTexture {
    UiTexture() = default;
    explicit UiTexture(graphics::texturesystem::Texture texture) : texture(texture), texcoordStart(math::Vector2f(0.f)), texcoordEnd(math::Vector2f(1.f)) {}
    explicit UiTexture(graphics::texturesystem::Texture texture, math::Vector2f texcoordStart, math::Vector2f texcoordEnd)
        : texture(texture), texcoordStart(texcoordStart), texcoordEnd(texcoordEnd) {}

    graphics::texturesystem::Texture texture;

    math::Vector2f texcoordStart;
    math::Vector2f texcoordEnd;
};

}