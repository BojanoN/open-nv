#include "audio.hpp"

#include <logc/log.h>

#include <AL/al.h>
#include <chrono>
#include <thread>

int main(void)
{

    if (AudioEngine::init()) {
        return -1;
    }

    StreamPlayer* player = AudioEngine::playFile("test.mp3");

    if (player == nullptr) {
        return -2;
    }

    if (player->start()) {
        AudioEngine::freeStreamPlayer(player);
        log_info("Finished playback early");
        return 0;
    }

    log_info("Started playback");

    while (!player->update())
        std::this_thread::sleep_for(std::chrono::nanoseconds(10000000));

    log_info("Finished playback");
    AudioEngine::freeStreamPlayer(player);
    return 0;
}
