#include "hashtable.h"
#include "byte_hash.h"
#include "logc/log.h"
#include <assert.h>
#include <string.h>

static const uint32_t TABLE_DEFAULT_SIZE = 50;
static const uint32_t TABLE_MAX_SIZE     = 2147483647; // signed int max
static const float    MAX_FILL           = 0.7;
static const float    MIN_FILL           = 0.1;

typedef struct {
    table_entry* entry;
    uint32_t     offset;
} entry_info;

static table_entry EMPTY
    = { NULL, NULL };

static uint32_t hashtable_index(uint32_t table_size, uint32_t no_collisions, sds key)
{
    uint32_t hval1 = fnv1a_hash(key, sdslen(key));
    uint32_t hval2 = fnv_hash(key, sdslen(key));
    uint32_t index = (hval1 + no_collisions * (hval2)) % table_size;

    return index;
}

static entry_info hashtable_exists(hash_table* table, sds key)
{
    uint32_t     index;
    uint32_t     collisions = 0;
    table_entry* item;
    entry_info   ret = { NULL, 0 };

    index = hashtable_index(table->size, collisions, key);
    item  = table->items[index];

    while (item) {
        if (strcmp(key, item->key)) {
            ret.offset = index;
            ret.entry  = item;
        }
        collisions++;
        index = hashtable_index(table->size, collisions, key);
        item  = table->items[index];
    }

    return ret;
}

static inline hash_table* new_hashtable_size(uint32_t size)
{

    assert(size < TABLE_MAX_SIZE);

    hash_table* ret = (hash_table*)malloc(sizeof(hash_table));

    if (ret == NULL) {
        log_fatal("Cannot allocate memory for hashtable!");
        return NULL;
    }

    ret->size  = TABLE_DEFAULT_SIZE;
    ret->count = 0;
    ret->items = (table_entry**)calloc(TABLE_DEFAULT_SIZE, sizeof(table_entry));

    return ret;
}

static void hashtable_clean_insert(hash_table* table, sds key, void* value)
{
    uint32_t     index;
    uint32_t     collisions = 0;
    table_entry* item;
    table_entry* entry;

    entry = (table_entry*)malloc(sizeof(table_entry));
    if (entry == NULL) {
        log_fatal("Cannot allocate memory for hashtable entry!");
        return;
    }

    entry->key   = sdsdup(key);
    entry->value = value;

    index = hashtable_index(table->size, collisions, key);
    item  = table->items[index];

    while (item && item != &EMPTY) {

        collisions++;
        index = hashtable_index(table->size, collisions, key);
        item  = table->items[index];
    }

    table->items[index] = entry;
    table->count++;
}

static inline void hashtable_adjust(hash_table* table)
{

    float    fill = (float)table->count / (float)table->size;
    uint32_t size;

    if (table->size <= TABLE_DEFAULT_SIZE) {
        return;
    }

    if (fill > MAX_FILL && (table->size) < TABLE_MAX_SIZE) {
        size = table->size * 2;
    } else if (fill < MIN_FILL) {
        size = table->size / 2;
    } else {
        return;
    }

    hash_table* new_table = new_hashtable_size(size);

    for (uint32_t i = 0; i < table->size; i++) {
        table_entry* tmp = table->items[i];
        if (tmp && tmp != &EMPTY) {
            hashtable_clean_insert(new_table, tmp->key, tmp->value);
        }
    }

    table_entry** tmp_items = new_table->items;
    table->size             = new_table->size;
    table->count            = new_table->count;

    new_table->items = table->items;
    free_hashtable(new_table);
}

static void hashtable_clean_delete(hash_table* table, sds key)
{
    uint32_t     index;
    table_entry* item;
    entry_info   info;

    info = hashtable_exists(table, key);
    if (info.entry != NULL) {
        table->items[info.offset] = &EMPTY;
        sdsfree(info.entry->key);
        free(info.entry);
        table->count--;
    }
}

hash_table* new_hashtable()
{
    return new_hashtable_size(TABLE_DEFAULT_SIZE);
}

void free_hashtable(hash_table* table)
{
    for (uint32_t i = 0; i < table->size; i++) {
        table_entry* tmp = table->items[i];
        if (tmp && tmp != &EMPTY) {
            sdsfree(tmp->key);
            free(tmp);
        }
    }
    free(table->items);
    free(table);
}

void hashtable_delete(hash_table* table, sds key)
{
    hashtable_clean_delete(table, key);
    hashtable_adjust(table);
}

void hashtable_insert(hash_table* table, sds key, void* value)
{
    hashtable_clean_insert(table, key, value);
    hashtable_adjust(table);
}

void* hashtable_find(hash_table* table, sds key)
{
    entry_info info;
    info = hashtable_exists(table, key);

    if (info.entry != NULL) {
        return info.entry->value;
    }

    return NULL;
}
