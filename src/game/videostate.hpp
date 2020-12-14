#pragma once

#include <engine/video.hpp>
#include <game/state.hpp>
#include <types/errorpair.hpp>
#include <util/timer.hpp>

#include <memory>
#include <vector>

class Texture2D;
class Shader;

namespace File {
class Configuration;
}

namespace Game {

using namespace Types;

class VideoState : public DrawableGameState {
public:
    virtual Error enter();
    virtual void  exit();
    virtual void  suspend();
    virtual void  resume();

    virtual DrawableGameStateStatus draw();

    static ErrorPair<std::shared_ptr<GameState>> create(const char* filePath);

    virtual ~VideoState();

private:
    VideoState(const char* path)
        : mpathVideoFile(path)
        , mTimeNextUpdateUsec(0) {};

    std::unique_ptr<VideoPlayer> mVideoPlayer;
    const std::string            mpathVideoFile;

    size_t mTimeNextUpdateUsec;
    Timer  mTimer;
};
}
