#pragma once

#include <cstdint>
#include <memory>

class Window;

typedef uint32_t                WindowID;
typedef std::shared_ptr<Window> WindowHandle;

class EventHandler;
typedef std::shared_ptr<EventHandler> EventHandlerPtr;

class Event;
typedef std::shared_ptr<Event> EventPtr;

enum class Key {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D
};

enum class KeyAction {
    PRESSED,
    RELEASED
};

enum KeyModifier : int {
    MOD_NONE  = 0x0,
    MOD_CTRL  = 1 << 1,
    MOD_ALT   = 1 << 2,
    MOD_SHIFT = 1 << 3,
    MOD_SUPER = 1 << 4
};
