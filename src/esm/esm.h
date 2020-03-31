#pragma once
#include "logc/log.h"
#include "sds/sds.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
} Esm;

Esm* esmnew(const sds path);
void esmfree(Esm* esm);

typedef struct {
    uint8_t  Type[4];
    uint32_t DataSize;
    uint32_t Flags;
    uint32_t ID;
    uint32_t CreationKitRevision;
    uint16_t FormVersion;

    /* ignoring this for now */
    uint16_t Unknown;
    // TODO: container of subrecords
} Record;

Record* recordnew(FILE* f);
void    recordfree(Record* record);
