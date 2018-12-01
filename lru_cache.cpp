
#include "lru_cache.h"
#include <assert.h>
#include <stdio.h>

// Updates LRU.
void lru_set(lru_cache_t *cache, char *key, data_t *data) {
    if (cache->currentSize == cache->maxSize) {
        // Evict LRU element (tail of list)
        table_entry_t *entry;
        HASH_FIND_STR(cache->table, cache->end->key, entry);
        HASH_DEL(cache->table, entry);
        free(entry);

        node_t *node = cache->end;
        cache->end = cache->end->prev;
        cache->end->next = NULL;
        cache->currentSize --;
        free(node->data);
        free(node->key);
        free(node);
    }

    node_t *newNode = (node *)malloc(sizeof(node));
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->data = data;
    newNode->key = strdup(key);

    // Insert into dll.
    newNode->next = cache->start;
    if (cache->start)
        cache->start->prev = newNode;
    cache->start = newNode;
    if (!cache->end) {
        cache->end = newNode;
    }

    // Insert into hash table.
    table_entry_t *table_entry = (table_entry_t *)malloc(sizeof(table_entry_t));
    table_entry->key = newNode->key;
    table_entry->value = newNode;
    HASH_ADD_STR(cache->table, key, table_entry);
    cache->currentSize ++;
}

// Updates LRU.
bool lru_get(lru_cache_t *cache, char *key, data_t **data /*out*/) {
    table_entry_t *entry;
    HASH_FIND_STR(cache->table, key, entry);
    if (!entry) {
        return false;
    }

    node_t *node = entry->value;
    *data = node->data;

    // Update LRU.
    if (node->prev) {
        node->prev->next = node->next;
    }

    if (node->next) {
        node->next->prev = node->prev;
    } else if (node->prev) {
        // Last element, update tail pointer.
        cache->end = node->prev;
    }

    node->prev = NULL;

    if (node != cache->start)
        node->next = cache->start;
    if (node != cache->end)
        cache->start->prev = node;
    cache->start = node;

    return true;
}

void init_cache(lru_cache_t *cache, unsigned size) {
    cache->start = NULL;
    cache->end = NULL;
    cache->table = NULL;
    cache->maxSize = size;
    cache->currentSize = 0;
}

void dump_cache(lru_cache_t *cache, char *out) {
    *out = '\0';
    for(node *n = cache->start; n; n = n->next) {
        sprintf(out + strlen(out), "%d", n->data->hello);
        if (n->next) {
            sprintf(out + strlen(out), " ");
        }
    }
}

int main(int argc, char **argv) {
    lru_cache_t cache;
    init_cache(&cache, 5);
    char out[100];

    // data_t *first = (data_t *)malloc(sizeof(data_t));
    // first->hello = 54;
    // lru_set(&cache, "test1", first);

    // data_t *second = (data_t *)malloc(sizeof(data_t));
    // second->hello = 55;
    // lru_set(&cache, "test2", second);

    // data_t *third = (data_t *)malloc(sizeof(data_t));
    // third->hello = 56;
    // lru_set(&cache, "test3", third);
    
    // dump_cache(&cache, out);
    // assert(!strcmp(out, "56 55 54"));

    // data_t *data = NULL;
    // lru_get(&cache, "test1", &data);
    // assert(data->hello = 54);
    // dump_cache(&cache, out);
    // assert(!strcmp(out, "54 56 55"));

    // data_t *fourth = (data_t *)malloc(sizeof(data_t));
    // fourth->hello = 57;
    // lru_set(&cache, "test4", fourth);

    // data_t *fifth = (data_t *)malloc(sizeof(data_t));
    // fifth->hello = 58;
    // lru_set(&cache, "test5", fifth);

    // data_t *sixth = (data_t *)malloc(sizeof(data_t));
    // sixth->hello = 59;
    // lru_set(&cache, "test6", sixth);

    // data_t *seventh = (data_t *)malloc(sizeof(data_t));
    // seventh->hello = 60;
    // lru_set(&cache, "test7", seventh);

    // dump_cache(&cache, out);
    // assert(!strcmp(out, "60 59 58 57 54"));

    // Memory leak test
    for(int i = 0; i < 10000000; ++i) {
        data_t *seventh = (data_t *)malloc(sizeof(data_t));
        seventh->hello = i;
        char buf[10];
        sprintf(buf, "test%d", i);
        lru_set(&cache, buf, seventh);
    }
}