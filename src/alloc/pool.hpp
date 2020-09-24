#pragma once

#include <memory>
#include <stdexcept>

template <typename T>
struct Pool {
public:
    Pool(int noElements)
    {
        if (noElements < 0) {
            throw std::runtime_error("Invalid size");
        }

        arr       = std::make_unique<T*>(new T[noElements]);
        freeIndex = 0;

        for (int i = 0; i < noElements; i++) {
            *reinterpret_cast<int*>(arr[i]) = i + 1;
        }

        // sentinel
        *reinterpret_cast<int*>(arr[noElements - 1]) = -1;
    }

    T* allocate()
    {
        T*  ret      = &arr[freeIndex];
        int nextFree = *reinterpret_cast<int*>(ret);

        freeIndex = nextFree;

        return ret;
    }

    void free(T* ptr)
    {
        int ptrIndex = reinterpret_cast<unsigned int>(ptr - &arr[0]) / sizeof(T);

        *reinterpret_cast<int*>(ptr) = freeIndex;

        freeIndex = ptrIndex;
    }

private:
    std::unique_ptr<T*> arr;
    int                 freeIndex;
};
