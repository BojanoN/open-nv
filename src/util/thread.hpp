#pragma once

#include <thread>
#include <unordered_map>

#include <pthread.h>

class ThreadManager {

public:
    template <class Function, class... Args>
    static std::thread::id startThread(Function&& f, Args&&... args)
    {
        std::thread     t(f, std::forward<Args>(args)...);
        std::thread::id threadID = t.get_id();

        mActiveThreadSet[threadID] = t.native_handle();

        t.detach();

        return threadID;
    }

    static void stopThread(std::thread::id threadID);

private:
    static void _nativeStopThread(pthread_t& id);

    static std::unordered_map<std::thread::id, pthread_t> mActiveThreadSet;
};
