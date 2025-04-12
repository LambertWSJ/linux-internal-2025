#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include "list_util.h"
#include "common.h"

/* Verify if list is order */
static bool list_is_ordered(const struct list_head *head)
{
    int value = list_entry(head->next, node_t, list)->value;
    node_t *entry;
    list_for_each_entry (entry, head, list) {
        if (entry->value < value)
            return false;
        value = entry->value;
    }

    return true;
}

/* shuffle array, only work if n < RAND_MAX */
void shuffle(int *array, size_t n)
{
    if (n <= 0)
        return;

    for (size_t i = 0; i < n - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
        int t = array[j];
        array[j] = array[i];
        array[i] = t;
    }
}

void quick_sort(struct list_head *list)
{
    int n = list_length(list);
    int value;
    int i = 0;
    int max_level = 2 * n;
    struct list_head *begin[max_level];
    struct list_head *result = NULL, *left = NULL, *right = NULL;
    begin[0] = list->next;
    list->prev->next = NULL;
    while (i >= 0) {
        struct list_head *L = begin[i], *R = list_tail(begin[i]);
        if (L != R) {
            struct list_head *pivot = L;
            value = list_entry(pivot, node_t, list)->value; /* HHHH */
            struct list_head *p = pivot->next;
            pivot->next = NULL; /* break the list */

            while (p) {
                struct list_head *n = p;
                p = p->next;
                int n_value = list_entry(n, node_t, list)->value; /* IIII */
                if (n_value > value) {
                    n->next = right;
                    right = n;
                } else {
                    n->next = left;
                    left = n;
                }
            }

            begin[i] = left;
            begin[i + 1] = pivot; /* JJJJ */
            begin[i + 2] = right; /* KKKK */
            left = right = NULL;
            i += 2;
        } else {
            if (L) {
                L->next = result;
                result = L;
            }
            i--;
        }
    }
    list->next = result;
    rebuild_list_link(list);
}

void list_quicksort(struct list_head *head)
{
    /* check empty */
    if (list_empty(head) || list_is_singular(head))
        return;

    LIST_HEAD(left);
    LIST_HEAD(right);
    /* find piviot */
    node_t *pivot = list_first_entry(head, node_t, list);
    list_del(&pivot->list);

    /* compare with pivot and split into left and right */
    node_t *itr, *safe;
    list_for_each_entry_safe(itr, safe, head, list) {
        if (itr->value < pivot->value)
            list_move(&itr->list, &left);
        else
            list_move_tail(&itr->list, &right);
    }

    /* sort left and right */
    list_quicksort(&left);
    list_quicksort(&right);

    /* head->pivot */
    list_add(&pivot->list, head);
    /* head->left->pivot */
    list_splice(&left, head);
    /* head->left->pivot->right */
    list_splice_tail(&right, head);
}

void run_quiz()
{
    struct list_head *list = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(list);

    size_t count = 100000;
    int *test_arr = malloc(sizeof(int) * count);
    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
        list_construct(list, test_arr[count]);
    quick_sort(list);
    assert(list_is_ordered(list));
    list_free(list);
    free(test_arr);
}

int main(UNUSED int argc, UNUSED char **argv)
{
    LIST_HEAD(list);
    srand(time(NULL));
    size_t count = 100;
    int *test_arr = malloc(sizeof(int) * count);
    for (int i = 0; i < count; ++i)
        test_arr[i] = i;
    shuffle(test_arr, count);

    while (count--)
        list_construct(&list, test_arr[count]);
    free(test_arr);

    list_quicksort(&list);

    assert(list_is_ordered(&list));
    return 0;
}
