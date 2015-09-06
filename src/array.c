/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <assert.h>

#include "array.h"

/*
 * Allocate more memory to the array if required.
 */
static void
array_alloc(struct array_list *ary)
{
    if (ary && ary->size == ary->allocated) {
        ary->allocated *= 2;
        ary->data = brealloc(ary->data, sizeof(void *) * ary->allocated);
    }
}

/*
 * Initialise the array list and allocate the minimum number of elements.
 */
void
array_init(struct array_list *ary, int (*cmp)(const void *, const void*),
        void (*free)(void *))
{
    assert(cmp);
    assert(free);

    if (ary) {
        ary->data = bmalloc(sizeof(void *) * ARRAY_MIN_SIZE);
        ary->size = 0;
        ary->allocated = ARRAY_MIN_SIZE;
        ary->cmp = cmp;
        ary->free = free;
    }
}

/*
 * Free all data elements and the array itself.
 */
void
array_destroy(struct array_list *ary)
{
    size_t i;

    if (ary) {
        for (i = 0; i < ary->size; ++i) {
            ary->free(ary->data[i]);
        }
        free(ary->data);
        ary->size = 0;
        ary->allocated = 0;
        ary->cmp = NULL;
        ary->free = NULL;
    }
}

/*
 * Push a new item onto the end of the array. Resize the array if required.
 */
void
array_push(struct array_list *ary, void *el)
{
    assert(ary);
    assert(el);

    array_alloc(ary);
    ary->data[ary->size++] = el;
}

/*
 * Sort the array.
 */
void
array_sort(struct array_list *ary)
{
    if (ary && ary->size > 1) {
        qsort(ary->data, ary->size, sizeof(void *), ary->cmp);
    }
}
