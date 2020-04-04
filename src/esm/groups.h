#include "esm.h"

typedef struct __attribute__((packed)) {
    Group   base;
    Record* records;
} TopLevelGroup;
