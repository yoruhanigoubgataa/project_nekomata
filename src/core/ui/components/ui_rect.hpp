#pragma once
#include "core/math/matrix_types.hpp"

namespace nekomata2::ui {

struct UiRect {
    UiRect() = default;
    explicit UiRect(math::Vector4f color) : color(color) {}

    math::Vector4f color;
};

}