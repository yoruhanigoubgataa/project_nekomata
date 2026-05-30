#pragma once
#include "components/ui_rect.hpp"
#include "components/ui_text.hpp"
#include "components/ui_texture.hpp"

#include "core/math/matrix_types.hpp"


#include <variant>

namespace nekomata2::ui {

using UiElement = std::variant<UiRect, UiText, UiTexture>;

struct UiNode {
    static auto create() -> std::unique_ptr<UiNode> {
        return std::make_unique<UiNode>();
    }
    math::Vector2f boundsBegin;
    math::Vector2f boundsEnd;

    bool visible = true;

    UiElement element = UiRect();

    std::vector<std::unique_ptr<UiNode>> children;
    UiNode* parent = nullptr;

    UiNode& addChild(std::unique_ptr<UiNode>&& child) {
        children.emplace_back(std::move(child));
        child->parent = this;
        return *children.back();
    }
};

}