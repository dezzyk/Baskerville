//
// Created by Feed on 7/7/2020.
//

#include "menu.h"

Menu::Menu(Widget* parent) : Widget(parent) {
    size.x = 500;
    size.y = getParent()->getSize().y;
    anchor = Anchor::Left;
    m_draw_context = Draw::Context("pane");

    Draw::Quad quad;
    quad.setColor({0.0f, 0.0f, 1.0f, 1.0f});
    m_draw_context.quadUpload(quad);
}

Draw::RedrawFlag Menu::onTextInput(const Event::TextInput& text) {
    return false;
}

Draw::RedrawFlag Menu::onMacro(const Event::Macro& macro) {
    return false;
}

Draw::RedrawFlag Menu::onMouseClick(Event::MouseClick mouse_click) {
    return false;
}

Draw::RedrawFlag Menu::derivedUpdate(f64 delta) {
    return false;
}

void Menu::derivedDraw(Draw::Queue& queue) {

    auto draw_pos = calcDrawPos();
    auto draw_size = calcDrawSize();
    m_draw_context.model = glm::mat4(1.0f);
    m_draw_context.model = glm::translate(m_draw_context.model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
    m_draw_context.model = glm::scale(m_draw_context.model, glm::vec3(draw_size.x, draw_size.y, 0.0f));

    queue.push_back(&m_draw_context);

}