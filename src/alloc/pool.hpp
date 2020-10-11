#pragma once

#include <memory>
#include <stdexcept>

namespace Allocator {

template <typename T>
struct Pool {
public:
    Pool(int noElements)
    {
        if (noElements < 0) {
            throw std::runtime_error("Invalid size");
        }

        arr = new T[noElements];

        if (arr == nullptr) {
            throw std::runtime_error("Failed to allocate memory");
        }

        //memset(arr, 0, noElements);

        freeIndex = 0;

        for (int i = 0; i < noElements; i++) {
            *reinterpret_cast<long*>(&arr[i]) = i + 1;
        }

        this->capacity = noElements;
        this->size     = 0;
        this->end      = &arr[noElements - 1];
    }

    ~Pool() { delete[] arr; }

    T* alloc()
    {
        if (size >= capacity) {
            return nullptr;
        }

        T*   ret      = &arr[freeIndex];
        long nextFree = *reinterpret_cast<long*>(ret);

        freeIndex = nextFree;
        this->size++;

        return ret;
    }

    void free(T* ptr)
    {
        long ptrIndex;

        if (ptr < this->arr || ptr > this->end) {
            throw std::runtime_error("Invalid pointer sent to free");
        }

        ptrIndex = reinterpret_cast<long>(ptr - arr);

        if (ptrIndex == freeIndex) {
            throw std::runtime_error("Double free");
        }

        *reinterpret_cast<long*>(ptr) = freeIndex;
        freeIndex                     = ptrIndex;

        this->size--;
    }

private:
    T* arr;
    T* end;

    unsigned int capacity;
    unsigned int size;
    long         freeIndex;
};
}
