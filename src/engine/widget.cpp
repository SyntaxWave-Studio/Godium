#include "widget.h"

void Widget::addWidget(std::shared_ptr<Widget> widget) {
    if (widget) {
        m_children.push_back(widget);
    }
}

void Widget::removeWidget(std::shared_ptr<Widget> widget) {
    if (!widget) return;
    m_children.erase(
        std::remove(m_children.begin(), m_children.end(), widget), 
        m_children.end()
    );
}

void Widget::resize(int width, int height)
{
    if (m_width != width || m_height != height) {
        resizeEvent(width, height);

        m_width = width;
        m_height = height;

        for (std::shared_ptr<Widget> child : m_children) {
            if (child) {
                child->resize(width, height);
            }
        }
    }
}