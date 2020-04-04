#include "subrecords.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Subrecord);

Subrecord* create_HEDR(FILE* esm_file)
{
    SAFE_MALLOC(HEDR, ret);

    fread(ret, sizeof(HEDR), 1, esm_file);
    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_CNAM(FILE* esm_file)
{
    SAFE_MALLOC(CNAM, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);

    ret->author = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);
    log_debug("Author: %s", ret->author);

    return (Subrecord*)ret;
}

void free_HEDR(Subrecord* record)
{
    HEDR* tmp = (HEDR*)record;
    free(tmp);
}

void free_CNAM(Subrecord* record)
{
    CNAM* tmp = (CNAM*)record;
    sdsfree(tmp->author);
    free(tmp);
}

void Subrecord_init_constructor_map()
{
    ADD_CONSTRUCTOR(Subrecord, "HEDR", create_HEDR);
    ADD_CONSTRUCTOR(Subrecord, "CNAM", create_CNAM);
}

void Subrecord_init_destructor_map()
{
    ADD_DESTRUCTOR(Subrecord, "HEDR", free_HEDR);
    ADD_DESTRUCTOR(Subrecord, "CNAM", free_CNAM);
}
