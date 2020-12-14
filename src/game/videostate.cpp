#include "videostate.hpp"
#include <file/configuration.hpp>
#include <logc/log.h>

namespace Game {

namespace fs = std::filesystem;
using namespace Types;

VideoState::~VideoState()
{
    this->mVideoPlayer->close();
}

ErrorPair<std::shared_ptr<GameState>> VideoState::create(const char* pathVideoFile)
{
    ErrorPair<std::shared_ptr<GameState>> ret;

    fs::path filePath { pathVideoFile };

    if (!fs::exists(filePath)) {
        log_error("Video file %s does not exist!", pathVideoFile);
        ret.error = Err::FileNotFound;
        return ret;
    }

    VideoState* instance = new VideoState(pathVideoFile);
    if (instance == nullptr) {
        log_fatal("Unable to allocate memory for VideoState!");
        ret.error = Err::MemoryError;
        return ret;
    }

    VideoPlayer* player = new VideoPlayer();
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

    instance->mVideoPlayer = std::unique_ptr<VideoPlayer>(player);

    ret.value = std::shared_ptr<GameState>(instance);
    ret.error = Err::Success;

    return ret;
}

Error VideoState::enter()
{
    ssize_t timeNextUpdateUsec = mVideoPlayer->play(mpathVideoFile.c_str());
    if (timeNextUpdateUsec < 0) {
        log_error("Unable to play video file %s", mpathVideoFile.c_str());
        return Err::InitError;
    }

    mTimer.start();
    mTimeNextUpdateUsec = mTimer.getElapsedMicroseconds() + timeNextUpdateUsec;

    return Err::Success;
}

void VideoState::exit()
{
}
void VideoState::resume()
{
}
void VideoState::suspend()
{
}

DrawableGameStateStatus VideoState::draw()
{
    if (mTimer.getElapsedMicroseconds() < mTimeNextUpdateUsec) {
        return DrawableGameStateStatus::Continue;
    }

    ssize_t timeNextUpdateUsec = mVideoPlayer->update();
    if (timeNextUpdateUsec < 0) {
        return DrawableGameStateStatus::EndOfOperation;
    }

    mTimeNextUpdateUsec = mTimer.getElapsedMicroseconds() + timeNextUpdateUsec;

    return DrawableGameStateStatus::ScreenContentChanged;
}

}
