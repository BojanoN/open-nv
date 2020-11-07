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

    StreamPlayer* player = AudioEngine::playFile("piano_D.mp3");
    // StreamPlayer* player1 = AudioEngine::playFile("piano_E.mp3");
    // StreamPlayer* player2 = AudioEngine::playFile("piano_G.mp3");
    //StreamPlayer* player3 = AudioEngine::playFile("piano_B.mp3");

    /*
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
        std::this_thread::sleep_for(std::chrono::milliseconds(4));

    log_info("Finished playback");
    */

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    } while (true);

    return 0;
}
