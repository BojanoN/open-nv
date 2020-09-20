#pragma once

#include "types.hpp"
#include <string>

class Window {
public:
    // No copying windows
    Window(const Window&) = delete;
    Window(Window&&)      = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    Window() {};
    virtual ~Window() {};

    virtual bool shouldClose()                                    = 0;
    virtual void setTitle(std::string& title)                     = 0;
    virtual void getSize()                                        = 0;
    virtual void setSize(unsigned int width, unsigned int height) = 0;
};

class WindowSystem {

public:
    virtual ~WindowSystem() {};

    virtual bool         init() = 0;
    virtual WindowHandle createWindow(const string& title, const int width, const int height);
    virtual registerCallback(WindowHandle window);
};
