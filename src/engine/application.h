#pragma once

#include <vector>
#include <memory>

#include "window.h"

class Application {

public:
    Application();
    ~Application();

    void addWindow(std::shared_ptr<Window> window);
    
    void exec();

private:
    std::vector<std::shared_ptr<Window>> m_windows;

};
