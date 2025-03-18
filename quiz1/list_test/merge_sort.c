#include <stddef.h>
#include <stdint.h>

#include "list.h"

static list_item_t *merge_two_lists(list_item_t *l1, list_item_t *l2)
{
    list_item_t *list = NULL, **indir = &list;
    list_item_t **node;
    for (node = NULL; l1 && l2; *node = (*node)->next) {
        node = l1->value < l2->value ? &l1 : &l2;
        *indir = *node;
        indir = &(*indir)->next;
    }
    *indir = (list_item_t *) ((uintptr_t) l1 | (uintptr_t) l2);
    return list;
}

list_item_t *list_merge_sort(list_item_t *head)
{
    if (!head || !head->next)
        return head;

    list_item_t *slow = head, *fast;
    for (fast = head->next; fast && fast->next; fast = fast->next->next)
        slow = slow->next;

    list_item_t *mid = slow->next;
    slow->next = NULL;
    list_item_t *left = list_merge_sort(head);
    list_item_t *right = list_merge_sort(mid);
    return merge_two_lists(left, right);
}
