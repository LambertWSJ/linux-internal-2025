#include <stdlib.h>
#include "list.h"

typedef struct __node {
    long value;
    struct list_head list;
} node_t;

void quick_sort(struct list_head *list);

static void list_construct(struct list_head *list, int n)
{
    node_t *node = malloc(sizeof(node_t));
    node->value = n;
    list_add(&node->list, list);
}

static void list_free(const struct list_head *head)
{
    node_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, head, list) {
        free(entry);
    }
}

static struct list_head *list_tail(struct list_head *head)
{
    while (head && head->next)
        head = head->next;
    return head;
}

static int list_length(struct list_head *left)
{
    int n = 0;
    struct list_head *node;
    list_for_each(node, left) n++;
    return n;
}

static void rebuild_list_link(struct list_head *head)
{
    if (!head)
        return;
    struct list_head *node, *prev;
    prev = head;
    node = head->next;
    while (node) {
        node->prev = prev;
        prev = node;
        node = node->next;
    }
    prev->next = head;
    head->prev = prev;/* GGGG */
}
