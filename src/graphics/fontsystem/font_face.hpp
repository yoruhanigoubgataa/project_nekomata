#pragma once
#include "core/platform/int_def.hpp"

namespace nekomata2::graphics::fonts {

struct FontFace {
    u32 handleIndex;

    bool operator==(const FontFace& other) const {
        return handleIndex == other.handleIndex;
    }
};

}