//
// Created by Feed on 7/20/2020.
//

#include "app.h"
#include "editor.h"
#include "animation.h"

#include "label.h"

void App::startup() {

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    /*auto editor_test = m_registry.create();
    {
        auto &widget = m_registry.emplace<Widget>(editor_test);
        widget = {};
        widget.size = {512, 512};
        auto &debug = m_registry.emplace<Debug>(editor_test);
    }

    auto label_test = m_registry.create();
    {
        auto &widget = m_registry.emplace<Widget>(label_test);
        widget = {};
        widget.parent = editor_test;
        widget.size.x = 512;
        auto &renderable = m_registry.emplace<Renderable>(label_test);
        auto &label = m_registry.emplace<Label>(label_test);
        label.font = Font::Cache::fetch("editor");
        label.pixel_height = 32;
        label.value = "test";
    }*/

    auto editor = Editor::create(m_registry);


}

void App::shutdown() {

    m_registry = {};

}

void App::update(f64 delta) {

    auto debug = m_registry.view<Widget, Debug>();
    for(auto entity: debug) {
        Debug::updateGeometry(entity, m_registry);
    }

    auto editor = m_registry.view<Widget, Debug>();
    for(auto entity: editor) {
        Widget& widget = m_registry.get<Widget>(entity);
        widget.size.y = Platform::getViewportSize().y;
    }

    //auto animation_offset = m_registry.view<Widget, Animation::Offset>();
    Animation::Offset::update(delta, m_registry);

}

void App::onTextInput(const Event::TextInput& text) {
    Editor::handleText(text, m_registry);
}

void App::onMacro(const Event::Macro& macro) {

}

void App::onMouseClick(const Event::MouseClick& mouse) {

}

void App::draw(std::vector<Draw>& queue) {

    auto debug = m_registry.view<Debug>();
    for (auto entity: debug) {
        Debug::draw(entity, m_registry, queue);
    }

    auto label = m_registry.view<Label>();
    for (auto entity: label) {
        Label::draw(entity, m_registry, queue);
    }

}