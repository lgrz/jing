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
 * Destroy AST and symbol table.
 */
void
jing_destroy(void)
{
    node_free(ntop);
    symtab_free();
}

/*
 * Exit with error status
 */
int
error_exit(void)
{
    jing_destroy();
    exit(EXIT_FAILURE);
}

/*
 * Stub out semantic analysis.
 */
void
semcheck_stub(struct node *top)
{
    if (top && NODE_LIST == top->type) {
        struct node_list *list = (struct node_list *)top;
        size_t i, j;

        for (i = 0; i < list->ary.size; ++i) {
            struct node *n = (struct node *)list->ary.data[i];

            /* just handle proc case for now */
            if (n && NODE_PROC == n->type) {
                size_t len = 0;
                struct node_proc *proc = (struct node_proc *)n;

                if (proc->body) {
                    len = proc->body->ary.size;
                }

                for (j = 0; j < len; ++j) {
                    /* can get away with only `node_symref` for now */
                    struct node_symref *ref;

                    ref = (struct node_symref *)proc->body->ary.data[j];
                    if (TACTION != ref->sym->type) {
                        /*
                         * FIXME: convert `sym->type` to human readable form
                         * FIXME: lineno info is incorrect
                         */
                        yyerror("expected action, found `%d`", ref->sym->type);
                        error_exit();
                    }
                }
            }
        }
    }
}

/*
 * Stub out the emitter module.
 */
void
emitter_walk_stub(struct node *top, FILE *stream)
{
    if (top && NODE_LIST == top->type) {
        struct node_list *list = (struct node_list *)top;
        size_t i, j;

        for (i = 0; i < list->ary.size; ++i) {
            struct node *n = (struct node *)list->ary.data[i];

            /* just handle proc case for now */
            if (n && NODE_PROC == n->type) {
                size_t len = 0;
                struct node_proc *proc = (struct node_proc *)n;

                fprintf(stream, "proc(%s, [", proc->sym->name);
                if (proc->body) {
                    len = proc->body->ary.size;
                }

                for (j = 0; j < len; ++j) {
                    /* can get away with only `node_symref` for now */
                    struct node_symref *ref;

                    ref = (struct node_symref *)proc->body->ary.data[j];
                    fprintf(stream, "%s", ref->sym->name);
                    if (j < len - 1) {
                        fprintf(stream, ", ");
                    }
                }
                fprintf(stream, "]).\n");
            }
        }
    }
}

int
main(int argc, char **argv)
{
    int ret = EXIT_SUCCESS;

    if (2 != argc) {
        fprintf(stderr, "usage: jing2indigo file.jing\n");
        exit(EXIT_FAILURE);
    }

    infile = argv[1];
    if (!(yyin = fopen(infile, "r"))) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    yyparse();
    /* must be called before `yylex_destroy` */
    fclose(yyin);
    yylex_destroy();

    if (yynerrs) {
        ret = EXIT_FAILURE;
    }

    semcheck_stub(ntop);
    emitter_walk_stub(ntop, stdout);
    jing_destroy();

    return ret;
}
