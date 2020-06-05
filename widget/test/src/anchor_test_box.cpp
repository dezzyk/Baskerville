//
// Created by Feed on 5/30/2020.
//

#include "test/anchor_test_box.h"

AnchorTestBox::AnchorTestBox(Widget& parent) : Widget(parent), test(*this) {
    m_size = {128, 128};
}

void AnchorTestBox::onCodepoint(const Event::Codepoint& codepoint) {
    test.onCodepoint(codepoint);
}

void AnchorTestBox::onMacro(const Event::Macro& macro) {
    if(macro == Event::Macro::ArrowUp) {
        m_offset.y += 0.05;
        std::cout << m_offset.y << std::endl;
    } else if(macro == Event::Macro::ArrowDown) {
        m_offset.x += 0.05;
    } else if(macro == Event::Macro::ArrowLeft) {
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  :
                m_anchor = Widget::Anchor::BottomRight;
                break;
            case Widget::Anchor::Top  :
                m_anchor = Widget::Anchor::TopLeft;
                break;
            case Widget::Anchor::TopRight  :
                m_anchor = Widget::Anchor::Top;
                break;
            case Widget::Anchor::Left  :
                m_anchor = Widget::Anchor::TopRight;
                break;
            case Widget::Anchor::Center  :
                m_anchor = Widget::Anchor::Left;
                break;
            case Widget::Anchor::Right  :
                m_anchor = Widget::Anchor::Center;
                break;
            case Widget::Anchor::BottomLeft  :
                m_anchor = Widget::Anchor::Right;
                break;
            case Widget::Anchor::Bottom  :
                m_anchor = Widget::Anchor::BottomLeft;
                break;
            case Widget::Anchor::BottomRight  :
                m_anchor = Widget::Anchor::Bottom;
                break;
        }
    } else if(macro == Event::Macro::ArrowRight) {
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  :
                m_anchor = Widget::Anchor::Top;
                break;
            case Widget::Anchor::Top  :
                m_anchor = Widget::Anchor::TopRight;
                break;
            case Widget::Anchor::TopRight  :
                m_anchor = Widget::Anchor::Left;
                break;
            case Widget::Anchor::Left  :
                m_anchor = Widget::Anchor::Center;
                break;
            case Widget::Anchor::Center  :
                m_anchor = Widget::Anchor::Right;
                break;
            case Widget::Anchor::Right  :
                m_anchor = Widget::Anchor::BottomLeft;
                break;
            case Widget::Anchor::BottomLeft  :
                m_anchor = Widget::Anchor::Bottom;
                break;
            case Widget::Anchor::Bottom  :
                m_anchor = Widget::Anchor::BottomRight;
                break;
            case Widget::Anchor::BottomRight  :
                m_anchor = Widget::Anchor::TopLeft;
                break;
        }
    }
}

void AnchorTestBox::onWindowResize(const Event::WindowResize& window_resize) {

}

void AnchorTestBox::draw(Draw::CallQueue& draw_buffer){
    debugDraw(draw_buffer);
    test.draw(draw_buffer);
}