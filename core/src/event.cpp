//
// Created by Feed on 5/26/2020.
//

#include "event.h"

#include <assert.h>

b32 Event::Manager::pollTextInput(u32& value) {
    if(m_queue.text_input.size() > 0) {
        value = m_queue.text_input[0];
        m_queue.text_input.erase(m_queue.text_input.begin(), m_queue.text_input.begin());
        return true;
    }
    return false;
}

b32 Event::Manager::pollMacro(Macro& value) {
    if(m_queue.macro.size() > 0) {
        value = m_queue.macro[0];
        m_queue.macro.erase(m_queue.macro.begin(), m_queue.macro.begin());
        return true;
    }
    return false;
}

b32 Event::Manager::pollResize(Resize& value) {
    if(m_queue.resize.size() > 0) {
        value = m_queue.resize[0];
        m_queue.resize.erase(m_queue.resize.begin(), m_queue.resize.begin());
        return true;
    }
    return false;
}

/*b32 Event::Manager::peekTextInput(u32& value, u32 index) {
    if(index < m_queue.text_input.size()) {
        value = m_queue.text_input[index];
        return true;
    }
    return false;
}*/

/*b32 Event::Manager::peekMacro(Macro& value, u32 index) {
    if(index < m_queue.macro.size()) {
        value = m_queue.macro[index];
        return true;
    }
    return false;
}*/