//
// Created by Feed on 7/22/2020.
//

#include "editor.h"
#include "widget.h"
#include "platform.h"
#include "debug.h"
#include "animation.h"

void Editor::handleText(const Event::TextInput& text, entt::registry& reg) {

    //Should only ever be one. so just grab the first one
    auto editor_view = reg.view<Editor>();
    entt::entity editor_entity = editor_view.front();

    Editor& editor = reg.get<Editor>(editor_entity);
    Label* active_label = &reg.get<Label>(editor.label_entitys[1]);
    Widget* active_widget = &reg.get<Widget>(editor.label_entitys[1]);

    std::string additive = text.value;

    if (!active_label->value.empty()) {
        if(!active_label->value.back() == '.') {
            if(!active_label->value.size() >= 2 && !active_label->value[!active_label->value.size() - 2] != '.') {
                if(additive.back() != ' ') {
                    if(additive.back() != '.') {
                        active_label->value += " ";
                    }
                    active_label->value += std::toupper(additive.back());
                } else {
                    active_label->value += additive.back();
                }
            } else {
                if(additive.back() != ' ') {
                    if(additive.back() != '.') {
                        active_label->value += " ";
                    }
                }
                active_label->value += additive.back();
            }
        } else if (active_label->value.size() >= 3) {
            if(active_label->value.back() == ' ' && active_label->value[active_label->value.size() - 2] == '.' && active_label->value[active_label->value.size() - 3] != '.') {
                active_label->value += std::toupper(additive.back());
            } else {
                active_label->value += additive.back();
            }
        } else {
            active_label->value += additive.back();
        }
    } else {
        active_label->value += additive.back();
    }

    //
    if(active_label->font->calcStringPixelWidth(active_label->value, active_label->pixel_height) > active_widget->size.x) {

        const f32 animation_time = 0.5f;

        if(active_label->value[active_label->value.size() - 1] != ' ') {
            static std::string buffer;
            u32 index = 0;
            while(active_label->value.at(active_label->value.size() - 1 - index) != ' ') {
                ++index;
                if(index == active_label->value.size()) {
                    break;
                }
            }
            buffer = active_label->value.substr(active_label->value.size() - index, index);
            active_label->value.erase(active_label->value.size() - index, index);

            entt::entity back = editor.label_entitys.back();
            {
                editor.label_entitys[5] = editor.label_entitys[4];
                Widget& widget = reg.get<Widget>(editor.label_entitys[5]);
                Label& label = reg.get<Label>(editor.label_entitys[5]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[5], glm::vec2(widget.offset.x, label.pixel_height * 3), glm::vec2(widget.offset.x, label.pixel_height * 4), animation_time, 0.0f);
            }
            {
                editor.label_entitys[4] = editor.label_entitys[3];
                Widget& widget = reg.get<Widget>(editor.label_entitys[4]);
                Label& label = reg.get<Label>(editor.label_entitys[4]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[4], glm::vec2(widget.offset.x, label.pixel_height * 2), glm::vec2(widget.offset.x, label.pixel_height * 3), animation_time, 0.0f);
            }
            {
                editor.label_entitys[3] = editor.label_entitys[2];
                Widget& widget = reg.get<Widget>(editor.label_entitys[3]);
                Label& label = reg.get<Label>(editor.label_entitys[3]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[3], glm::vec2(widget.offset.x, label.pixel_height), glm::vec2(widget.offset.x, label.pixel_height * 2), animation_time, 0.0f);
            }
            {
                editor.label_entitys[2] = editor.label_entitys[1];
                Widget& widget = reg.get<Widget>(editor.label_entitys[2]);
                Label& label = reg.get<Label>(editor.label_entitys[2]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[2], glm::vec2(widget.offset.x, 0.0f), glm::vec2(widget.offset.x, label.pixel_height), animation_time, 0.0f);
            }
            {
                editor.label_entitys[1] = editor.label_entitys[0];
                Widget& widget = reg.get<Widget>(editor.label_entitys[1]);
                Label& label = reg.get<Label>(editor.label_entitys[1]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[1], glm::vec2(widget.offset.x, label.pixel_height * -1), glm::vec2(widget.offset.x, 0), animation_time, 0.0f);
            }
            {
                editor.label_entitys[0] = back;
                Widget& widget = reg.get<Widget>(editor.label_entitys[0]);
                Label& label = reg.get<Label>(editor.label_entitys[0]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[0], glm::vec2(widget.offset.x, label.pixel_height * 4), glm::vec2(widget.offset.x, label.pixel_height * -1), animation_time, 0.0f);
                reg.emplace_or_replace<Animation::Offset::Log>(editor.label_entitys[0]);
            }

            active_label = &reg.get<Label>(editor.label_entitys[1]);
            active_widget = &reg.get<Widget>(editor.label_entitys[1]);
            active_label->value = buffer;
        } else {

            entt::entity back = editor.label_entitys.back();
            {
                editor.label_entitys[5] = editor.label_entitys[4];
                Widget& widget = reg.get<Widget>(editor.label_entitys[5]);
                Label& label = reg.get<Label>(editor.label_entitys[5]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[5], glm::vec2(widget.offset.x, label.pixel_height * 3), glm::vec2(widget.offset.x, label.pixel_height * 4), animation_time, 0.0f);
            }
            {
                editor.label_entitys[4] = editor.label_entitys[3];
                Widget& widget = reg.get<Widget>(editor.label_entitys[4]);
                Label& label = reg.get<Label>(editor.label_entitys[4]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[4], glm::vec2(widget.offset.x, label.pixel_height * 2), glm::vec2(widget.offset.x, label.pixel_height * 3), animation_time, 0.0f);
            }
            {
                editor.label_entitys[3] = editor.label_entitys[2];
                Widget& widget = reg.get<Widget>(editor.label_entitys[3]);
                Label& label = reg.get<Label>(editor.label_entitys[3]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[3], glm::vec2(widget.offset.x, label.pixel_height), glm::vec2(widget.offset.x, label.pixel_height * 2), animation_time, 0.0f);
            }
            {
                editor.label_entitys[2] = editor.label_entitys[1];
                Widget& widget = reg.get<Widget>(editor.label_entitys[2]);
                Label& label = reg.get<Label>(editor.label_entitys[2]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[2], glm::vec2(widget.offset.x, 0.0f), glm::vec2(widget.offset.x, label.pixel_height), animation_time, 0.0f);
            }
            {
                editor.label_entitys[1] = editor.label_entitys[0];
                Widget& widget = reg.get<Widget>(editor.label_entitys[1]);
                Label& label = reg.get<Label>(editor.label_entitys[1]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[1], glm::vec2(widget.offset.x, label.pixel_height * -1), glm::vec2(widget.offset.x, 0), animation_time, 0.0f);
            }
            {
                editor.label_entitys[0] = back;
                Widget& widget = reg.get<Widget>(editor.label_entitys[0]);
                Label& label = reg.get<Label>(editor.label_entitys[0]);
                reg.emplace_or_replace<Animation::Offset>(editor.label_entitys[0], glm::vec2(widget.offset.x, label.pixel_height * 4), glm::vec2(widget.offset.x, ((int)label.pixel_height) * -1), animation_time, 0.0f);
                reg.emplace_or_replace<Animation::Offset::Log>(editor.label_entitys[0]);
            }

            active_label = &reg.get<Label>(editor.label_entitys[1]);
            active_widget = &reg.get<Widget>(editor.label_entitys[1]);
            active_label->value = "";
        }
    }

}

entt::entity Editor::create(entt::registry& reg) {

    entt::entity entity = reg.create();

    Widget& widget = reg.emplace<Widget>(entity);
    widget.alignment = Widget::Align::Center;
    widget.size.x = 800;
    widget.size.y = Platform::getViewportSize().y;
    widget.unscaled_height = true;

    reg.emplace<Debug>(entity);

    Editor& editor = reg.emplace<Editor>(entity);

    int offset_scaler = -1;
    for(auto& label_entity : editor.label_entitys) {
        label_entity = reg.create();

        Widget& widget = reg.emplace<Widget>(label_entity);
        widget.size.x = 800;
        widget.alignment = Widget::Align::Center;
        widget.parent = entity;
        //widget.unscaled_height = true;
        //widget.unscaled_width = true;

        Label& label = reg.emplace<Label>(label_entity);
        label.value = "Test";
        label.font = Font::Cache::fetch("editor");
        label.pixel_height = 32;

        reg.emplace<Debug>(label_entity);
        reg.emplace<Renderable>(label_entity);

        widget.offset.y = (i32)label.pixel_height * offset_scaler;
        ++offset_scaler;
    }

    return entity;

}