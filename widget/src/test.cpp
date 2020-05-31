//
// Created by Feed on 5/30/2020.
//

#include "test.h"

TestWidget::TestWidget() {
    m_size = {128, 128};
}

TestWidget::TestWidget(Widget& parent) : Widget(parent) {
    m_size = {128, 128};
}

void TestWidget::onCodepoint(const Event::Codepoint& codepoint) {

}

void TestWidget::onMacro(const Event::Macro& macro) {
    if(macro == Event::Macro::ArrowUp) {
        m_offset.y += 0.05;
        std::cout << m_offset.y << std::endl;
    } else if(macro == Event::Macro::ArrowDown) {
        m_offset.x += 0.05;
    } else if(macro == Event::Macro::ArrowLeft) {
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  :
                m_anchor = Widget::Anchor::Center;
                break; //optional
            case Widget::Anchor::Top  :
                m_anchor = Widget::Anchor::TopLeft;
                break; //optional
            case Widget::Anchor::TopRight  :
                m_anchor = Widget::Anchor::Top;
                break; //optional
            case Widget::Anchor::Left  :
                m_anchor = Widget::Anchor::TopRight;
                break; //optional
            case Widget::Anchor::Right  :
                m_anchor = Widget::Anchor::Left;
                break; //optional
            case Widget::Anchor::BottomLeft  :
                m_anchor = Widget::Anchor::Right;
                break; //optional
            case Widget::Anchor::Bottom  :
                m_anchor = Widget::Anchor::BottomLeft;
                break; //optional
            case Widget::Anchor::BottomRight  :
                m_anchor = Widget::Anchor::Bottom;
                break; //optional
            case Widget::Anchor::Center  :
                m_anchor = Widget::Anchor::BottomRight;
                break; //optional
        }
    } else if(macro == Event::Macro::ArrowRight) {
        switch (m_anchor) {
            case Widget::Anchor::TopLeft  :
                m_anchor = Widget::Anchor::Top;
                break; //optional
            case Widget::Anchor::Top  :
                m_anchor = Widget::Anchor::TopRight;
                break; //optional
            case Widget::Anchor::TopRight  :
                m_anchor = Widget::Anchor::Left;
                break; //optional
            case Widget::Anchor::Left  :
                m_anchor = Widget::Anchor::Right;
                break; //optional
            case Widget::Anchor::Right  :
                m_anchor = Widget::Anchor::BottomLeft;
                break; //optional
            case Widget::Anchor::BottomLeft  :
                m_anchor = Widget::Anchor::Bottom;
                break; //optional
            case Widget::Anchor::Bottom  :
                m_anchor = Widget::Anchor::BottomRight;
                break; //optional
            case Widget::Anchor::BottomRight  :
                m_anchor = Widget::Anchor::Center;
                break; //optional
            case Widget::Anchor::Center  :
                m_anchor = Widget::Anchor::TopLeft;
                break; //optional
        }
    }
}

void TestWidget::onWindowResize(const Event::WindowResize& window_resize) {

}

void TestWidget::draw(const Window::Viewport& viewport){
    debugDraw(viewport);
}