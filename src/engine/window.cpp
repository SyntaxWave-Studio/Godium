#include <stdexcept>
#include <algorithm>

#include "window.h"

Window::Window(int width, int height, const std::string& title) : Widget(width, height), m_title(title) {
    
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); 
    
    m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    
    if (!m_glfwWindow) {
        throw std::runtime_error("Failed to create GLFW window: " + m_title);
    }

    glfwSetWindowUserPointer(m_glfwWindow, this);
    glfwSetFramebufferSizeCallback(m_glfwWindow, framebufferSizeCallback);

    m_pixelBuffer.resize(m_width * m_height, 0xFF000000);
}

Window::~Window() {
    if (m_glfwWindow) {
        glfwDestroyWindow(m_glfwWindow);
    }
}

void Window::addWidget(std::shared_ptr<Widget> widget) {
    if (!m_children.empty()) {
        throw std::runtime_error("Critical Error: Window '" + m_title + "' cannot have more than 1 child!");
    }
    Widget::addWidget(widget);
}

void Window::close() {
    glfwSetWindowShouldClose(m_glfwWindow, GLFW_TRUE);
}

cairo_surface_t* Window::render() {
    if (m_children.empty()) {
        return cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
    }
    return m_children.front()->render(); 
}

void Window::update() {
    if (!m_glfwWindow) return;
    glfwMakeContextCurrent(m_glfwWindow);

    cairo_surface_t* surface = render();
    
    unsigned char* srcPixels = cairo_image_surface_get_data(surface);
    int stride = cairo_image_surface_get_stride(surface);

    if (srcPixels) {
        for (int y = 0; y < m_height; ++y) {
            uint32_t* srcRow = reinterpret_cast<uint32_t*>(srcPixels + (y * stride));
            std::copy(srcRow, srcRow + m_width, m_pixelBuffer.begin() + (y * m_width));
        }
    }

    cairo_surface_destroy(surface);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, m_pixelBuffer.data());
    glfwSwapBuffers(m_glfwWindow);
}

void Window::resizeEvent(int width, int height)
{
    if (m_glfwWindow) {
        glfwMakeContextCurrent(m_glfwWindow);
        glViewport(0, 0, width, height); 
    }
    m_pixelBuffer.resize(width * height, 0xFF000000);
}

void Window::framebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
    Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
    if (window) {
        window->resize(width, height);
    }
}
