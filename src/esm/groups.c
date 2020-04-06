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
    MALLOC_WARN(TopLevelGroup, ret);

    uint32_t dataSize = 0;
    uint32_t groupEnd = 0;

    fread(&(ret->base), sizeof(Group), 1, esm_file);
    dataSize = ret->base.GroupSize - 24;
    groupEnd = ftell(esm_file) + dataSize;
    sds type = sdsnewlen(ret->base.Label, 4);

    RecordConstructor* func = GET_CONSTRUCTOR(Record, type);
    sdsfree(type);

    log_group(ret);

    if (func == NULL) {
        free(ret);
        log_warn("Record type %s not yet implemented.", type);
        return NULL;
    }

    uint32_t start;
    uint32_t end;

    while (ftell(esm_file) < groupEnd) {
        start          = ftell(esm_file) + sizeof(Record);
        Record* record = func(esm_file);
        log_info("Current file pointer location: 0x%06x", ftell(esm_file));
        end = ftell(esm_file);

        if (record) {
            arrput(ret->records, record);
        } else {
            groupfree((Group*)ret);
            return NULL;
        }

        if ((end - start) != record->DataSize) {
            log_fatal("Failed %.4s record parsing: DataSize mismatch.", record->Type);
            log_fatal("Read %u bytes, expected %u bytes.", (end - start), record->DataSize);
            log_fatal("Your esm file might be corrupted.");

            groupfree((Group*)ret);
            return NULL;
        }
    }

    return (Group*)ret;
}

void free_TopLevel(Group* group)
{
    TopLevelGroup*    tmp  = (TopLevelGroup*)group;
    sds               type = sdsnewlen(tmp->base.Label, 4);
    RecordDestructor* func = GET_DESTRUCTOR(Record, type);
    if (func == NULL) {
        sdsfree(type);
        log_fatal("Destructor for type %s not found", type);
        return;
    }
    sdsfree(type);
    for (uint32_t i = 0; i < arrlenu(tmp->records); i++) {
        if (tmp->records[i] != NULL)
            func(tmp->records[i]);
    }
    arrfree(tmp->records);
    free(tmp);
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
