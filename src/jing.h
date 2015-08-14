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

extern int
yylineno;

extern int
yyparse(void);

void
yyerror(char *s, ...);

#endif /* JING_H */
