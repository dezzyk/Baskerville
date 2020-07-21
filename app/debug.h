//
// Created by Feed on 7/20/2020.
//

#include "common.h"
#include "renderable.h"

#include "entt/entt.hpp"

struct Debug : public Renderable {
    Debug() : Renderable() {
        m_shader = Shader::Cache::fetch("pane");
    }
    static void update(entt::entity entity, entt::registry &reg);
    static void draw(entt::entity entity, entt::registry &reg, std::vector<const Renderable*>& queue);

};
