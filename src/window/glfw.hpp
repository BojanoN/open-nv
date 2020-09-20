#include "windowing.hpp"
#include <stdexcept>

#include <GLFW/glfw3.h>
#include <logc/log.h>

class GLFWWindow : public Window {
public:
    GLFWWindow(WindowID id, unsigned int width, unsigned int height, const std::string& title)
        : Window(id)
    {
        this->window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (window == nullptr) {
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(this->window);
    };

    virtual ~GLFWWindow()
    {
        glfwDestroyWindow(this->window);
    };

    virtual bool shouldClose() { return glfwWindowShouldClose(this->window); };
    virtual void setTitle(std::string& title) { glfwSetWindowTitle(this->window, title.c_str()); };
    virtual void setSize(unsigned int width, unsigned int height) { glfwSetWindowSize(this->window, width, height); };
    virtual void setPosition(unsigned int x, unsigned int y) { glfwSetWindowPos(this->window, x, y); };

private:
    GLFWwindow* window;
};

inline void glfwErrorHandler(int err, const char* err_str)
{
    log_fatal("GLFW error: %s", err_str);
}

class GLFWWindowSystem : public WindowSystem {
public:
    GLFWWindowSystem()
    {
        glfwSetErrorCallback(glfwErrorHandler);

        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    virtual ~GLFWWindowSystem()
    {
        glfwTerminate();
    }

    virtual WindowHandle createWindow(const int width, const int height, const std::string& title)
    {
        try {
            WindowHandle h = std::make_shared<Window>(new GLFWWindow(this->currId++, width, height, title));

            this->windows[h->id] = h;

            return h;
        } catch (std::runtime_error& e) {
            //TODO: log
        }

        return nullptr;
    }

    virtual void registerCallback(WindowHandle window, const EventType type, EventHandlerPtr handler)
    {
        std::pair<EventType, EventHandlerPtr> entry { type, handler };
        this->windowRegisteredEvents[window->id] = entry;
    }

private:
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        // TODO: call handler if event type is registered
    }

    static void mousePositionCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        // TODO: call handler if event type is registered
    }

    void registerGLFWCallbacks(WindowHandle window)
    {
        glfwSetKeyCallback(window->glfwWindow, keyCallback);
        glfwSetCursorPosCallback(window->glfwWindow, mousePositionCallback);
    }
};
