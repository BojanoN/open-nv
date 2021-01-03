#pragma once

#include <cstddef>
#include <type_traits>

#ifdef DEBUG
#include <cassert>
#endif

#include <resources/resource.hpp>

struct ResourceHandle {
    uint32_t index;
    unsigned generation : 20;
    unsigned resourceType : 12;

    inline bool valid()
    {
        return *reintepret_cast<uint64_t*>(&index) != 0;
    }

    inline bool operator==(ResourceHandle r)
    {
        *reintepret_cast<uint64_t*>(&this->index) == *reintepret_cast<uint64_t*>(&r);
    }
};

template <typename T, uint32_t N = 4096>
struct HandleContainer {

    static_assert(std::is_base_of<Resource, T>::value, "Handle containers can only be instantatiated for Resource types!");

    HandleContainer()
        : mFreelistHead(0)
        , mFreelistTail(N - 1)
        , mNoAllocatedResources(0)
    {
        for (uint32_t i = 0; i < N; i++) {
            mIndices[i].nextIndex          = i + 1;
            mIndices[i].resourceArrayIndex = HandleContainer::Sentinel;
            mIndices[i].handle.index       = 0;
            mIndices[i].handle.generation  = 1;
        }
    };
    ~HandleContainer() {};

    inline T& get(ResourceHandle handle)
    {
#ifdef DEBUG
        assert(isValid(handle));
#endif

        return mResource[mIndices[handle.index].resourceArrayIndex];
    }

    inline bool isValid(ResourceHandle handle)
    {
#ifdef DEBUG
        assert(handle < N);
#endif

        InternalIndex& index = mIndices[handle.index];
        return index.resourceArrayIndex != HandleContainer::Sentinel
            && handle < N
            && handle == index.handle
            && handle.resourceType == index.handle.resourceType;
    }

    inline void release(ResourceHandle handle)
    {
#ifdef DEBUG
        assert(isValid(handle) && mNoAllocatedResources > 0);
#endif

        InternalIndex& handleIndex  = mIndices[handle.index];
        Resource&      lastResource = mResource[mNoAllocatedResources - 1];

        // Place last object in array to the now freed object and update the corresponding index
        mIndices[lastResource.internalIndexOffset].resourceArrayIndex = handleIndex.resourceArrayIndex;
        mResource[handleIndex.resourceArrayIndex]                     = lastResource;

        // Release index
        handleIndex.resourceArrayIndex = HandleContainer::Sentinel;
        handleIndex.handle.generation++;
        handleIndex.nextIndex = mFreelistHead;
        mFreelistHead         = handleIndex.handle.index;

        mNoAllocatedResources--;
    }

    inline ResourceHandle create()
    {
        if (mNoAllocatedResources == N) {
            return reinterpret_cast<ResourceHandle>(0);
        }

        InternalIndex& index     = mIndices[mFreelistTail];
        index.resourceArrayIndex = mNoAllocatedResources;

        mFreelistTail = index.nextIndex;
        mNoAllocatedResources++;

        return index.handle;
    }

private:
    struct InternalIndex {
        ResourceHandle handle;
        uint32_t       resourceArrayIndex;
        uint32_t       nextIndex;
    };

    struct Resource {
        uint32_t internalIndexOffset;
        T        resource;
    };

    Resource      mResource[N] = { 0 };
    InternalIndex mIndices[N]  = { 0 };

    uint32_t mFreelistHead;
    uint32_t mFreelistTail;

    uint32_t mNoAllocatedResources;

    static constexpr uint32_t Sentinel = std::numeric_limits<uint32_t>::max();
};
