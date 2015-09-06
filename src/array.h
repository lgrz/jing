/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

#define ARRAY_MIN_SIZE 8

/* A dynamic array */
struct array_list {
    void **data;
    /* number of slots used */
    size_t size;
    /* space available */
    size_t allocated;
    /* comparison function */
    int (*cmp)(const void *, const void *);
    /* free function */
    void (*free)(void *);
};

void
array_init(struct array_list *ary, int (*cmp)(const void *, const void*),
        void (*free)(void *));

void
array_destroy(struct array_list *ary);

void
array_push(struct array_list *ary, void *el);

void
array_sort(struct array_list *ary);

#endif /* ARRAY_H */
