//
// Created by Feed on 7/20/2020.
//

#include "app.h"

void App::startup() {

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    test = m_registry.create();
    auto& widget = m_registry.emplace<Widget>(test);
    widget = {};
    widget.size = {512, 512};

    auto& debug = m_registry.emplace<Debug>(test);

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

    auto view = m_registry.view<Widget, Debug>();
    for(auto entity: view) {
        Debug::draw(entity, m_registry, queue);
    }

}