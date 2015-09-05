/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "symtab.h"

/* Symbol table */
static struct symbol *
symtab[NHASH] = { 0 };

/* Map strings to unsigned integers */
static unsigned long
str_hash(char *str)
{
    char c;
    unsigned long hash = 2081;

    while ((c = *str++)) {
        hash = hash ^ (c + (hash << 6) + (hash >> 2));
    }

    return hash;
}

/*
 * Find a symbol in the symbol table, if not found create a new entry.
 */
struct symbol *
symtab_lookup(char *name)
{
    char c;
    uint32_t key;
    struct symbol *sym;

    key = str_hash(name) % NHASH;
    c = name[0];
    for (sym = symtab[key]; sym != NULL; sym = sym->next) {
        if (c != sym->name[0]) {
            continue;
        }

        if (0 == strcmp(name, sym->name)) {
            return sym;
        }
    }

    sym = bmalloc(sizeof(*sym));
    sym->name = bmalloc(sizeof(name) + 1);
    strcpy(sym->name, name);
    sym->def = NULL;

    sym->next = symtab[key];
    symtab[key] = sym;

    return sym;
}
