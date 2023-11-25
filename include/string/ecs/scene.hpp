#pragma once

#include <vector>
namespace String {

class Entity;

class Scene {
public:
    explicit Scene();

    auto create_entity() -> void;
    auto destroy_entity() -> void;
    
private:
    std::vector<Entity> entities_;
};

}