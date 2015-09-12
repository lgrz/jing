/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "emitter.h"

/* stream to write output */
static FILE *
stream = NULL;

/*
 * Initialise emitter.
 */
void
emitter_init(FILE *fp)
{
    if (fp) {
        stream = fp;
    }
}

/*
 * Walk the AST and generate the relevant output code.
 */
void
emitter_walk(struct node *root)
{
    if (root && NODE_LIST == root->type) {
        bool is_root = true;
        struct node_list *list = (struct node_list *)root;

        /* root level list does not require enclosing `[`, `]` */
        emitter_gen_list(list, is_root);
    }
}

/*
 * Handle each type of node.
 */
void
emitter_gen_node(struct node *n)
{
    if (!n) {
        return;
    }

    switch (n->type) {
    case NODE_PROC:
        emitter_gen_proc((struct node_proc *)n);
        break;
    case NODE_LIST:
        emitter_gen_list((struct node_list *)n, false);
        break;
    case NODE_SYMREF:
        emitter_gen_symref((struct node_symref *)n);
        break;
    case NODE_SEARCH:
        emitter_gen_search((struct node_search *)n);
        break;
    case NODE_COMDCL:
        /* unused */
        break;
    case NODE_NIL:
    default:
        fprintf(stderr, "emitter_gen_node: type error\n");
        break;
    }
}

/*
 * Generate a `procedure`
 */
void
emitter_gen_proc(struct node_proc *proc)
{
    assert(proc->sym);

    fprintf(stream, "proc(%s, ", proc->sym->name);
    if (proc->body) {
        emitter_gen_node((struct node *)proc->body);
    } else {
        fprintf(stream, "[]");
    }
    fprintf(stream, ").\n");
}

/*
 * Generate a list of statements
 */
void
emitter_gen_list(struct node_list *list, bool is_root)
{
    size_t i, len = 0;

    assert(list);

    if (!is_root) {
        fprintf(stream, "[");
    }

    len = list->ary.size;
    for (i = 0; i < len; ++i) {
        struct node *n = (struct node *)list->ary.data[i];

        emitter_gen_node(n);
        if (!is_root && i < len - 1) {
            fprintf(stream, ", ");
        }
    }

    if (!is_root) {
        fprintf(stream, "]");
    }
}

/*
 * Generate a symbol refernce.
 */
void
emitter_gen_symref(struct node_symref *ref)
{
    assert(ref);
    assert(ref->sym);

    fprintf(stream, "%s", ref->sym->name);
}

/*
 * Generate a `search` block.
 */
void
emitter_gen_search(struct node_search *search)
{
    assert(search);

    fprintf(stream, "search(");
    if (search->body) {
        emitter_gen_node((struct node *)search->body);
    } else {
        fprintf(stream, "[]");
    }
    fprintf(stream, ")");
}
