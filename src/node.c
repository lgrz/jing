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
 * Create a `procedure` node.
 */
struct node *
node_proc_new(struct symbol *sym, struct node *args)
{
    struct node_proc *proc = bmalloc(sizeof(*proc));
    proc->type = NODE_PROC;
    proc->args = (struct node_list *)args;
    proc->sym = sym;
    proc->body = NULL;

    symtab_set_type(sym, TPROC);
    symtab_set_def(sym, (struct node *)proc);

    /* set arguments type */
    assert(proc->args);
    for (size_t i = 0; i < proc->args->ary.size; ++i) {
        assert(proc->args->ary.data[i]);
        struct node_symref *ref =
            (struct node_symref *)proc->args->ary.data[i];
        symtab_set_type(ref->sym, TPROCVAR);
    }

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
node_symref_new(struct symbol *sym, struct node *args)
{
    struct node_symref *symref;

    symref = bmalloc(sizeof(*symref));
    symref->type = NODE_SYMREF;
    symref->sym = sym;
    symref->args = (struct node_list *)args;

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
 * Append a list to another.
 */
void
node_list_append(struct node *start, struct node *end)
{
    struct node_list *end_list = (struct node_list *)end;
    size_t i, size = ((struct node_list *)end)->ary.size;

    for (i = 0; i < size; i++) {
        node_list_add(start, end_list->ary.data[i]);
    }
    free(end_list->ary.data);
    free(end_list);
}

/*
 * Create a common declaration, one of `action`, `fluent`, `prolog`.
 */
struct node *
node_comdcl_new(enum type stype, struct symbol *sym, uint8_t arity)
{
    struct node_comdcl *dcl = bmalloc(sizeof(*dcl));

    dcl->type = NODE_COMDCL;
    dcl->arity = arity;
    dcl->sym = sym;

    symtab_set_type(sym, stype);
    symtab_set_def(sym, (struct node *)dcl);

    return (struct node *)dcl;
}

/*
 * Create a block node. A block node has a header and a list of statements.
 */
struct node *
node_block_new(enum node_type type, struct node *body)
{
    struct node_block *blk = bmalloc(sizeof(*blk));

    blk->type = type;
    blk->body = (struct node_list *)body;

    return (struct node *)blk;
}

/*
 * Create a 'pick' node.
 */
struct node *
node_pick_new(struct node *vars, struct node *body)
{
    struct node_pick *pick = bmalloc(sizeof(*pick));

    pick->type = NODE_PICK;
    pick->vars = (struct node_list *)vars;
    pick->body = (struct node_list *)body;

    return (struct node *)pick;
}

/*
 * Create an `if` node.
 */
struct node *
node_if_new(struct node *cond, struct node *then, struct node *elseif_list,
        struct node *alt)
{
    struct node_if *nif = bmalloc(sizeof(*nif));
    nif->type = NODE_IF;
    nif->cond = cond;
    nif->then = (struct node_list *)then;
    nif->elseif_list = (struct node_list *)elseif_list;
    nif->alt = (struct node_list *)alt;

    return (struct node *)nif;
}

/*
 * Create an `interrupt` node.
 */
struct node *
node_interrupt_new(struct node *cond, struct node *body)
{
    struct node_cond_block *interrupt = node_cond_block_new(cond, body);
    interrupt->type = NODE_INTERRUPT;

    return (struct node *)interrupt;
}

/*
 * Create a `while` node.
 */
struct node *
node_while_new(struct node *cond, struct node *body)
{
    struct node_cond_block *nwhile = node_cond_block_new(cond, body);
    nwhile->type = NODE_WHILE;

    return (struct node *)nwhile;
}

/*
 * Create a node with a condition and a block of code.
 */
struct node_cond_block *
node_cond_block_new(struct node *cond, struct node *body)
{
    struct node_cond_block *cond_block = bmalloc(sizeof(*cond_block));
    cond_block->cond = cond;
    cond_block->body = (struct node_list *)body;

    return cond_block;
}

/*
 * Create an `op` node.
 */
struct node *
node_op_new(enum op_type op_type, char *op_str, struct node *left,
        struct node *right)
{
    struct node_op *op = bmalloc(sizeof(*op));
    op->type = NODE_OP;
    op->op_str = op_str;
    op->left = left;
    op->right = right;
    op->otype = op_type;

    return (struct node *)op;
}

/*
 * Create formula node.
 */
struct node *
node_formula_new(struct node *cond)
{
    struct node_formula *f = bmalloc(sizeof(*f));
    f->type = NODE_FORMULA;
    f->cond = cond;

    return (struct node *)f;
}

/*
 * Create new integer.
 */
struct node *
node_int_new(long n)
{
    struct node *num = bmalloc(sizeof(*num));

    num->type = NODE_VAL;
    num->val.vtype = VAL_INT;
    num->val.u.int_val = n;

    return num;
}

/*
 * Node value `true`.
 */
struct node *
node_get_true(void)
{
    static struct node ntrue;

    ntrue.type = NODE_VAL;
    ntrue.val.vtype = VAL_BOOL;
    ntrue.val.u.bool_val = 1;

    return &ntrue;
}

/*
 * Node value `false`.
 */
struct node *
node_get_false(void)
{
    static struct node nfalse;

    nfalse.type = NODE_VAL;
    nfalse.val.vtype = VAL_BOOL;
    nfalse.val.u.bool_val = 1;

    return &nfalse;
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

    if (!n) {
        return;
    }

    switch (n->type) {
    case NODE_PROC:
        {
            struct node_proc *proc = (struct node_proc *)n;
            if (proc->args) {
                node_free(proc->args);
            }
            if (proc->body) {
                node_free(proc->body);
            }
        }
        break;
    case NODE_LIST:
        {
            struct node_list *list = (struct node_list *)n;
            array_destroy(&(list->ary));
        }
        break;
    case NODE_SYMREF:
        {
            struct node_symref *ref = (struct node_symref *)n;
            node_free(ref->args);
        }
        break;
    case NODE_COMDCL:
        /* unused */
        break;
    case NODE_PICK:
        {
            struct node_pick *pick = (struct node_pick *)n;
            node_free(pick->vars);
            node_free(pick->body);
        }
        break;
    case NODE_VAL:
        {
            /* bool value is static */
            if (VAL_BOOL == n->val.vtype) {
                return;
            }
        }
        break;
    case NODE_ITER:
    case NODE_CITER:
    case NODE_SEARCH:
    case NODE_NDET:
    case NODE_CONC:
    case NODE_PCONC:
        {
            struct node_block *blk = (struct node_block *)n;
            if (blk->body) {
                node_free(blk->body);
            }
        }
        break;
    case NODE_IF:
        {
            struct node_if *nif = (struct node_if *)n;
            node_free(nif->cond);
            node_free(nif->then);
            node_free(nif->elseif_list);
            node_free(nif->alt);
        }
        break;
    case NODE_INTERRUPT:
    case NODE_WHILE:
        {
            struct node_cond_block *cond_block = (struct node_cond_block *)n;
            node_free(cond_block->cond);
            node_free(cond_block->body);
        }
        break;
    case NODE_OP:
        {
            struct node_op *op = (struct node_op *)n;
            free(op->op_str);
            node_free(op->left);
            node_free(op->right);
        }
        break;
    case NODE_FORMULA:
        {
            struct node_formula *f = (struct node_formula *)n;
            node_free(f->cond);
        }
        break;
    case NODE_NIL:
    default:
        fprintf(stderr, "node_free: type error %d \n", n->type);
    }

    free(n);
}
