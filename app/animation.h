//
// Created by Feed on 7/22/2020.
//

#pragma once

#include "common.h"
#include "widget.h"
#include "label.h"

#include "entt/entt.hpp"
#include "glm/glm.hpp"

#include <iostream>

//TODO: add mote options

namespace Animation {

    struct Offset {

        struct Log{};

        glm::vec2 base_offset = {0.0f, 0.0f};
        glm::vec2 target_offset = {0.0f, 0.0f};
        f32 base_alpha = 1.0f;
        f32 target_alpha = 1.0f;
        f64 time = 0.0;
        f64 accumulator = 0.0;

        static void update(f64 delta, entt::registry &reg) {

            auto view = reg.view<Widget, Animation::Offset>();
            for(auto entity : view) {

                Widget& widget = reg.get<Widget>(entity);
                Offset& anim = reg.get<Offset>(entity);

                f32 t = (anim.accumulator / anim.time);
                if(t >= 0.999f) t = 1.0f; // approx
                f32 m = t - 1.0f;
                f32 scaler = glm::sqrt(1.0f - m * m);

                widget.offset = anim.base_offset + ((anim.target_offset - anim.base_offset) * scaler);
                widget.alpha = anim.base_alpha + ((anim.target_alpha - anim.base_alpha) * scaler);

                if(t == 1.0f) {
                    reg.remove<Offset>(entity);
                } else {
                    anim.accumulator += delta;
                }

                if(reg.has<Animation::Offset::Log>(entity)) {
                    std::cout << "ANIMATION LOG" << std::endl;
                    std::cout << "CUR_OFFSET = <" << widget.offset.x << "," << widget.offset.y << ">" << std::endl;
                    std::cout << "BASE_OFFSET = <" << anim.base_offset.x << "," << anim.base_offset.y << ">" << std::endl;
                    std::cout << "TAR_OFFSET = <" << anim.target_offset.x << "," << anim.target_offset.y << ">" << std::endl;
                }

            }

        }

    };

}
