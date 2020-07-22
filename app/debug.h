//
// Created by Feed on 7/20/2020.
//

#include "common.h"
#include "renderable.h"
#include "draw.h"

#include "entt/entt.hpp"

// Debug inherits from Renderable to isolate its own rendering from any Renderable components. All widget types should
// be standalone and reference an entities Renderable component rather than inherit from it
struct Debug : public Renderable {
    Debug() : Renderable() {};
    static void update(entt::entity entity, entt::registry &reg);
    static void draw(entt::entity entity, entt::registry &reg, std::vector<Draw>& queue);

};
