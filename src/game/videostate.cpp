#include "videostate.hpp"
#include <file/configuration.hpp>
#include <logc/log.h>

namespace Game {

namespace fs = std::filesystem;
using namespace Types;

ErrorPair<std::shared_ptr<VideoState>> VideoState::create(const char* pathVideoFile)
{
    ErrorPair<std::shared_ptr<VideoState>> ret;

    fs::path filePath { pathVideoFile };

    if (!fs::exists(filePath)) {
        log_error("Video file %s does not exist!", pathVideoFile);
        ret.error = Err::FileNotFound;
        return ret;
    }

    VideoState* instance = new VideoState();
    if (instance == nullptr) {
        log_fatal("Unable to allocate memory for VideoState!");
        ret.error = Err::MemoryError;
        return ret;
    }

    VideoPlayer* player = new VideoPlayer(mDisplayConfiguration);
    if (player == nullptr) {
        log_fatal("Unable to allocate memory for VideoPlayer!");
        ret.error = Err::MemoryError;

        delete instance;
        return ret;
    }

    Error err = player->init();
    if (err.fail()) {
        log_fatal("Unable to initialize VideoPlayer!");
        ret.error = err;

        delete instance;
        delete player;
        return ret;
    }

    instance->mpathVideoFile = pathVideoFile;
    instance->mVideoPlayer   = std::unique_ptr<VideoPlayer>(player);

    ret.value = std::shared_ptr<VideoState>(instance);
    ret.error = Err::Success;

    return ret;
}

Error VideoState::enter()
{
    ssize_t timeNextUpdateUsec = mVideoPlayer->play(mpathVideoFile);
    if (timeNextUpdate < 0) {
        log_error("Unable to play video file %s", mpathVideoFile);
        return Err::InitError;
    }

    mTimer.start();
    mtimeNextUpdateUsec = mTimer.getElapsedMicroseconds() + timeNextUpdateUsec;

    return Err::Success;
}

bool VideoState::draw()
{
    ssize_t timeNextUpdateUsec = mVideoPlayer->update();
    if (timeNextUpdate < 0) {
        return false;
    }

    mtimeNextUpdateUsec = mTimer.getElapsedMicroseconds() + timeNextUpdateUsec;

    return true;
}

}
