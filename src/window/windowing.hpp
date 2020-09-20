#pragma once

#include "events.hpp"
#include "types.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class Window {
public:
    // No copying windows
    Window(const Window&) = delete;
    Window(Window&&)      = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    Window(WindowID i)
        : id(i) {};
    virtual ~Window() {};

    virtual bool shouldClose()                                    = 0;
    virtual void setTitle(std::string& title)                     = 0;
    virtual void setSize(unsigned int width, unsigned int height) = 0;
    virtual void setPosition(unsigned int x, unsigned int y)      = 0;

    const WindowID id;
};

class WindowSystem {

public:
    virtual ~WindowSystem() {};

    WindowSystem()
        : currId(0)
    {
    }

    virtual WindowHandle createWindow(const int width, const int height, const std::string& title);
    virtual void         registerCallback(WindowHandle window, const EventType type);

protected:
    int                                                                 currId;
    std::unordered_map<WindowID, WindowHandle>                          windows;
    std::unordered_map<WindowID, std::pair<EventType, EventHandlerPtr>> windowRegisteredEvents;
};
