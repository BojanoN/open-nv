#include "thread.hpp"

void ThreadManager::stopThread(std::thread::id threadID)
{
    auto found = mActiveThreadSet.find(threadID);
    if (found != mActiveThreadSet.end()) {
        _nativeStopThread(found->second);
        mActiveThreadSet.erase(found);
    }
}

void ThreadManager::_nativeStopThread(pthread_t& id)
{
    pthread_cancel(id);
}

std::unordered_map<std::thread::id, pthread_t> ThreadManager::mActiveThreadSet {};
