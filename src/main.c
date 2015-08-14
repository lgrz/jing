/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#include <stdio.h>
#include <stdarg.h>

#include "jing.h"

void
yyerror(char *s, ...)
{
    va_list ap;

    va_start(ap, s);
    fprintf(stderr, "%d: error: ", yylineno);
    vfprintf(stderr, s, ap);
    fprintf(stderr, "\n");
}

int
main(int argc, char **argv)
{
    (void) argc;
    (void) argv;

    return yyparse();
}
