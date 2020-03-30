#pragma once

#include <cstdint>

uint32_t fnv_hash(const char* buffer, uint32_t len);
uint32_t fnv_hash(uint8_t* buffer, uint32_t len);


