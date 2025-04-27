#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

typedef struct block {
    size_t size;
    struct block *l, *r;
} block_t;

block_t **find_free_tree(block_t **root, block_t *target)
{
    for (block_t **indir = root, *itr; *indir;) {
        itr = *indir;
        if (itr->size == target->size)
            return indir;
        indir = itr->size > target->size ? &itr->l : &itr->r;
    }
    return NULL;
}

block_t **find_free_tree_recur(block_t **root, block_t *target)
{
    block_t *node = *root;
    if (!node)
        return root;
    if (node == target)
        return root;
    else
        return node->size < target->size
                   ? find_free_tree_recur(&node->r, target)
                   : find_free_tree_recur(&node->l, target);
    return NULL;
}

/* find_predecessor_free_tree */
block_t *find_predecessor_free_tree(block_t **root, block_t *node)
{
    block_t *pred = NULL;
    for (block_t *itr = *root, **indir = &itr; *indir;) {
        itr = *indir;
        indir = itr->size < node->size?(pred = itr, &itr->r): &itr->l;
    }
    return pred;
}

block_t *find_predecessor_free_tree_ori(block_t **root, block_t *node)
{
    block_t *pred;
    for (block_t *itr = *root; itr;) {
        if (itr->size < node->size) {
            pred = itr;
            itr = itr->r;
        } else
            itr = itr->l;
    }
    return pred;
}

void block_insert_free_tree(block_t **root, block_t *node)
{
    block_t **indir = root;
    for (block_t *itr; *indir;) {
        itr = *indir;
        indir = itr->size > node->size ? &itr->l : &itr->r;
    }

    *indir = node;
}

void block_insert_free_tree_recur(block_t **root, block_t *node)
{
    block_t *itr = *root;
    if (!itr) {
        *root = node;
        return;
    }
    block_t **next = itr->size > node->size ? &itr->l : &itr->r;
    block_insert_free_tree_recur(next, node);
}

void block_insert_free_tree_itr(block_t **root, block_t *node)
{
    if (!node)
        return;
    if (!*root && node) {
        *root = node;
        return;
    }
    block_t *proot = *root;
    while (proot) {
        if (proot->size < node->size) {
            if (!proot->r) {
                proot->r = node;
                break;
            }
            proot = proot->r;
        } else {
            if (!proot->l) {
                proot->l = node;
                break;
            }
            proot = proot->l;
        }
    }
}

/*
 * Structure representing a free memory block in the memory allocator.
 * The free tree is a binary search tree that organizes free blocks (of type block_t)
 * to efficiently locate a block of appropriate size during memory allocation.
 */
void remove_free_tree(block_t **root, block_t *target)
{
    /* Locate the pointer to the target node in the tree. */
    block_t **node_ptr = find_free_tree(root, target);

    /* If the target node has two children, we need to find a replacement. */
    if ((*node_ptr)->l && (*node_ptr)->r) {
        /* Find the in-order predecessor:
         * This is the rightmost node in the left subtree.
         */
        block_t **pred_ptr = &(*node_ptr)->l;
        while ((*pred_ptr)->r)
            pred_ptr = &(*pred_ptr)->r;

        /* Verify the found predecessor using a helper function (for debugging).
         */
        block_t *expected_pred = find_predecessor_free_tree(root, *node_ptr);
        assert(expected_pred == *pred_ptr);

        /* If the predecessor is the immediate left child. */
        if (*pred_ptr == (*node_ptr)->l) {
            block_t *old_right = (*node_ptr)->r;
            *node_ptr = *pred_ptr; /* Replace target with its left child. */
            (*node_ptr)->r = old_right; /* Attach the original right subtree. */
            assert(*node_ptr != (*node_ptr)->l);
            assert(*node_ptr != (*node_ptr)->r);
        } else {
            /* The predecessor is deeper in the left subtree. */
            block_t *old_left = (*node_ptr)->l;
            block_t *old_right = (*node_ptr)->r;
            block_t *pred_node = *pred_ptr;
            /* Remove the predecessor from its original location. */
            remove_free_tree(&old_left, *pred_ptr);
            /* Replace the target node with the predecessor. */
            *node_ptr = pred_node;
            (*node_ptr)->l = old_left;
            (*node_ptr)->r = old_right;
            assert(*node_ptr != (*node_ptr)->l);
            assert(*node_ptr != (*node_ptr)->r);
        }
    }
    /* If the target node has one child (or none), simply splice it out. */
    else if ((*node_ptr)->l || (*node_ptr)->r) {
        block_t *child = ((*node_ptr)->l) ? (*node_ptr)->l : (*node_ptr)->r;
        *node_ptr = child;
    } else {
        /* No children: remove the node. */
        *node_ptr = NULL;
    }

    /* Clear the removed node's child pointers to avoid dangling references. */
    target->l = NULL;
    target->r = NULL;
}

block_t *init_free_tree(size_t size)
{
    block_t *root = malloc(sizeof(block_t));
    root->size = size;
    root->l = root->r = NULL;
    return root;
}

static void inorder_traversal(block_t *root)
{
    if (!root)
        return;

    inorder_traversal(root->l);
    printf("%ld ", root->size);
    inorder_traversal(root->r);
}

void dump_free_tree(block_t *root)
{
    inorder_traversal(root);
    puts("");
}

int main()
{
    size_t sizes[] = {50, 30, 70, 20, 40, 60, 80};
    block_t *root = NULL;
    for (size_t i = 0; i < ARRAY_SIZE(sizes); i++) {
        block_t *block = init_free_tree(sizes[i]);
        block_insert_free_tree(&root, block);
    }
    dump_free_tree(root);
    remove_free_tree(&root, root->l);
    dump_free_tree(root);

    return 0;
}