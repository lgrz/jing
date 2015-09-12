/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef EMITTER_H
#define EMITTER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "node.h"
#include "symtab.h"

void
emitter_init(FILE *fp);

void
emitter_walk(struct node *top);

void
emitter_gen_node(struct node *n);

void
emitter_gen_proc(struct node_proc *proc);

void
emitter_gen_list(struct node_list *list, bool is_root);

void
emitter_gen_symref(struct node_symref *ref);

void
emitter_gen_search(struct node_search *search);

#endif /* EMITTER_H */
