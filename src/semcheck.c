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
 * Lookup the argument count in the symbol definition.
 */
static uint8_t
semcheck_get_args(struct symbol *sym)
{
    assert(sym);
    assert(sym->def);

    if (NODE_PROC == sym->def->type) {
        struct node_proc *proc = (struct node_proc *)sym->def;

        return proc->args->ary.size;
    }

    if (NODE_COMDCL == sym->def->type) {
        struct node_comdcl *dcl = (struct node_comdcl *)sym->def;

        return dcl->arity;
    }

    fprintf(stderr, "no args for node type %d\n", sym->def->type);

    return 0;
}

/*
 * Determine if the error code is an error.
 */
bool
semcheck_is_error(enum error_code err)
{
    return ENONE != err;
}

/*
 * Map error codes to error messages with location information.
 */
void
semcheck_errorl(YYLTYPE t, enum error_code err, struct node *n)
{
    struct node_symref *ref = NULL;
    char *name = NULL;
    char *op_str = NULL;

    assert(n);

    /*
     * Fill variables required for `symref` error reporting.
     */
    if (NODE_SYMREF == n->type) {
        ref = (struct node_symref *)n;
        assert(ref->sym);
        assert(ref->sym->name);
        name = ref->sym->name;
    } else if (NODE_EXPR == n->type) {
        op_str = ((struct node_expr *)n)->operator;
    }

    switch (err) {
    case E0001:
        yyerrorl(t, "unresolved name `%s`", name);
        break;
    case E0002:
        yyerrorl(t, "unresolved action `%s`", name);
        break;
    case E0003:
        assert(ref->args);
        yyerrorl(t, "`%s` takes %d parameters but found %d", name,
                semcheck_get_args(ref->sym), ref->args->ary.size);
        break;
    case E0010:
        yyerrorl(t, "invalid unary expression, found integer `%ld`",
                n->val.u.int_val);
        break;
    case E0011:
        yyerrorl(t, "invalid expression, found callable "
                "(`action` or `procedure`) `%s`", name);
        break;
    case E0012:
        yyerrorl(t, "invalid operands to binary operator `%s` (rel fluent)",
                op_str);
        break;
    case E0013:
        yyerrorl(t, "invalid unary expression, found fun fluent `%s`", name);
        break;
    default:
        yyerrorl(t, "unhandled error code %d", err);
    }
}

/*
 * Check the argument count for a symbol reference.
 */
enum error_code
semcheck_sym_args(struct node *n)
{
    enum error_code err_ret = ENONE;
    struct node_symref *ref;

    assert(n);
    assert(n->type == NODE_SYMREF);

    ref = (struct node_symref *)n;

    switch (ref->sym->type) {
    case TACTION:
    {
        struct node_comdcl *dcl = (struct node_comdcl *)ref->sym->def;
        if (ref->args->ary.size != dcl->arity) {
            err_ret = E0003;
        }
    }
        break;
    case TPROC:
    {
        struct node_proc *proc = (struct node_proc *)ref->sym->def;
        if (ref->args->ary.size != proc->args->ary.size) {
            err_ret = E0003;
        }
    }
        break;
    case TNONE:
    default:
        /* don't report args error for unresolved symbols */
        break;
    }

    return err_ret;
}

/*
 * Check a usercall reference is defined. A usercall is an `action` or a
 * `procedure` call.
 */
enum error_code
semcheck_usercall_defined(struct symbol *sym)
{
    assert(sym);

    if (TACTION == sym->type || TPROC == sym->type) {
        return ENONE;
    }

    return E0001;
}

/*
 * Check `action` reference is defined.
 */
enum error_code
semcheck_action_defined(struct symbol *sym)
{
    assert(sym);

    if (TACTION == sym->type) {
        return ENONE;
    }

    return E0002;
}

/*
 * Determine if a node is a `rel fluent`.
 */
static bool
is_rel_fluent(struct node *n)
{
    struct node_symref *ref;

    assert(n);
    if (NODE_SYMREF == n->type) {
        ref = (struct node_symref *)n;
        assert(ref->sym);

        return ref->sym->type == TFLUENTREL;
    }

    return false;
}

/*
 * Formula expression semantic checks.
 */
enum error_code
semcheck_expr(struct node *n)
{
    /* unary integer */
    if (NODE_VAL == n->type && VAL_INT == n->val.vtype) {
        return E0010;
    }

    if (NODE_SYMREF == n->type) {
        struct node_symref *ref = (struct node_symref *)n;
        assert(ref->sym);
        /* undefined atom in an expression */
        if (TNONE == ref->sym->type) {
            return E0001;
        }

        /* callable used in expression */
        if (TACTION == ref->sym->type || TPROC == ref->sym->type) {
            return E0011;
        }

        /* unary fun fluent */
        if (TFLUENTFUN == ref->sym->type) {
            return E0013;
        }
    }

    if (NODE_EXPR == n->type) {
        struct node_expr *op = (struct node_expr *)n;
        if (is_rel_fluent(op->left) || is_rel_fluent(op->right)) {
            return E0012;
        }
    }

    return ENONE;
}
