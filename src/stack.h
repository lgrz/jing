/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "node.h"
#include "symtab.h"

#define STACKSIZE 1024

struct stack {
    size_t size;
    struct node_symref *data[STACKSIZE];
};

void
stack_push(struct node *vars);

void
stack_pop(struct node *vars);

struct node_symref *
stack_search(struct symbol *item);

uint8_t
stack_is_empty();

#endif /* STACK_H */
