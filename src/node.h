/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdint.h>

#include "util.h"

struct symbol;

enum node_type {
    NODE_NIL,
    NODE_PROC
};

struct node {
    uint8_t type;
    struct node *left;
    struct node *right;
};

struct node_proc {
    uint8_t type;
    struct node *args;
    struct node *body;
    struct symbol *sym;
};

struct node *
node_proc_new(struct symbol *sym, struct node *n);

/* FIXME: body should be node_list */
void
node_proc_body(struct node *proc, struct node *body);

#endif /* NODE_H */
