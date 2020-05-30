//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "common.h"

//#include <vector>
#include <queue>

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
        u32 value;
    };

    struct Macro {
        enum class Type {
            SelectAll,
            Backspace,
            Delete,
            Copy,
            Cut,
            Escape,
            Paste,
            Enter,
            Tab,
            Save
        };
        Type type;
    };

    struct Resize {
        u32 w = 0;
        u32 h = 0;
    };

    struct QueueState {
        RingQueue<Codepoint, 128> codepoint;
        RingQueue<Macro, 4> macro;
        RingQueue<Resize, 4> resize;
    };

    class Manager {
    public:
        b32 pollCodepoint(Codepoint& value);
        b32 pollMacro(Macro& value);
        b32 pollResize(Resize& resize);
        friend class ::Window;
    private:
        QueueState m_queue_state;
    };

}