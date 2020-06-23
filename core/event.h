//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "common.h"

#include "glm/glm.hpp"

#include <optional>

class Window;

namespace Event {

    template<typename T, u32 capacity>
    class RingQueue { // Circular queue
    public:
        T front() {
            return m_buffer[m_head];
        }
        u32 size() {
            return m_size;
        }
        void pop() {
            if(m_head != m_tail) {
                m_head = (m_head % (capacity)) + 1;
                m_size--;
            }
        }
        void push(T value) {
            if(((m_tail % (capacity)) + 1) != m_head) {
                m_buffer[m_tail] = value;
                m_tail = (m_tail % (capacity)) + 1;
                m_size++;
            }
        }
    private:
        T m_buffer[capacity + 1];
        u32 m_head = 0;
        u32 m_tail = 0;
        u32 m_size = 0;
    };

    struct Codepoint {
        u32 value = 0;
    };

    enum class Macro {
        SelectAll,
        Backspace,
        Delete,
        Copy,
        Cut,
        Escape,
        Paste,
        Enter,
        Tab,
        Save,
        Open,
        New,
        ArrowUp,
        ArrowDown,
        ArrowLeft,
        ArrowRight
    };

    struct MouseClick {
        glm::vec2 pos = {0.0f, 0.0f};
        b32 button = 0;
    };

}