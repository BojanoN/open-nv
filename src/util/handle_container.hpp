#pragma once

#include <cstddef>
#include <type_traits>

#ifdef DEBUG
#include <cassert>
#endif

#include <resources/handle_resource.hpp>

template <typename T, HandleResourceType type, uint32_t N = 4096>
struct HandleContainer {

    HandleContainer()
        : mFreelistHead(0)
        , mFreelistTail(N - 1)
        , mNoAllocatedResources(0)
    {
        for (uint32_t i = 0; i < N; i++) {
            InternalIndex& currentIndex = mIndices[i];

            currentIndex.nextIndex           = i + 1;
            currentIndex.resourceArrayIndex  = HandleContainer::Sentinel;
            currentIndex.handle.index        = i;
            currentIndex.handle.generation   = 1;
            currentIndex.handle.resourceType = type;
        }
    };
    ~HandleContainer() {};

    inline T& get(ResourceHandle handle)
    {
#ifdef DEBUG
        assert(isValid(handle));
#endif

        return mResource[mIndices[handle.index].resourceArrayIndex].resource;
    }

    inline bool isValid(const ResourceHandle handle)
    {
#ifdef DEBUG
        assert(handle < N);
#endif

        InternalIndex& index = mIndices[handle.index];
        return index.resourceArrayIndex != HandleContainer::Sentinel
            && handle == index.handle;
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
        handleIndex.nextIndex = mFreelistTail;
        mFreelistTail         = handleIndex.handle.index;

        mNoAllocatedResources--;
    }

    inline ResourceHandle create()
    {
        if (mNoAllocatedResources == N) {
            uint64_t ret = 0;
            return *reinterpret_cast<ResourceHandle*>(&ret);
        }

        InternalIndex& index     = mIndices[mFreelistHead];
        index.resourceArrayIndex = mNoAllocatedResources;

        mFreelistHead = index.nextIndex;
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
