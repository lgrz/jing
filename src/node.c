/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "node.h"

/*
 * A wrapper around `symtab_free`.
 */
static void
sym_free(struct symbol *sym)
{
    extern void symtab_free(struct symbol *);

    symtab_free(sym);
}

/*
 * Create a `procedure` node.
 */
struct node *
node_proc_new(struct symbol *sym, struct node *args)
{
    struct node_proc *proc = bmalloc(sizeof(*proc));
    proc->type = NODE_PROC;
    proc->args = args;
    proc->sym = sym;
    proc->body = NULL;

    return (struct node *)proc;
}

/*
 * Assign `procedure` body.
 */
void
node_proc_body(struct node *n, struct node *body)
{
    struct node_proc *proc = (struct node_proc *)n;
    proc->body = (struct node_list *)body;
}

/*
 * Create a reference to a symbol
 */
struct node *
node_symref_new(struct symbol *sym)
{
    struct node_symref *symref = bmalloc(sizeof(*symref));
    symref->type = NODE_SYMREF;
    symref->sym = sym;

    return (struct node *)symref;
}

/*
 * Create a new statement list.
 */
struct node *
node_list_new(void)
{
    struct node_list *list = bmalloc(sizeof(*list));
    list->type = NODE_LIST;
    array_init(&(list->ary), node_cmp, node_free);

    return (struct node *)list;
}

/*
 * Add a statement to the list.
 */
void
node_list_add(struct node *n, struct node *el)
{
    struct node_list *list = (struct node_list *)n;

    array_push(&list->ary, el);
}

/*
 * Compare node `xa` to `xb`.
 */
int
node_cmp(const void *xa, const void *xb)
{
    /* FIXME: implement comparison */

    (void) xa;
    (void) xb;

    return 0;
}

/*
 * Free node resources.
 */
void
node_free(void *del)
{
    struct node *n = (struct node *)del;

    switch (n->type) {
    case NODE_PROC:
        {
            struct node_proc *proc = (struct node_proc *)n;
            if (proc->body) {
                node_free(proc->body);
            }
            sym_free(proc->sym);
        }
        break;
    case NODE_SYMREF:
        {
            struct node_symref *ref = (struct node_symref *)n;
            sym_free(ref->sym);
        }
        break;
    case NODE_LIST:
        {
            struct node_list *list = (struct node_list *)n;
            array_destroy(&(list->ary));
        }
        break;
    case NODE_NIL:
    default:
        fprintf(stderr, "node_free: type error\n");
    }

    free(n);
}
