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

    virtual bool draw();

    static ErrorPair<std::shared_ptr<VideoState>> create(const char* filePath);

    virtual ~VideoState() {};

private:
    VideoState(File::Configuration& displayConfiguration)
        : mDisplayConfiguration(displayConfiguration)
        , timeNextUpdate(0) {};

    File::Configuration&         mDisplayConfiguration;
    std::unique_ptr<VideoPlayer> mVideoPlayer;
    const char*                  mpathVideoFile;

    size_t mtimeNextUpdateUsec;
    Timer  mTimer;
};
}
