#include "subrecords.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Subrecord);

Subrecord* create_HEDR(FILE* esm_file)
{
    MALLOC_WARN(HEDR, ret);

    fread(ret, sizeof(HEDR), 1, esm_file);
    log_subrecord(ret);

    return (Subrecord*)ret;
}

Subrecord* create_CNAM(FILE* esm_file)
{
    MALLOC_WARN(CNAM, ret);

    fread(ret, sizeof(Subrecord), 1, esm_file);

    char* tmp = (char*)malloc(ret->base.DataSize);
    fread(tmp, ret->base.DataSize, 1, esm_file);

    ret->author = sdsnewlen(tmp, ret->base.DataSize);
    free(tmp);

    log_subrecord(ret);
    log_debug("Author: %s", ret->author);

    return (Subrecord*)ret;
}

Subrecord* create_OBND(FILE* esm_file)
{
    MALLOC_WARN(OBNDSubrecord, subrecord);

    fread(subrecord, sizeof(OBNDSubrecord), 1, esm_file);
    log_debug("Object bounds:");
    log_debug("X 1: %d", subrecord->x1);
    log_debug("Y 1: %d", subrecord->y1);
    log_debug("Z 1: %d", subrecord->z1);
    log_debug("X 2: %d", subrecord->x2);
    log_debug("Y 2: %d", subrecord->y2);
    log_debug("Z 2: %d", subrecord->z2);

    return (Subrecord*)subrecord;
}

Subrecord* create_DODT(FILE* esm_file)
{
    MALLOC_WARN(DODTSubrecord, subrecord);

    fread(subrecord, sizeof(DODTSubrecord), 1, esm_file);
    log_debug("Decal data:");
    log_debug("Min width: %f", subrecord->minWidth);
    log_debug("Max width: %f", subrecord->maxWidth);
    log_debug("Min height: %f", subrecord->minHeight);
    log_debug("Max height: %f", subrecord->maxHeight);
    log_debug("Depth: %f", subrecord->depth);
    log_debug("Shininess: %f", subrecord->shininess);
    log_debug("Parallax scale: %f", subrecord->parallaxScale);
    log_debug("Parallax passes: %d", subrecord->parallaxPasses);
    log_debug("Flags: 0x%02x", subrecord->flags);
    log_debug("Unused bytes: 0x%02x 0x%02x", subrecord->unused[0], subrecord->unused[1]);
    log_debug("Color: 0x%02x 0x%02x 0x%02x 0x%02x", subrecord->color[0], subrecord->color[1], subrecord->color[2], subrecord->color[3]);

    return (Subrecord*)subrecord;
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
    ADD_CONSTRUCTOR(Subrecord, "OBND", create_OBND);
    ADD_CONSTRUCTOR(Subrecord, "DODT", create_DODT);
}

void Subrecord_init_destructor_map()
{
    ADD_DESTRUCTOR(Subrecord, "HEDR", free_HEDR);
    ADD_DESTRUCTOR(Subrecord, "CNAM", free_CNAM);
}
