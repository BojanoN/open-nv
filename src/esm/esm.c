#include "esm.h"
Esm* esmnew(const sds path)
{
    FILE* esm_file;
    esm_file = fopen(path, "rb");
    if (esm_file == NULL) {
        log_fatal("file %s does not exist", path);
        exit(1);
    }

    // ...

    fclose(esm_file);
}
void esmfree(Esm* esm)
{
}

Record*
recordnew(FILE* f)
{
    Record* rec = malloc(sizeof(Record));

    fread(&(rec->Type), sizeof(rec->Type), 1, f);
    fread(&(rec->DataSize), sizeof(rec->DataSize), 1, f);
    fread(&(rec->Flags), sizeof(rec->Flags), 1, f);
    fread(&(rec->ID), sizeof(rec->ID), 1, f);
    fread(&(rec->CreationKitRevision), sizeof(rec->CreationKitRevision), 1, f);
    fread(&(rec->FormVersion), sizeof(rec->FormVersion), 1, f);
    fread(&(rec->Unknown), sizeof(rec->Unknown), 1, f);

    // dummy data read
    fseek(f, (long)(rec->DataSize), SEEK_CUR);

    // TODO: subrecord  loading and container store

    return rec;
}
void recordfree(Record* record)
{
    free(record);
    // TODO: free any internal containers
}