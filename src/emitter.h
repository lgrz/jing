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
#include <assert.h>

#include "node.h"
#include "symtab.h"

void
emitter_init(FILE *fp);

void
emitter_gen_node(struct node *n);

void
emitter_gen_proc(struct node_proc *proc);

void
emitter_gen_list(struct node_list *list);

void
emitter_gen_symref(struct node_symref *ref);

void
emitter_gen_comstmt(struct node_comstmt *stmt);

void
emitter_gen_if(struct node_if *nif);

void
emitter_gen_value(struct node *nval);

void
emitter_gen_cond_block(struct node_cond_block *cond_block);

void
emitter_gen_multistmt(struct node_comstmt *stmt, char *string,
        size_t stmts_left);

#endif /* EMITTER_H */
