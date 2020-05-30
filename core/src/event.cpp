//
// Created by Feed on 5/26/2020.
//

#include "event.h"

#include <assert.h>

b32 Event::Manager::pollCodepoint(Codepoint& value) {
    if(m_queue_state.codepoint.size() > 0) {
        value = m_queue_state.codepoint.front();
        m_queue_state.codepoint.pop();
        return true;
    }
    return false;
}

b32 Event::Manager::pollMacro(Macro& value) {
    if(m_queue_state.macro.size() > 0) {
        value = m_queue_state.macro.front();
        m_queue_state.macro.pop();
        return true;
    }
    return false;
}

b32 Event::Manager::pollResize(Resize& resize) {
    if(m_queue_state.resize.size() > 0) {
        resize = m_queue_state.resize.front();
        m_queue_state.resize.pop();
        return true;
    }
    return false;
}