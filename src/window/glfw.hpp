#include "windowing.hpp"
#include <stdexcept>

#include <GLFW/glfw3.h>
#include <logc/log.h>

class GLFWWindowSystem;

class GLFWWindow : public Window {
public:
    friend class GLFWWindowSystem;
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

typedef std::shared_ptr<GLFWWindow> GLFWWindowHandle;

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
            GLFWWindowHandle h = std::make_shared<GLFWWindow>(new GLFWWindow(this->currId++, width, height, title));

            GLFWWindowSystem::windows[h->id]           = h;
            GLFWWindowSystem::glfwWindowIds[h->window] = h->id;

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
        if (GLFWWindowSystem::glfwWindowIds.count(window) == 0) {
            log_warn("Attempted to handle event on unregistered window");
            return;
        }

        WindowID                              id    = GLFWWindowSystem::glfwWindowIds[window];
        std::pair<EventType, EventHandlerPtr> entry = GLFWWindowSystem::windowRegisteredEvents[id];

        // Check if the handlers handles this event
        if (!(entry.first & EventType::KEY)) {
            return;
        }

        // TODO: convert GLFW key parameters to our parameters

        EventPtr e = std::make_shared<KeyEvent>(id, key, action, mods);

        entry.second->handle(e);
    }

    void registerGLFWCallbacks(GLFWWindow* window)
    {
        GLFWwindow* glfwWindow = window->window;

        glfwSetKeyCallback(glfwWindow, keyCallback);
    }

    static int                                                                 currId;
    static std::unordered_map<WindowID, WindowHandle>                          windows;
    static std::unordered_map<WindowID, std::pair<EventType, EventHandlerPtr>> windowRegisteredEvents;
    static std::unordered_map<GLFWwindow*, WindowID>                           glfwWindowIds;
};
