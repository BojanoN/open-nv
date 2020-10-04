#include "audio.hpp"

#include <cstddef>
#include <logc/log.h>

int AudioEngine::init()
{
    const char* defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    this->currentDevice           = alcOpenDevice(defaultDeviceName);

    if (!this->currentDevice) {
        log_error("Unable to open default device: %s", defaultDeviceName);
        return -1;
    }

    this->currentDeviceContext = alcCreateContext(this->currentDevice, NULL);
    if (!alcMakeContextCurrent(this->currentDeviceContext)) {
        log_error("Unable to make context for default device");
        return -1;
    }

    this->initialized = true;

    return 0;
};

int AudioEngine::openFile(std::string& path)
{
    alGenSources(1, &this->source);

    if (alGetError() != AL_NO_ERROR) {
        log_error("Unable to generate audio source");
        return -1;
    }

    return 0;
}
