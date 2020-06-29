//
// Created by Feed on 5/29/2020.
//

#include "root.h"

#include "gl_err.h"

// NOTE: ALL child widget's must be instantiated shorthand like this
Root::Root(CacheBank& cache) : m_editor(this, cache) {
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
    // Root always needs to have its size set to the viewport size, so explicitly set the scale to 1.0 and keep it there.
    // If we do not set the scale, calcDrawPos breaks for all child widgets
    setScaleAndReportChange(1.0f);
}

Draw::RedrawFlag Root::update(f64 delta) {
    return false;
}

Draw::RedrawFlag Root::onCodepoint(const Event::Codepoint& codepoint) {
    return m_editor.onCodepoint(codepoint);
}

Draw::RedrawFlag Root::onTextInput(const Event::TextInput& text) {
    return m_editor.onTextInput(text);
}

Draw::RedrawFlag Root::onMacro(const Event::Macro& macro) {
    return m_editor.onMacro(macro);
}

Draw::RedrawFlag Root::onMouseClick(Event::MouseClick mouse_click) {
    return false;
}

void Root::draw(Draw::CallQueue& draw_buffer, f32 scale) {

    // Remember Root always needs to have its size set to the viewport size.
    if(Platform::getViewportSize() != m_size) {
        m_size = Platform::getViewportSize();
        m_draw_size = m_size;

        Draw::Box box;
        auto draw_pos = calcDrawPos();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
        // Could just scale by m_size explicitly, but continuing to do it via draw_size for consistencies sake
        model = glm::scale(model, glm::vec3(m_draw_size.x, m_draw_size.y, 0.0f));
        box *= model;
        box.setColor({0.97f, 0.98f, 0.99f, 1.0f});
        m_draw_context.boxUpload(box);

        debugViewUpdate();
    }

    Draw::Call call;
    call.context = &m_draw_context;
    call.shader = m_shader;
    call.count = 6;

    draw_buffer.push_back(call);
    debugViewDraw(draw_buffer);

    m_editor.draw(draw_buffer, scale);

}
