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

/*
{
    list_item_t **p;
    for (p = AAAA; *p != BBBB; p = CCCC)
        ;
    *p = item;
    DDDD = before;
}
AAAA, BBBB, CCCC, DDDD 皆為 C 語言表示式，不含 ; 或 , 字元
以最精簡的方式撰寫，不包含空白
 */