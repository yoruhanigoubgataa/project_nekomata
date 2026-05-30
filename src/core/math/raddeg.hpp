#pragma once
#include "consts.hpp"

namespace nekomata2::math {

inline float degreesToRadians(float degrees) {
    return degrees * (consts::PI / 180.0f);
}

}
