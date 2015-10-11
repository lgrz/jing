/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "stack.h"

static struct stack stack;

/*
 * Push a list of nodes onto the stack.
 */
void
stack_push(struct node *vars)
{
    struct node_list *list = (struct node_list *)vars;
    size_t i, lsize;

    lsize = list->ary.size;
    for (i = 0; i < lsize; i++) {
        stack.data[stack.size + i] = list->ary.data[i];
    }
    stack.size += lsize;
}

/*
 * Pop a list of nodes from the stack.
 */
void
stack_pop(struct node *vars)
{
    stack.size -= ((struct node_list *)vars)->ary.size;
}

/*
 * Search the stack for a particular symbol.
 */
struct node_symref *
stack_search(struct symbol *item)
{
    size_t i;
    for (i = 0; i < stack.size; i++) {
        if (!strcmp(item->name, stack.data[i]->sym->name)){
            return stack.data[i];
        }
    }
    return NULL;
}

/*
 * Check if the stack is empty.
 */
uint8_t
stack_is_empty()
{
    return !!stack.size;
}
