#include "groups.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Group);

FETCH_CONSTRUCTOR_MAP(Record);
FETCH_DESTRUCTOR_MAP(Record);

Group* init_TopLevel(FILE* esm_file)
{
    SAFE_MALLOC(TopLevelGroup, ret);

    return (Group*)ret;
}

void free_TopLevel(Group* group)
{
}

void Group_init_constructor_map()
{
}

void Group_init_destructor_map()
{
}
