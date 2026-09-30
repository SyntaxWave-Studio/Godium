#include <stdexcept>
#include <algorithm>

#include "application.h"

Application::Application() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

Application::~Application() {
    m_windows.clear();
    glfwTerminate();
}

void Application::addWindow(std::shared_ptr<Window> window) {
    if (!window) return;

    std::vector<std::shared_ptr<Window>>::iterator it = std::find(m_windows.begin(), m_windows.end(), window);
    if (it == m_windows.end()) {
        m_windows.push_back(window);
    }
}

void Application::exec() {
    while (!m_windows.empty()) {
        glfwPollEvents();

        for (std::vector<std::shared_ptr<Window>>::iterator it = m_windows.begin(); it != m_windows.end();) {
            std::shared_ptr<Window> window = *it;

            GLFWwindow* nativeWin = window ? window->m_glfwWindow : nullptr;

            if (!nativeWin || glfwWindowShouldClose(nativeWin)) {
                it = m_windows.erase(it);
            } else {
                window->update();
                ++it;
            }
        }
    }
}
