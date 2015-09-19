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
#include <string.h>

#include "jing.h"
#include "strbuf.h"

/* AST root node */
struct node *
ntop = NULL;

static char *
infile = NULL;

/*
 * Display error with line number.
 */
void
yyerror(const char *s, ...)
{
    va_list ap;

    ++yynerrs;

    va_start(ap, s);
    fprintf(stderr, "%s:%d: error: ", infile, yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

/*
 * Display error with location information.
 */
void
yyerrorl(YYLTYPE t, const char *s, ...)
{
    va_list ap;

    ++yynerrs;

    va_start(ap, s);
    if (t.first_line) {
        fprintf(stderr, "%s:%d:%d %d:%d error: ", infile, t.first_line,
                t.first_column, t.last_line, t.last_column);
    }
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
    emitter_free();
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
    char *ptr;

    if (2 != argc) {
        fprintf(stderr, "usage: jing2indigo file.jing\n");
        exit(EXIT_FAILURE);
    }

    infile = argv[1];
    if (!(yyin = fopen(infile, "r"))) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    ptr = strrchr(infile, '/');
    if (ptr) {
        infile = ++ptr;
    }

    emitter_init(strbuf_new());

    yyparse();
    /* must be called before `yylex_destroy` */
    fclose(yyin);
    yylex_destroy();

    if (yynerrs) {
        error_exit();
    }

    fprintf(stdout, "%s", emitter_get_str());
    jing_destroy();

    return 0;
}
