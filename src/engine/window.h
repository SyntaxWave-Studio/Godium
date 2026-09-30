#pragma once

#include <GLFW/glfw3.h>
#include <string>

#include "widget.h"

class Window : public Widget {
    
    friend class Application; 

public:
    Window(int width, int height, const std::string& title);
    ~Window() override;

    void addWidget(std::shared_ptr<Widget> widget) override;

    cairo_surface_t* render() override;

    void update();
    void close();

protected:
    GLFWwindow* m_glfwWindow = nullptr;

    std::string m_title;
    std::vector<uint32_t> m_pixelBuffer;

    void resizeEvent(int width, int height) override;

private:
    static void framebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height);

};
