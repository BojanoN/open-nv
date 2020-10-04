#pragma once

class AudioDecoder {

public:
    static int decodeFile(const char* path, const int sample_rate, double** data, int* size);

private:
};
