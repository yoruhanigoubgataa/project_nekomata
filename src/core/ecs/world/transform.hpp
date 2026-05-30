#pragma once
#include "core/math/matrix_types.hpp"
#include "core/math/prelude.hpp"
#include "core/math/quaternion.hpp"
#include "core/math/transform3d.hpp"

using namespace nekomata2::math;

namespace nekomata2::ecs::components {

class Transform {
public:
    Transform() = default;

    Transform3D m_transform3d;
    Matrix4x4f m_modelMatrix;
    bool m_modelMatrixIsDirty{};

    
};

};