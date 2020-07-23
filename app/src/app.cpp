//
// Created by Feed on 7/20/2020.
//

#include "app.h"

#include "label.h"

void App::startup() {

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    auto editor_test = m_registry.create();
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
        widget.size = {512, 512};
        auto &renderable = m_registry.emplace<Renderable>(label_test);
        auto &label = m_registry.emplace<Label>(label_test);
        label.font = Font::Cache::fetch("editor");
        label.pixel_height = 32;
        label.value = "test";
    }

}

void App::shutdown() {

    m_registry = {};

}

void App::update() {

    auto view = m_registry.view<Widget, Debug>();
    for(auto entity: view) {
        Debug::update(entity, m_registry);
    }

}

void App::draw(std::vector<Draw>& queue) {

    {
        auto view = m_registry.view<Widget, Debug>();
        for (auto entity: view) {
            Debug::draw(entity, m_registry, queue);
        }
    }

    {
        auto view = m_registry.view<Label>();
        for (auto entity: view) {
            Label::draw(entity, m_registry, queue);
        }
    }

}