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
#include <assert.h>

#include "array.h"
#include "type.h"
#include "util.h"

/* Forward declare symtab */
struct symbol;

enum val_type {
    VAL_BOOL
};

struct value {
    uint8_t vtype;
    union {
        uint8_t bool_val;
    } u;
};

enum node_type {
    NODE_NIL,
    NODE_PROC,
    NODE_SYMREF,
    NODE_LIST,
    NODE_COMDCL,
    NODE_SEARCH,
    NODE_IF,
    NODE_VAL,
    NODE_INTERRUPT,
    NODE_WHILE,
    NODE_ITER,
    NODE_CITER
};

struct node {
    uint8_t type;
    struct value val;
};

struct node_proc {
    uint8_t type;
    struct node *args;
    struct node_list *body;
    struct symbol *sym;
};

struct node_symref {
    uint8_t type;
    struct symbol *sym;
    struct node_list *args;
};

struct node_list {
    uint8_t type;
    struct array_list ary;
};

struct node_comdcl {
    uint8_t type;
    uint8_t arity;
    struct symbol *sym;
};

struct node_comstmt {
    uint8_t type;
    struct node_list *body;
};

struct node_if {
    uint8_t type;
    struct node *cond;
    struct node_list *then;
    struct node_list *elseif_list;
    struct node_list *alt;
};

struct node_cond_block {
    uint8_t type;
    struct node *cond;
    struct node_list *body;
};

struct node *
node_proc_new(struct symbol *sym, struct node *n);

void
node_proc_body(struct node *proc, struct node *body);

struct node *
node_symref_new(struct symbol *sym, struct node *args);

struct node *
node_list_new(void);

void
node_list_add(struct node *list, struct node *el);

struct node *
node_comdcl_new(enum type stype, struct symbol *sym, uint8_t arity);

struct node *
node_comstmt_new(enum node_type type, struct node *body);

struct node *
node_if_new(struct node *cond, struct node *then, struct node *elseif_list,
        struct node *alt);

struct node *
node_interrupt_new(struct node *cond, struct node *body);

struct node *
node_while_new(struct node *cond, struct node *body);

struct node_cond_block *
node_cond_block_new(struct node *cond, struct node *body);

struct node *
node_get_true(void);

struct node *
node_get_false(void);

int
node_cmp(const void *xa, const void *xb);

void
node_free(void *n);

/* Forward declare symtab */
void
symtab_set_type(struct symbol *sym, enum type stype);

/* Forward declare symtab */
void
symtab_set_def(struct symbol *sym, struct node *n);

#endif /* NODE_H */
