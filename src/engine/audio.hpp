#pragma once

extern "C" {
#include <AL/al.h>
#include <AL/alc.h>
}

class AudioEngine {
public:
    AudioEngine()
        : currentDevice(nullptr)
        , initialized(false) {};

    int init();
    int openFile(std::string& path);

private:
    ALint       source;
    ALCdevice*  currentDevice;
    ALCcontext* currentDeviceContext;
    bool        initialized;
};
