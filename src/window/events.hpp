#pragma once

#include "types.hpp"

enum EventType : int32_t {
    KEY
};

class Event {
public:
    Event(EventType t, WindowID id);

private:
    const EventType type;
    const WindowID  windowID;
};

class EventHandler {
public:
    virtual bool handle(const Event& e) = 0;
};

class KeyEvent : public Event {
public:
    KeyEvent(WindowID id, Key k, KeyAction a, KeyModifier m)
        : Event(EventType::KEY, id)
        , key(k)
        , action(a)
        , mod(m) {};
    ~KeyEvent() {};

    Key         key;
    KeyAction   action;
    KeyModifier mod;
};
