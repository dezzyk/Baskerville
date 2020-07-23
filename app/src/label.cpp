//
// Created by Feed on 7/22/2020.
//

#include "label.h"
#include "widget.h"
#include "platform.h"

#include "glm/gtc/matrix_transform.hpp"

void Label::draw(entt::entity entity, entt::registry &reg, std::vector<Draw>& queue) {

    if(reg.valid(entity) && reg.has<Label>(entity)) {
        Widget& widget = reg.get<Widget>(entity);
        Label& label = reg.get<Label>(entity);
        Renderable& renderable = reg.get<Renderable>(entity);

        if(label.font != nullptr) {

            widget.size.y = label.font->getPixelHeight() * (label.pixel_height / label.font->getPixelHeight());

            if (!label.value.empty()) {

                static std::vector<Renderable::Quad> boxes;
                boxes.resize(0);

                i32 xpos = 0;
                for (int i = 0; i < label.value.size(); ++i) {

                    i32 advance;
                    i32 lsb = 0;
                    label.font->getCodepointAdvance(label.pixel_height, label.value.at(i), advance, lsb);

                    // Calc an offset for the glyph to orient it in accordence to the font metrics since they always render from the center.
                    int xoffset = advance / 2;
                    if (i == 0) {
                        xoffset -= lsb; // lsb is always negative
                    }

                    if (label.value.at(i) != ' ' && label.font->getCodepointLayer((u32)label.value.at(i)) != -1) {

                        Renderable::Quad box;
                        box.setColor({1.0f, 1.0f, 1.0f, 1.0f});
                        box.setUVDepth(label.font->getCodepointLayer((u32)label.value.at(i)));

                        glm::mat4 model = glm::mat4(1.0f);
                        glm::vec2 bounding_box_size = label.font->getBoundingBoxSize();
                        model = glm::translate(model, glm::vec3(xpos + xoffset, 0.0f, 0.0f));
                        model = glm::scale(model, glm::vec3(bounding_box_size.x, bounding_box_size.y, 0.0f));

                        box *= model;

                        boxes.push_back(box);

                    }

                    xpos += advance;
                    int kern = 0;
                    if (i < label.value.size() - 1) {
                        xpos += label.font->getKernAdvance(label.pixel_height, label.value.at(i), label.value.at(i + 1));
                    }

                    if(xpos * label.font->calcScale(label.pixel_height) > widget.size.x) {
                        break;
                    }

                }

                if (!boxes.empty()) {
                    renderable.quadUpload(boxes);
                } else {
                    renderable.clear();
                }

            } else {
                renderable.clear();
            }

            if(renderable.size() > 0) {

                glm::vec2 draw_size = Widget::calcDrawSize(entity, reg).value();
                glm::vec2 draw_pos = Widget::calcDrawPos(entity, reg).value();
                f32 scale = Platform::getViewportScaler();

                renderable.model = glm::mat4(1.0f);
                renderable.model = glm::translate(renderable.model, glm::vec3(draw_pos.x - draw_size.x / 2, draw_pos.y, 0.0f));
                renderable.model = glm::scale(renderable.model, glm::vec3(label.font->calcScale(label.pixel_height) * scale,
                                                                          label.font->calcScale(label.pixel_height) * scale, 0.0f));
                renderable.descriptors[0].setValue(Renderable::Descriptor::ArrayTexture(label.font->getHandle().value(), 0));
                renderable.descriptors[1].setValue(label.color);

                queue.push_back({&renderable, Shader::Cache::fetch("msdf")});

            }

        }

    }

}