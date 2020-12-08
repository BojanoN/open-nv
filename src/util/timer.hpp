#pragma once
#include <chrono>

class Timer {
public:
    Timer() {};
    ~Timer() {};

    void start()
    {
        mStartTime = std::chrono::steady_clock::now();
        running    = true;
    }
    void stop() { running = false; }

    size_t getElapsedSeconds()
    {
        return running ? static_cast<size_t>((std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mStartTime).count())) / 1000. : 0;
    }

    size_t getElapsedMilliseconds()
    {
        return running ? static_cast<size_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - mStartTime).count()) : 0;
    }

    size_t getElapsedMicroseconds()
    {
        return running ? static_cast<size_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - mStartTime).count()) : 0;
    }

private:
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
    bool                                               running = false;
};
