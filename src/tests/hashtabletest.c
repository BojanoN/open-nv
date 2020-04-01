#include "util/hashtable.h"
#include "logc/log.h"
#include "sds/sds.h"
#include <assert.h>

int main(void)
{
    hash_table* table;

    table = new_hashtable();
    int value = 3;
    unsigned int size_before = table->size;
    unsigned int size_after;

    log_info("Size before insertion: %d", size_before);
    hashtable_insert(table, "test", (void *) &value);
    size_after = table->size;
    log_info("Size after insertion: %d", table->size);
    assert(size_after == size_before);

    int stored = *((int *)hashtable_find(table, "test"));
    log_info("Inserted value / stored value: %d / %d", value, stored);


    hashtable_delete(table, "test");

    free_hashtable(table);

    return 0;
}
