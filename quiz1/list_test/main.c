#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "list.h"

#define my_assert(test, message) \
    do {                         \
        if (!(test))             \
            return message;      \
    } while (0)
#define my_run_test(test)       \
    do {                        \
        char *message = test(); \
        tests_run++;            \
        if (message)            \
            return message;     \
    } while (0)

#define N 1000
#define TEST_INSERT 1
#define TEST_SORT 2

static list_item_t items[N];
static list_t l;

static list_t *list_reset(int test)
{
    for (size_t i = 0; i < N; i++) {
        items[i].value = test == TEST_INSERT ? i : rand();
        items[i].next = NULL;
    }
    l.head = NULL;
    return &l;
}

static size_t list_size(list_t *l)
{
    if (!l)
        return 0;
    size_t len = 0;
    struct list_item *node;
    for (node = l->head; node; node = node->next)
        len++;
    return len;
}

static char *test_list(void)
{
    /* Test inserting at the beginning */
    list_reset(TEST_INSERT);
    my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
    for (size_t i = 0; i < N; i++)
        list_insert_before(&l, l.head, &items[i]);
    my_assert(list_size(&l) == N, "Final list size should be N");
    size_t k = N - 1;
    list_item_t *cur = l.head;
    while (cur) {
        my_assert(cur->value == k, "Unexpected list item value");
        k--;
        cur = cur->next;
    }

    /* Test inserting at the end */
    list_reset(TEST_INSERT);
    my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
    for (size_t i = 0; i < N; i++)
        list_insert_before(&l, NULL, &items[i]);
    my_assert(list_size(&l) == N, "Final list size should be N");
    k = 0;
    cur = l.head;
    while (cur) {
        my_assert(cur->value == k, "Unexpected list item value");
        k++;
        cur = cur->next;
    }

    /* Reset the list and insert elements in order (i.e. at the end) */
    list_reset(TEST_INSERT);
    my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
    for (size_t i = 0; i < N; i++)
        list_insert_before(&l, NULL, &items[i]);
    my_assert(list_size(&l) == N, "list size should be N");

    list_reset(TEST_SORT);
    my_assert(list_size(&l) == 0, "Initial list size is expected to be zero.");
    for (size_t i = 0; i < N; i++)
        list_insert_before(&l, l.head, &items[i]);
    my_assert(list_size(&l) == N, "Final list size should be N");
    l.head = list_merge_sort(l.head);
    my_assert(list_size(&l) == N, "Sorted list size should be N");

    list_item_t *prev = NULL;
    for (cur = l.head; cur; cur = cur->next) {
        if (!prev)
            continue;
        my_assert(cur->value > prev->value, "sort error");
    }

    return NULL;
}

int tests_run = 0;

static char *test_suite(void)
{
    srand(time(NULL));
    my_run_test(test_list);
    return NULL;
}

int main(void)
{
    printf("---=[ List tests\n");
    char *result = test_suite();
    if (result)
        printf("ERROR: %s\n", result);
    else
        printf("ALL TESTS PASSED\n");
    printf("Tests run: %d\n", tests_run);
    return !!result;
}
