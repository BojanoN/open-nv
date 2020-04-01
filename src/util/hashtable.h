#pragma once
#include "sds/sds.h"
#include <malloc.h>
#include <stdint.h>

typedef struct {
    sds   key;
    void* value;
} table_entry;

typedef struct {
    uint32_t      size;
    uint32_t      count;
    table_entry** items;
} hash_table;

hash_table* new_hashtable();
void        free_hashtable(hash_table* table);

void* hashtable_find(hash_table* table, sds key);
void  hashtable_delete(hash_table* table, sds key);
void  hashtable_insert(hash_table* table, sds key, void* value);
void  hashtable_update(hash_table* table, sds key, void* value);
