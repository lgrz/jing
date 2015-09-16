/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef JING_H
#define JING_H

#include <stdio.h>

#include "node.h"
#include "symtab.h"
#include "emitter.h"

extern struct node *
ntop;

extern FILE *
yyin;

extern int
yylineno;

extern int
yynerrs;

extern int
yyparse(void);

extern int
yylex_destroy(void);

void
yyerror(const char *s, ...);

extern int
error_exit(void);

#endif /* JING_H */
