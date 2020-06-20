//
// Created by Feed on 6/20/2020.
//

#include "common.h"

#include "glm/glm.hpp"
#include "glm/gtx/fast_exponential.hpp"

inline f32 flip(f32 val);
inline f32 easeIn(f32 t);
inline f32 easeOut(f32 t);
inline f32 lerp(const f32& start, const f32& end, f32 p);
inline glm::vec2 lerp(const glm::vec2& start, const glm::vec2& end, f32 p);
inline f32 easeInOut(f32 t);

inline f32 flip(f32 val) {
    return 1.0f - val;
}

inline f32 easeIn(f32 t) {
    return t * t;
}

inline f32 easeOut(f32 t) {
    return flip(glm::sqrt(flip(t)));
}

inline f32 easeInOut(f32 t) {
    return lerp(easeIn(t), easeIn(t), t);
}

inline f32 lerp(const f32& start, const f32& end, f32 p) {
    return (start + (end - start) * p);
}

inline glm::vec2 lerp(const glm::vec2& start, const glm::vec2& end, f32 p) {
    return (start + (end - start) * p);
}