#pragma once
#include <functional>
#include <memory>

namespace nekomata2 {
namespace ecs {
class World;
}

auto entry(const std::function<void(std::unique_ptr<ecs::World>&)>& initFn) -> void;
    
auto entryAfterSdlInit(const std::function<void(std::unique_ptr<ecs::World>&)>& initFn) -> void;

}