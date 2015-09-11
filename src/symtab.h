/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "type.h"
#include "util.h"

#define NHASH 1021

struct node;

struct symbol {
    struct symbol *next;
    char *name;
    struct node *def;
    enum type type;
};

struct symbol *
symtab_lookup(char *name);

void
symtab_free(void);

void
symtab_set_type(struct symbol *sym, enum type stype);

void
symtab_set_def(struct symbol *sym, struct node *n);

#endif /* SYMTAB_H */
