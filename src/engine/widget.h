#pragma once

#include <cairo.h>
#include <vector>
#include <memory>
#include <algorithm>

class Widget {

public:
    Widget(int width, int height) : m_width(width), m_height(height) {}
    virtual ~Widget() = default;

    int width() const { return m_width; }
    int height() const { return m_height; }

    virtual void addWidget(std::shared_ptr<Widget> widget);
    virtual void removeWidget(std::shared_ptr<Widget> widget);

    const std::vector<std::shared_ptr<Widget>>& children() const { return m_children; }
    
    virtual cairo_surface_t* render() = 0;

protected:
    int m_width;
    int m_height;
    
    std::vector<std::shared_ptr<Widget>> m_children;

    void resize(int width, int height);
    virtual void resizeEvent(int width, int height) {}

};
