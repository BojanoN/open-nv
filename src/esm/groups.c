#include "groups.h"
#include "util/memutils.h"

DECLARE_FUNCTION_MAPS(Group);

FETCH_CONSTRUCTOR_MAP(Record);
FETCH_DESTRUCTOR_MAP(Record);

const char* groupTypeKeys[] = {
    "TopLevel",
    "WorldChildren",
    "InteriorCellBlock",
    "InteriorCellSubBlock",
    "ExteriorCellBlock",
    "ExteriorCellSubBlock",
    "CellChildren",
    "TopicChildren",
    "CellPersistentChildren",
    "CellTemporaryChildren",
    "CellVisibleDistantChildren"
};

Group* init_TopLevel(FILE* esm_file)
{
    SAFE_MALLOC(TopLevelGroup, ret);

    uint32_t read      = 0;
    uint32_t totalRead = 0;

    read = fread(&(ret->base), sizeof(Group), 1, esm_file);

    log_group(ret);

    return (Group*)ret;
}

void free_TopLevel(Group* group)
{
    TopLevelGroup* tmp = (TopLevelGroup*)group;
    free(group);
}

void Group_init_constructor_map()
{
    ADD_CONSTRUCTOR(Group, groupTypeKeys[0], init_TopLevel);
}

void Group_init_destructor_map()
{
    ADD_DESTRUCTOR(Group, groupTypeKeys[0], free_TopLevel);
}

Group* groupnew(FILE* f)
{
    Group*   ret       = NULL;
    uint32_t groupType = 0;

    // Read group type
    fseek(f, 12, SEEK_CUR);
    fread(&groupType, sizeof(uint32_t), 1, f);
    fseek(f, -16, SEEK_CUR);

    if (groupType > 10) {
        log_error("Invalid group type %u", groupType);
        return NULL;
    }

    const char*       type = groupTypeKeys[groupType];
    GroupConstructor* func = GET_CONSTRUCTOR(Group, type);

    if (func == NULL) {
        log_warn("Group type %s not yet implemented.", type);
        return NULL;
    }

    ret = func(f);
    return ret;
}
void groupfree(Group* group)
{
    const char*      type = groupTypeKeys[group->GroupType];
    GroupDestructor* func = GET_DESTRUCTOR(Group, type);

    func(group);
}
