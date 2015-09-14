/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "semcheck.h"

/*
 * Display error message.
 */
static void
semcheck_emit_err(const char *fmt, ...)
{
    va_list ap;
    int dummy_lineno = 1;

    fprintf(stderr, "%d: error: ", dummy_lineno);
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
}

/*
 * Walk the AST and check for semantic errors.
 */
void
semcheck_walk(struct node *root)
{
    if (root && NODE_LIST == root->type) {
        struct node_list *list = (struct node_list *)root;

        semcheck_chk_list(list);
    }
}

/*
 * Handle each type of node.
 */
void
semcheck_chk_node(struct node *n)
{
    if (!n) {
        return;
    }

    switch (n->type) {
    case NODE_PROC:
        semcheck_chk_proc((struct node_proc *)n);
        break;
    case NODE_LIST:
        semcheck_chk_list((struct node_list *)n);
        break;
    case NODE_SYMREF:
        semcheck_chk_symref((struct node_symref *)n);
        break;
    case NODE_ITER:
    case NODE_CITER:
    case NODE_SEARCH:
        semcheck_chk_comstmt((struct node_comstmt *)n);
        break;
    case NODE_NDET:
    case NODE_CONC:
    case NODE_PCONC:
        break;
    case NODE_COMDCL:
        /* unused */
        break;
    case NODE_IF:
        semcheck_chk_if((struct node_if *)n);
        break;
    case NODE_INTERRUPT:
    case NODE_WHILE:
        semcheck_chk_cond_block((struct node_cond_block *)n);
        break;
    case NODE_NIL:
    default:
        semcheck_emit_err("semcheck_chk_node: type error");
        break;
    }
}

/*
 * Traverse a `procedure`.
 */
void
semcheck_chk_proc(struct node_proc *proc)
{
    assert(proc->sym);

    /*
     * FIXME: check `procedure` is not already defined elsewhere.
     *        a simple solution may be to have a counter on `struct symbol`.
     */

    if (proc->body) {
        semcheck_chk_list(proc->body);
    }
}

/*
 * Traverse each node in a `node_list`.
 */
void
semcheck_chk_list(struct node_list *list)
{
    size_t i;

    assert(list);

    for (i = 0; i < list->ary.size; ++i) {
        struct node *n = (struct node *)list->ary.data[i];

        semcheck_chk_node(n);
    }
}

/*
 * Check a symbol reference.
 */
void
semcheck_chk_symref(struct node_symref *ref)
{
    assert(ref);
    assert(ref->sym);

    if (TNONE == ref->sym->type) {
        /*
         * FIXME: convert `sym->type` to human readable form
         * FIXME: lineno info is incorrect
         */
        semcheck_emit_err("expected action or procedure call, found `%d`\n",
                ref->sym->type);
        error_exit();
    }
}

/*
 * Check a `search`, `iter`, or `citer` block.
 */
void
semcheck_chk_comstmt(struct node_comstmt *stmt)
{
    assert(stmt);

    if (stmt->body) {
        semcheck_chk_list(stmt->body);
    }
}

/*
 * Check an `if` statement.
 */
void
semcheck_chk_if(struct node_if *nif)
{
    size_t i;

    assert(nif);
    assert(nif->cond);
    assert(nif->elseif_list);

    /* FIXME: semcheck `nif->cond` */
    if (nif->then) {
        semcheck_chk_list(nif->then);
    }
    if (nif->elseif_list) {
        for (i = 0; i < nif->elseif_list->ary.size; ++i) {
            struct node_if *nelse_if;

            nelse_if = nif->elseif_list->ary.data[i];
            /* FIXME: semcheck `nelse_if->cond` */
            if (nelse_if->then) {
                semcheck_chk_list(nelse_if->then);
            }
        }
    }
    if (nif->alt) {
        semcheck_chk_list(nif->alt);
    }
}

/*
 * Check node that has a conditional and a block of code.
 */
void
semcheck_chk_cond_block(struct node_cond_block *cond_block)
{
    assert(cond_block);
    assert(cond_block->cond);
    assert(cond_block->body);

    /* FIXME: semcheck `cond_block->cond` */
    semcheck_chk_list(cond_block->body);
}
