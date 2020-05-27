//
// Created by Feed on 5/26/2020.
//

#pragma once

#include "common.h"

#include <vector>

class Window;

namespace Event {

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
        Save
    };

    struct Resize {
        u32 w = 0;
        u32 h = 0;
    };

    struct Queue {
        std::vector<u32> text_input;
        std::vector<Macro> macro;
        std::vector<Resize> resize;
    };

    class Manager {
    public:
        b32 pollTextInput(u32& value);
        b32 pollMacro(Macro& value);
        b32 pollResize(Resize& value);
        //b32 peekTextInput(u32& value, u32 index);
        //b32 peekMacro(Macro& value, u32 index);
        friend class ::Window;
    private:
        Queue m_queue;
    };

}