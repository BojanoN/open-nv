#pragma once

struct MediaFrame {
    uint8_t* data = nullptr;
    size_t   size = 0;
    double   pts  = 0.0;
};
