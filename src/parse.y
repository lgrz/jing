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

%token LACTION
       LFLUENT
       LIF
       LELSE
       LWHILE
       LPROCEDURE
       LITER
       LCITER
       LPICK
       LSEARCH
       LTRUE
       LFALSE
       LINTERRUPT
       LNDET
       LCONC
       LPCONC
       LOROR
       LANDAND

%token LNUMBER
       LNAME

%left LOROR
%left LANDAND
%right '~'

%%

program: opt_xdcl_list
;

stmt: common_dcl term
    | simple_stmt
    | complex_stmt
    | compound_stmt
    | empty_stmt
;

empty_stmt: term
;

simple_stmt: psuedo_expr
           | formula_expr
;

complex_stmt: if_stmt
            | while_stmt
            | iter_stmt
            | citer_stmt
            | pick_stmt
            | search_stmt
            | interrupt_stmt
            | ndet_stmt
            | conc_stmt
            | pconc_stmt
;

compound_stmt: '{' opt_stmt_list '}'
;

if_stmt: LIF '(' expr ')' compound_stmt elseif_list else
;

elseif: LELSE LIF '(' expr ')' compound_stmt
;

elseif_list:
           | elseif_list elseif

else:
    | LELSE compound_stmt
;

while_stmt: LWHILE '(' expr ')' compound_stmt
;

iter_stmt: LITER compound_stmt
;

citer_stmt: LCITER compound_stmt
;

pick_stmt: LPICK '<' arg_list_r '>' compound_stmt
;

search_stmt: LSEARCH compound_stmt
;

interrupt_stmt: LINTERRUPT '(' expr ')' compound_stmt
              | LINTERRUPT '(' expr ')' psuedo_expr
;

ndet_stmt: LNDET '{' delta_pair delta_list '}'
;

delta_pair: delta_item delta_item
;

delta_item: delta_label compound_stmt
;

delta_label: LNAME '.'
;

conc_stmt: LCONC '{' delta_pair delta_list '}'
;

pconc_stmt: LPCONC '{' delta_pair delta_list '}'
;

formula_expr: '?' '(' expr ')'
;

expr: '~' expr
    | expr LOROR expr
    | expr LANDAND expr
    | '(' expr ')'
    | psuedo_expr
    | LTRUE
    | LFALSE
;

psuedo_expr: LNAME
           | LNAME '(' opt_arg_list ')'
;

xdcl: common_dcl
    | xproc_dcl
    | term
;

common_dcl: action_dcl
          | fluent_dcl
;

action_dcl: LACTION LNAME '(' arg_list_r ')'
;

fluent_dcl: LFLUENT LNAME '(' arg_list_r ')'
;

xproc_dcl: proc_dcl compound_stmt
;

proc_dcl: LPROCEDURE LNAME '(' opt_arg_list ')'
;

/* lists */
xdcl_list_r: xdcl
           | xdcl_list_r xdcl
;

stmt_list_r: stmt
           | stmt_list_r stmt
;

arg_list_r: arg
        | arg_list_r ',' arg
;

arg: LNAME
;

delta_list:
         | delta_list delta_item
;

/* optional */
opt_xdcl_list:
             | xdcl_list_r
;

opt_stmt_list:
             | stmt_list_r
;

opt_arg_list:
            | arg_list_r
;

term: ';'
;

%%
