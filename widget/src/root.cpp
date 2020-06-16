//
// Created by Feed on 5/29/2020.
//

#include "root.h"

#include "gl_err.h"

// NOTE: ALL child widget's must be instantiated shorthand like this
Root::Root() : m_test(this) {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_shader = Shader::Cache::fetch("box_draw");
    if(m_shader == nullptr) {
        std::string vert =
#include "shader/box_draw.vert"
        ;
        std::string frag =
#include "shader/box_draw.frag"
        ;
        m_shader = Shader::Cache::load("box_draw", vert, frag);
    }
}

void Root::update() {
    //m_editor.update();
}

void Root::onCodepoint(const Event::Codepoint& codepoint) {
    m_test.onCodepoint(codepoint);
}

void Root::onMacro(const Event::Macro& macro) {
    m_test.onMacro(macro);
}

void Root::onMouseClick(Event::MouseClick mouse_click) {
}

void Root::onWindowResize() {
    m_size = Platform::getViewportSize();
    m_draw_size = m_size;

    Draw::Box box;
    glm::vec2 draw_pos = calcViewportPos();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
    model = glm::scale(model, glm::vec3(m_draw_size.x, m_draw_size.y, 0.0f));
    box *= model;
    box.setColor({0.97f, 0.98f, 0.99f, 1.0f});
    m_draw_context.boxUpload(box);

    debugViewUpdate();
    //m_editor.onWindowResize();
    m_test.onWindowResize();
}

void Root::draw(Draw::CallQueue& draw_buffer) {

    Draw::Call call;
    call.context = &m_draw_context;
    call.shader = m_shader;
    call.count = 6;

    draw_buffer.push_back(call);
    debugViewDraw(draw_buffer);
    m_test.draw(draw_buffer);
}
