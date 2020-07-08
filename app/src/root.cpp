//
// Created by Feed on 5/29/2020.
//

#include "root.h"

#include "gl_err.h"

// NOTE: ALL child widget's must be instantiated shorthand like this
Root::Root(CacheBank& cache) : Widget(nullptr) {
    size = Platform::getViewportSize();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    m_draw_context = Draw::Context("pane");
    m_editor = std::make_unique<Editor>(this, cache);
    m_menu = std::make_unique<Menu>(this);
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

void Root::derivedDraw(Draw::Queue &queue) {

    // Remember Root always needs to have its size set to the viewport size.
    if(Platform::getViewportSize() != size) {
        size = Platform::getViewportSize();

        Draw::Quad box;
        auto draw_size = calcDrawSize();
        auto draw_pos = calcDrawPos();
        m_draw_context.model = glm::mat4(1.0f);
        m_draw_context.model = glm::translate(m_draw_context.model, glm::vec3(draw_pos.x, draw_pos.y, 0.0f));
        m_draw_context.model = glm::scale(m_draw_context.model, glm::vec3(draw_size.x, draw_size.y, 0.0f));
        box.setColor({0.97f, 0.98f, 0.99f, 1.0f});
        m_draw_context.quadUpload(box);
    }

    queue.push_back(&m_draw_context);
    m_editor->draw(queue);
    m_menu->draw(queue);

}
