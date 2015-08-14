/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

%{
#include "jing.h"

int
yylex(void);
%}

%token keyword_hello

%%

program:
       | program stmt term
;

stmt:
    | keyword_hello
        {
            printf("world\n");
        }
;

term: '\n'
;

%%
