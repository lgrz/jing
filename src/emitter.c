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
    case NODE_IF:
        emitter_gen_if((struct node_if *)n);
        break;
    case NODE_VAL:
        emitter_gen_value(n);
        break;
    case NODE_INTERRUPT:
    case NODE_WHILE:
        emitter_gen_cond_block((struct node_cond_block *)n);
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

    if (!is_root) {
        fprintf(stream, "[");
    }

    if (list) {
        len = list->ary.size;
    }

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

    /* handle `action` and `procedure` with 0 arguments. */
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

/*
 * Generate an `if` statement.
 */
void
emitter_gen_if(struct node_if *nif)
{
    assert(nif);
    assert(nif->cond);
    assert(nif->elseif_list);

    fprintf(stream, "if(");
    emitter_gen_node(nif->cond);
    fprintf(stream, ", ");
    emitter_gen_list(nif->then, false);

    fprintf(stream, ", ");
    if (nif->elseif_list->ary.size > 0) {
        size_t i;
        for (i = 0; i < nif->elseif_list->ary.size; ++i) {
            struct node_if *nelse_if;

            nelse_if = nif->elseif_list->ary.data[i];
            fprintf(stream, "if(");
            emitter_gen_node(nelse_if->cond);
            fprintf(stream, ", ");
            emitter_gen_list(nelse_if->then, false);
            fprintf(stream, ", ");
        }

        emitter_gen_list(nif->alt, false);
        while (i--) {
            fputc(')', stream);
        }
    } else {
        emitter_gen_list(nif->alt, false);
    }
    fputc(')', stream);
}

/*
 * Generate a value.
 */
void
emitter_gen_value(struct node *nval)
{
    assert(nval);

    if (VAL_BOOL == nval->val.vtype) {
        if (0 == nval->val.u.bool_val) {
            fprintf(stream, "false");
        } else {
            fprintf(stream, "true");
        }
    }
}

/*
 * Generate an `while` or `interrupt` block.
 */
void
emitter_gen_cond_block(struct node_cond_block *cond_block)
{
    assert(cond_block);
    assert(cond_block->cond);
    assert(cond_block->body);

    if (NODE_WHILE == cond_block->type) {
        fprintf(stream, "while(");
    } else {
        fprintf(stream, "interrupt(");
    }
    emitter_gen_node(cond_block->cond);
    fprintf(stream, ", ");
    emitter_gen_list(cond_block->body, false);
    fputc(')', stream);
}
