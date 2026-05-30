#pragma once

#include "core/ecs/entity.hpp"
namespace nekomata2::ecs {

class World;
class ScriptBase {
public:
    Entity   m_workingEntity;
    World*   m_workingWorld{};

    virtual ~ScriptBase() = default;

    virtual void onCreate()  {}
    virtual void onDestroy() {}

    virtual void onUpdate(float dt) {}
};

}