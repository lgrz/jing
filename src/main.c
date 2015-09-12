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
#include "emitter.h"
#include "semcheck.h"

/* AST root node */
struct node *
ntop = NULL;

static char *
infile = NULL;

void
yyerror(const char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
    va_end(ap);
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

    semcheck_walk(ntop);
    emitter_init(stdout);
    emitter_walk(ntop);
    jing_destroy();

    return ret;
}
