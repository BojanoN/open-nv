#pragma once

// NB: maximum allowed number of distinct resource types is 4096
enum HandleResourceType {
    VertexBuffer,
    VertexArray,
    ElementBuffer,

};

struct ResourceHandle {
    uint32_t index;
    unsigned generation : 20;
    unsigned resourceType : 12;

    inline bool valid()
    {
        return *reinterpret_cast<uint64_t*>(&index) != 0;
    }

    inline bool operator==(ResourceHandle r) const
    {
        return *reinterpret_cast<const uint64_t*>(&this->index) == *reinterpret_cast<const uint64_t*>(&r);
    }
};
