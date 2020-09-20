#include "glfw.hpp"

int GLFWWindowSystem::currId = 0;

std::unordered_map<WindowID, WindowHandle>                          GLFWWindowSystem::windows {};
std::unordered_map<WindowID, std::pair<EventType, EventHandlerPtr>> GLFWWindowSystem::windowRegisteredEvents {};
std::unordered_map<GLFWwindow*, WindowID>                           GLFWWindowSystem::glfwWindowIds {};
