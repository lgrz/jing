/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "jing.h"

/* AST root node */
struct node *
ntop = NULL;

static char *
infile = NULL;

void
yyerror(char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

/*
 * Stub out the emitter module.
 */
void
emitter_walk_stub(struct node *top, FILE *stream)
{
    if (NODE_PROC == top->type) {
        struct node_proc *proc = (struct node_proc *)top;
        fprintf(stream, "proc(%s, []).\n", proc->sym->name);

        free(proc->sym->name);
        free(proc->sym);
        free(proc);
    }
}

int
main(int argc, char **argv)
{
    if (2 != argc) {
        fprintf(stderr, "todo usage\n");
        exit(EXIT_FAILURE);
    }

    infile = argv[1];
    if (!(yyin = fopen(infile, "r"))) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    yyparse();

    yylex_destroy();
    fclose(yyin);

    emitter_walk_stub(ntop, stdout);

    return 0;
}
