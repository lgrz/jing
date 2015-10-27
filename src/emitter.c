/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include "emitter.h"

/* strbuf to write output */
static struct strbuf *
buf = NULL;

/*
 * Initialise emitter.
 *
 * `emitter` takes ownership of `bufp` and must `free` it when done.
 */
void
emitter_init(struct strbuf *bufp)
{
    if (bufp) {
        buf = bufp;
    }
}

/*
 * Free the string buffer.
 */
void
emitter_free(void)
{
    if (buf) {
        strbuf_free(buf);
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
        emitter_gen_list_brackets((struct node_list *)n);
        break;
    case NODE_SYMREF:
        emitter_gen_symref((struct node_symref *)n);
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
    case NODE_OP:
        emitter_gen_op((struct node_op *)n);
        break;
    case NODE_ITER:
        strbuf_append(buf, "star(");
        emitter_gen_comstmt((struct node_comstmt *)n);
        strbuf_append(buf, ")");
        break;
    case NODE_CITER:
        strbuf_append(buf, "iconc(");
        emitter_gen_comstmt((struct node_comstmt *)n);
        strbuf_append(buf, ")");
        break;
    case NODE_SEARCH:
        strbuf_append(buf, "search(");
        emitter_gen_comstmt((struct node_comstmt *)n);
        strbuf_append(buf, ")");
        break;
    case NODE_OR:
        emitter_gen_comstmt((struct node_comstmt *)n);
        break;
    case NODE_NDET:
        {
            struct node_comstmt *stmt = (struct node_comstmt *)n;
            emitter_gen_multistmt(stmt, "ndet", stmt->body->ary.size);
            break;
        }
    case NODE_CONC:
        {
            struct node_comstmt *stmt = (struct node_comstmt *)n;
            emitter_gen_multistmt(stmt, "conc", stmt->body->ary.size);
            break;
        }
    case NODE_PCONC:
        {
            struct node_comstmt *stmt = (struct node_comstmt *)n;
            emitter_gen_multistmt(stmt, "pconc", stmt->body->ary.size);
            break;
        }
    case NODE_FORMULA:
        emitter_gen_formula((struct node_formula *)n);
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

    strbuf_append(buf, "proc(%s, ", proc->sym->name);
    if (proc->body) {
        emitter_gen_node((struct node *)proc->body);
    } else {
        strbuf_append(buf, "[]");
    }
    strbuf_append(buf, ").\n");
}

/*
 * Generate a list of statements
 */
void
emitter_gen_list(struct node_list *list)
{
    size_t i, len = 0;

    if (list) {
        len = list->ary.size;
    }

    for (i = 0; i < len; ++i) {
        struct node *n = (struct node *)list->ary.data[i];

        emitter_gen_node(n);
        if (i < len - 1) {
            strbuf_append(buf, ", ");
        }
    }
}

/*
 * Generate a list of statements with surrounding square brackets
 */
void
emitter_gen_list_brackets(struct node_list *list)
{
    strbuf_append(buf, "[");
    emitter_gen_list(list);
    strbuf_append(buf, "]");
}

/*
 * Generate a list of statements with surrounding parentheses
 */
void
emitter_gen_list_parens(struct node_list *list)
{
    strbuf_append(buf, "(");
    emitter_gen_list(list);
    strbuf_append(buf, ")");
}

/*
 * Generate a symbol reference.
 */
void
emitter_gen_symref(struct node_symref *ref)
{
    assert(ref);
    assert(ref->sym);

    /* handle `action` and `procedure` with 0 arguments. */
    strbuf_append(buf, "%s", ref->sym->name);
    if (ref->args && ref->args->ary.size) {
        emitter_gen_list_parens(ref->args);
    }
}

/*
 * Generate `ndet`, `conc`, or `pconc` statements
 */
void
emitter_gen_multistmt(struct node_comstmt *stmt, const char *string,
        size_t stmts_left)
{
    size_t size = stmt->body->ary.size;

    strbuf_append(buf, "%s(", string);
    emitter_gen_node((struct node *)stmt->body->ary.data[size - stmts_left]);
    strbuf_append(buf, ", ");
    if (stmts_left > 2) {
        emitter_gen_multistmt(stmt, string, stmts_left - 1);
    } else {
        emitter_gen_node((struct node *)stmt->body->ary.data[size-1]);
    }
    strbuf_append(buf, ")");
}

/*
 * Generate `star`, `iconc`, or `search` statements
 */
void
emitter_gen_comstmt(struct node_comstmt *stmt)
{
    assert(stmt);

    if (stmt->body) {
        emitter_gen_node((struct node *)stmt->body);
    } else {
        strbuf_append(buf, "[]");
    }
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

    strbuf_append(buf, "if(");
    emitter_gen_node(nif->cond);
    strbuf_append(buf, ", ");
    emitter_gen_list_brackets(nif->then);

    strbuf_append(buf, ", ");
    if (nif->elseif_list->ary.size > 0) {
        size_t i;
        for (i = 0; i < nif->elseif_list->ary.size; ++i) {
            struct node_if *nelse_if;

            nelse_if = nif->elseif_list->ary.data[i];
            strbuf_append(buf, "if(");
            emitter_gen_node(nelse_if->cond);
            strbuf_append(buf, ", ");
            emitter_gen_list_brackets(nelse_if->then);
            strbuf_append(buf, ", ");
        }

        emitter_gen_list_brackets(nif->alt);
        while (i--) {
            strbuf_append(buf, ")");
        }
    } else {
        emitter_gen_list_brackets(nif->alt);
    }
    strbuf_append(buf, ")");
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
            strbuf_append(buf, "false");
        } else {
            strbuf_append(buf, "true");
        }
    } else {
        strbuf_append(buf, "%ld", nval->val.u.int_val);
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
        strbuf_append(buf, "while(");
    } else {
        strbuf_append(buf, "interrupt(");
    }
    emitter_gen_node(cond_block->cond);
    strbuf_append(buf, ", ");
    emitter_gen_list_brackets(cond_block->body);
    strbuf_append(buf, ")");
}

/*
 * Generate an op.
 */
void
emitter_gen_op(struct node_op *op)
{
    assert(op);

    switch (op->otype) {
    case ONOT:
        strbuf_append(buf, "neg");
        break;
    case OANDAND:
        strbuf_append(buf, "and");
        break;
    case OOROR:
        strbuf_append(buf, "or");
        break;
    case ONONE:
        strbuf_append(buf, "(");
        emitter_gen_node(op->left);
        strbuf_append(buf, ")");
        return;
    default:
        break;
    }

    if (op->otype < OPROLOGBEGIN) {
        /* output IndiGolog logic expression */
        strbuf_append(buf, "(");
        emitter_gen_node(op->left);
        if (op->otype != ONOT) {
            strbuf_append(buf, ", ");
            emitter_gen_node(op->right);
        }
        strbuf_append(buf, ")");
    } else {
        /* output Prolog expression */
        emitter_gen_node(op->left);
        strbuf_append(buf, " %s ", op->op_str);
        emitter_gen_node(op->right);
    }
}

/*
 * Generate formula `?(...)`.
 */
void
emitter_gen_formula(struct node_formula *formula)
{
    assert(formula);
    assert(formula->cond);

    strbuf_append(buf, "?(");
    emitter_gen_node(formula->cond);
    strbuf_append(buf, ")");
}

/*
 * Get a reference to the generated code.
 */
const char *
emitter_get_str(void)
{
    if (buf && buf->data) {
        return buf->data;
    }

    return "";
}
