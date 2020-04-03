#include "subrecords.h"

DECLARE_FUNCTION_MAPS(Subrecord);

Subrecord* create_HEDR(FILE* esm_file)
{
    HEDR* ret = (HEDR*)malloc(sizeof(HEDR));
    if (ret == NULL) {
        log_fatal("Unable to allocate memory for HEDR!");
        return NULL;
    }

    fread(ret, sizeof(HEDR), 1, esm_file);
    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_CNAM(FILE* esm_file)
{
    CNAM* ret = (CNAM*)malloc(sizeof(CNAM));
    if (ret == NULL) {
        log_fatal("Unable to allocate memory for CNAM!");
        return NULL;
    }

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);

    ret->author = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);

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
    shput(Subrecord_constructor_map, "HEDR", create_HEDR);
    shput(Subrecord_constructor_map, "CNAM", create_CNAM);
}

void Subrecord_init_destructor_map()
{
    shput(Subrecord_destructor_map, "HEDR", free_HEDR);
    shput(Subrecord_destructor_map, "CNAM", free_CNAM);
}
