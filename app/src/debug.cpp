//
// Created by Feed on 7/20/2020.
//

#include "widget.h"
#include "debug.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void Debug::update(entt::entity entity, entt::registry &reg) {

    Debug& debug = reg.get<Debug>(entity);

    static std::array<Renderable::Quad, 8> boxes;
    boxes = {};

    glm::vec2 draw_size = Widget::calcDrawSize(entity, reg).value();
    glm::vec2 draw_pos = Widget::calcDrawPos(entity, reg).value();

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 2.0f, draw_pos.y + (draw_size.y / 2) - 8.0, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
    boxes[0] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 8.0f, draw_pos.y + (draw_size.y / 2) - 2, 0.0f));
    model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
    boxes[1] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 2.0f, draw_pos.y - (draw_size.y / 2) + 8.0, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
    boxes[2] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x - (draw_size.x / 2) + 8.0f, draw_pos.y - (draw_size.y / 2) + 2.0, 0.0f));
    model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
    boxes[3] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 2, draw_pos.y - (draw_size.y / 2) + 8.0, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
    boxes[4] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 8, draw_pos.y - (draw_size.y / 2) + 2.0, 0.0f));
    model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
    boxes[5] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 2, draw_pos.y + (draw_size.y / 2) - 8, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 8.0f, 0.0f));
    boxes[6] *= model;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x + (draw_size.x / 2) - 8, draw_pos.y + (draw_size.y / 2) - 2, 0.0f));
    model = glm::scale(model, glm::vec3(8.0f, 2.0f, 0.0f));
    boxes[7] *= model;

    for(auto& box : boxes) {
        for(auto& vert : box.vertices) {
            vert.color = {1.0f, 0.0f, 0.0f, 0.5f};
        }
    }

    debug.model = glm::mat4(1.0f);
    debug.quadUpload<8>(boxes);

}

void Debug::draw(entt::entity entity, entt::registry &reg, std::vector<Draw>& queue) {
    Debug& debug = reg.get<Debug>(entity);
    queue.push_back({&debug, Shader::Cache::fetch("pane")});
}