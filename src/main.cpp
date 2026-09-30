#include <cmath>
#include <iostream>

#include "engine/application.h"
#include "engine/window.h"
#include "engine/widget.h"

class TestPanel : public Widget {
private:
    float m_animationTime;
    double m_r;
    double m_g;
    double m_b;

public:
    TestPanel(int w, int h, double r, double g, double b) 
        : Widget(w, h), m_animationTime(0.0f), m_r(r), m_g(g), m_b(b) {}

    cairo_surface_t* render() override {
        cairo_surface_t* mainSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
        cairo_t* cr = cairo_create(mainSurface);

        cairo_set_source_rgb(cr, 0.1, 0.1, 0.12);
        cairo_paint(cr);

        cairo_surface_t* sourceSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
        cairo_t* crSource = cairo_create(sourceSurface);
        for (int x = 0; x < m_width; x += 40) {
            for (int y = 0; y < m_height; y += 40) {
                if ((x + y) % 80 == 0) {
                    cairo_set_source_rgb(crSource, m_r, m_g, m_b);
                } else {
                    cairo_set_source_rgb(crSource, 0.2, 0.2, 0.25);
                }
                cairo_rectangle(crSource, x + 2, y + 2, 36, 36);
                cairo_fill(crSource);
            }
        }
        cairo_destroy(crSource);

        cairo_surface_t* maskSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
        cairo_t* crMask = cairo_create(maskSurface);
        
        m_animationTime += 0.03f;
        double maskX = m_width / 2.0 + std::cos(m_animationTime) * 120.0;
        double maskY = m_height / 2.0 + std::sin(m_animationTime * 1.2f) * 80.0;
        
        cairo_set_source_rgba(crMask, 1.0, 1.0, 1.0, 1.0);
        cairo_arc(crMask, maskX, maskY, 100.0, 0, 2 * M_PI);
        cairo_fill(crMask);
        cairo_destroy(crMask);

        cairo_set_source_surface(cr, sourceSurface, 0, 0);
        cairo_mask_surface(cr, maskSurface, 0, 0);

        cairo_surface_destroy(sourceSurface);
        cairo_surface_destroy(maskSurface);
        cairo_destroy(cr);

        return mainSurface;
    }
};

int main() {
    try {
        Application app;

        std::shared_ptr<Window> win = std::make_shared<Window>(640, 480, "Godium");
        std::shared_ptr<TestPanel> panel = std::make_shared<TestPanel>(640, 480, 0.0, 0.7, 0.9);
        
        win->addWidget(panel);
        app.addWindow(win);

        app.exec();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
