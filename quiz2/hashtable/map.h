#pragma once

#include "types.h"

typedef struct {
    int bits;
    struct hlist_head *ht;
} map_t;

struct hash_key {
    int key;
    void *data;
    struct hlist_node node;
};

void *map_get(map_t *map, int key);

void map_add(map_t *map, int key, void *data);

void map_deinit(map_t *map);

map_t *map_init(int bits);
