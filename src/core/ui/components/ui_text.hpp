#pragma once
#include "graphics/fontsystem/font_face.hpp"

#include <string>

namespace nekomata2::ui {

struct UiText {
    UiText() = default;
    explicit UiText(std::string_view text, float size, graphics::fonts::FontFace&& fontFace)
        : size(size), fontFace(std::move(fontFace)), text(text) {}

    float size;
    graphics::fonts::FontFace fontFace;
    std::string text;
};

}