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
    sym->type = TNONE;

    sym->next = symtab[key];
    symtab[key] = sym;

    return sym;
}

/*
 * Remove all symbols from the symbol table
 */
void
symtab_free(void)
{
    int i;
    struct symbol *curr, *sym = NULL;

    for (i = 0; i < NHASH; ++i) {
        if (!symtab[i]) {
            continue;
        }

        curr = symtab[i];
        while (curr) {
            sym = curr;
            curr = curr->next;
            free(sym->name);
            free(sym);
        }
        symtab[i] = NULL;
    }
}

/*
 * Set a symbol's type
 *
 * Avoids circular dependencies between `node` and `symtab`.
 */
void
symtab_set_type(struct symbol *sym, enum type stype)
{
    sym->type = stype;
}

/*
 * Set a symbol's definition node.
 *
 * Avoids circular dependencies between `node` and `symtab`.
 */
void
symtab_set_def(struct symbol *sym, struct node *n)
{
    sym->def = n;
}
