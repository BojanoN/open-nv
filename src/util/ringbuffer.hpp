#pragma once

#include <atomic>
#include <cstring>
#include <memory>
#include <mutex>

#include <iostream>

class SPSCByteRingBuffer {
public:
    explicit SPSCByteRingBuffer(size_t size)
        : capacity(size)
        , mHead(0)
        , mTail(0)
    {
        this->buffer = std::make_unique<uint8_t[]>(size);
    }

    size_t write(uint8_t* arr, size_t size)
    {
        size_t written = 0;
        size_t tail { mTail.load(std::memory_order_acquire) };

        if (tail == (mHead.load(std::memory_order_acquire) - 1)) {
            return 0;
        }

        if (tail + size >= capacity) {
            size_t sizeUntilEnd = (capacity - tail);
            size -= sizeUntilEnd;
            memcpy(&buffer[tail], arr, sizeUntilEnd);

            tail = 0;
            written += sizeUntilEnd;
        }

        if (tail == (mHead.load(std::memory_order_acquire) - 1)) {
            return written;
        }

        size_t head { mHead.load(std::memory_order_acquire) };
        if (tail + size >= head) {
            size = head - tail - 1;
        }

        memcpy(&buffer[tail], arr + written, size);
        tail += size;
        written += size;

        mTail.store(tail, std::memory_order_release);

        return written;
    }

    size_t read(size_t readSize, uint8_t* dst)
    {
        size_t read = 0;
        size_t head { mHead.load(std::memory_order_acquire) };

        if (mTail.load(std::memory_order_acquire) == (head - 1)) {
            return 0;
        }

        if (head + readSize >= capacity) {
            size_t sizeUntilEnd = capacity - head;
            memcpy(dst, &buffer[head], sizeUntilEnd);
            head = 0;
            read += sizeUntilEnd;
        }

        if (mTail.load(std::memory_order_acquire) == (head - 1)) {
            mHead.store(head, std::memory_order_release);
            return read;
        }

        size_t tail { mTail.load(std::memory_order_acquire) };
        if (head + (readSize - read) > tail) {
            readSize -= (head - tail);
        }

        memcpy(dst, &buffer[head], readSize);

        mHead.store(head, std::memory_order_release);
        return read;
    }

    uint8_t* getPtr()
    {
        return buffer.get();
    }

private:
    const size_t capacity;

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

    T* getPtr()
    {
        return buffer.get();
    }

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
        : capacity(size)
        , mCurrentSize(0)
        , mHead(0)
        , mTail(0)
    {
        this->buffer = std::make_unique<T[]>(size);
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

    T* getPtr()
    {
        return buffer.get();
    }

    void reset()
    {
        mHead = mTail = 0;
        mCurrentSize  = 0;
    }

private:
    const size_t capacity;

    std::atomic<size_t> mCurrentSize;
    std::atomic<size_t> mHead;
    std::atomic<size_t> mTail;

    std::unique_ptr<T[]> buffer;
};
