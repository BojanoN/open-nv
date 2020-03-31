#pragma once
#include <stdint.h>

uint32_t fnv_hash(const uint8_t* buffer, const uint32_t len);
