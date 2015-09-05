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
    proc->body = body;
}
