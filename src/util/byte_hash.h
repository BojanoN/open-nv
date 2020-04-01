#pragma once
#include "sds/sds.h"
#include <stdint.h>

uint32_t fnv1a_hash(const sds buffer, const uint32_t len);
uint32_t fnv_hash(const sds buffer, const uint32_t len);
