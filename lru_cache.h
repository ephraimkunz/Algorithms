#include "uthash.h"
#include <stdbool.h>

typedef struct data {
    int hello;
} data_t;

typedef struct node node_t;
struct node {
    node_t *prev;
    node_t *next;
    data_t *data;
    char *key; // Just for easier debugging / testing.
};

typedef struct table_entry {
    char *key;
    node_t *value;
    UT_hash_handle hh;
} table_entry_t;

typedef struct lru_cache {
    node_t *start;
    node_t *end;

    table_entry_t *table;
    
    unsigned currentSize;
    unsigned maxSize;
} lru_cache_t;

void lru_set(lru_cache_t *cache, char *key, data_t *data);
bool lru_get(lru_cache_t *cache, char *key, data_t *data /*out*/);