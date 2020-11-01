#pragma once

#include <atomic>
#include <cstring>
#include <memory>
#include <mutex>

#include <logc/log.h>

class SPSCByteRingBuffer {
public:
    explicit SPSCByteRingBuffer(size_t size)
        : capacity(size)
        , mCurrentSize(0)
        , mHead(0)
        , mTail(0)
    {
        this->buffer = std::make_unique<uint8_t[]>(size);
    }

    size_t write(uint8_t* arr, size_t size)
    {
        size_t written = 0;

        if (full()) {
            return 0;
        }

        if (mCurrentSize.load(std::memory_order_relaxed) + size >= capacity) {
            size -= capacity - mCurrentSize.load(std::memory_order_relaxed);
        }

        size_t tail { mTail.load(std::memory_order_relaxed) };

        // Wrap tail if needed
        if (tail + size >= capacity) {
            size_t sizeUntilWrap = (capacity - tail);
            size -= sizeUntilWrap;
            memcpy(&buffer[tail], arr, sizeUntilWrap);

            mTail.store(0, std::memory_order_acq_rel);
            mCurrentSize.fetch_add(sizeUntilWrap, std::memory_order_acq_rel);
            written += sizeUntilWrap;
        }

        memcpy(&buffer[mTail.load(std::memory_order_acquire)], arr + written, size);
        mTail.fetch_add(size, std::memory_order_acq_rel);
        mCurrentSize.fetch_add(size, std::memory_order_acq_rel);
        written += size;

        return written;
    }

    size_t read(size_t readSize, uint8_t* dst)
    {
        size_t read = 0;

        if (empty()) {
            return 0;
        }

        if (readSize > mCurrentSize.load(std::memory_order_relaxed)) {
            readSize = mCurrentSize.load(std::memory_order_relaxed);
        }

        size_t head { mHead.load(std::memory_order_acquire) };
        if (head + readSize >= capacity) {
            size_t sizeUntilWrap = capacity - head;
            readSize -= sizeUntilWrap;
            memcpy(dst, &buffer[head], sizeUntilWrap);
            mHead.store(0, std::memory_order_acq_rel);
            read += sizeUntilWrap;
        }

        memcpy(dst, &buffer[mHead.load(std::memory_order_acquire)], readSize);
        mHead.store(head, std::memory_order_release);
        read += readSize;

        return read;
    }

    bool full()
    {
        return mCurrentSize.load(std::memory_order_relaxed) >= capacity;
    }

    bool empty()
    {
        return mCurrentSize.load(std::memory_order_relaxed) <= 0;
    }

    const size_t capacity;

private:
    std::atomic<size_t> mCurrentSize;
    std::atomic<size_t> mHead;
    std::atomic<size_t> mTail;

    std::unique_ptr<uint8_t[]> buffer;
};

template <class T>
class RingBuffer {
public:
    explicit RingBuffer(size_t size)
        : capacity(size)
        , mCurrentSize(0)
        , mHead(0)
        , mTail(0)
    {
        this->buffer = std::make_unique<T[]>(size);
    }

    int put(const T& elem)
    {
        std::unique_lock<std::mutex> bufferLock(mMutex);

        if (capacity == mCurrentSize) {
            return -1;
        }

        buffer[mTail] = elem;
        mTail         = (mTail + 1) % capacity;
        mCurrentSize++;

        return 0;
    }

    bool get(T& dst)
    {
        std::unique_lock<std::mutex> bufferLock(mMutex);

        if (mCurrentSize == 0) {
            return false;
        }

        dst   = buffer[mHead];
        mHead = (mHead + 1) % capacity;
        mCurrentSize--;

        return true;
    }

    bool empty() { return mCurrentSize == 0; }
    bool full() { return mCurrentSize == capacity; }

    void reset()
    {
        mHead = mTail = 0;
        mCurrentSize  = 0;
    }

private:
    const size_t capacity;

    size_t mCurrentSize;
    size_t mHead;
    size_t mTail;

    std::mutex mMutex;

    std::unique_ptr<T[]> buffer;
};

template <class T>
class SPSCRingBuffer {
public:
    explicit SPSCRingBuffer(size_t size)
        : mCurrentSize(0)
        , capacity(size)
        , mHead(0)
        , mTail(0)
    {
        this->buffer = std::make_unique<T[]>(size);
    }

    T* peek()
    {
        if (empty()) {
            return nullptr;
        }

        return &buffer[mHead.load(std::memory_order_acq_rel)];
    }

    T* peekTail()
    {
        if (empty()) {
            return nullptr;
        }

        return &buffer[mTail.load(std::memory_order_acq_rel)];
    }

    void incTail()
    {
        size_t tail = mTail.load(std::memory_order_acquire);
        mTail.store((tail + 1) % capacity, std::memory_order_release);
    }

    void pop()
    {
        if (empty()) {
            return;
        }

        size_t head = mHead.load(std::memory_order_acquire);
        mHead.store((head + 1) % capacity, std::memory_order_release);
    }

    int put(T& elem)
    {

        if (full()) {
            return -1;
        }
        size_t tail  = mTail.load(std::memory_order_acquire);
        buffer[tail] = elem;
        mTail.store((tail + 1) % capacity, std::memory_order_release);
        mCurrentSize.fetch_add(1, std::memory_order_acq_rel);

        return 0;
    }

    bool get(T& dst)
    {

        if (empty()) {
            return false;
        }

        size_t head = mHead.load(std::memory_order_acquire);
        dst         = buffer[head];
        mHead.store((head + 1) % capacity, std::memory_order_release);
        mCurrentSize.fetch_sub(1, std::memory_order_acq_rel);

        return true;
    }

    bool empty() { return mCurrentSize.load(std::memory_order_relaxed) == 0; }
    bool full() { return mCurrentSize.load(std::memory_order_relaxed) == capacity; }

    void reset()
    {
        mHead = mTail = 0;
        mCurrentSize  = 0;
    }
    std::atomic<size_t> mCurrentSize;
    const size_t        capacity;

private:
    //    std::atomic<size_t> mCurrentSize;
    std::atomic<size_t> mHead;
    std::atomic<size_t> mTail;

    std::unique_ptr<T[]> buffer;
};
