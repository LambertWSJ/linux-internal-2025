#pragma once
#include <stddef.h>


typedef struct list_item {
    int value;
    struct list_item *next;
} list_item_t;

typedef struct {
    struct list_item *head;
} list_t;

static inline void list_insert_before(list_t *l,
                        struct list_item *before,
                        struct list_item *item)
{
    list_item_t **p;
    for (p = &l->head; *p != before; p = &(*p)->next)
        ;
    *p = item;
    item->next = before;
}
