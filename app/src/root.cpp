//
// Created by Feed on 5/29/2020.
//

#include "root.h"

#include "gl_err.h"

// NOTE: ALL child widget's must be instantiated shorthand like this
Root::Root(CacheBank& cache) : Widget(nullptr) {
    size = Platform::getViewportSize();
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
    m_editor = std::make_unique<Editor>(this, cache);
}

Draw::RedrawFlag Root::derivedUpdate(f64 delta) {
    return m_editor->update(delta);
}

Draw::RedrawFlag Root::onTextInput(const Event::TextInput& text) {
    return m_editor->onTextInput(text);
}

Draw::RedrawFlag Root::onMacro(const Event::Macro& macro) {
    return m_editor->onMacro(macro);
}

Draw::RedrawFlag Root::onMouseClick(Event::MouseClick mouse_click) {
    return false;
}

void Root::derivedDraw(Draw::CallQueue &draw_buffer) {

    // Remember Root always needs to have its size set to the viewport size.
    if(Platform::getViewportSize() != size) {
        size = Platform::getViewportSize();

        Draw::Box box;
        auto draw_size = calcDrawSize();
        auto draw_pos = calcDrawPos();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
        model = glm::scale(model, glm::vec3(draw_size.x, draw_size.y, 0.0f));
        box *= model;
        box.setColor({0.97f, 0.98f, 0.99f, 1.0f});
        m_draw_context.boxUpload(box);
    }

    Draw::Call call;
    call.context = &m_draw_context;
    call.shader = m_shader;
    call.count = 6;

    draw_buffer.push_back(call);

    m_editor->draw(draw_buffer);

}
