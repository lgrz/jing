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

%union {
    struct node *node;
    struct symbol *sym;
}

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

%type <node> opt_xdcl_list xdcl_list_r xdcl
%type <node> opt_stmt_list stmt_list_r
%type <node> opt_arg_list
%type <node> xproc_dcl proc_dcl
%type <node> if_stmt while_stmt iter_stmt citer_stmt
%type <node> pick_stmt search_stmt interrupt_stmt
%type <node> ndet_stmt conc_stmt pconc_stmt
%type <node> psuedo_expr formula_expr
%type <node> stmt common_dcl simple_stmt complex_stmt compound_stmt empty_stmt

%type <sym> LNAME

%left LOROR
%left LANDAND
%right '~'

%%

program: opt_xdcl_list
       {
            ntop = $1;
       }
;

stmt: common_dcl term
    | simple_stmt
    | complex_stmt
    | compound_stmt
    | empty_stmt
;

empty_stmt: term
                {
                    $$ = NULL;
                }
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
                {
                    $$ = $2;
                }
;

if_stmt: LIF '(' expr ')' compound_stmt elseif_list else
        {
        }
;

elseif: LELSE LIF '(' expr ')' compound_stmt
        {
        }
;

elseif_list:
           | elseif_list elseif

else:
    | LELSE compound_stmt
;

while_stmt: LWHILE '(' expr ')' compound_stmt
            {
            }
;

iter_stmt: LITER compound_stmt
            {
            }
;

citer_stmt: LCITER compound_stmt
            {
            }
;

pick_stmt: LPICK '<' arg_list_r '>' compound_stmt
            {
            }
;

search_stmt: LSEARCH compound_stmt
            {
            }
;

interrupt_stmt: LINTERRUPT '(' expr ')' compound_stmt
                {
                }
              | LINTERRUPT '(' expr ')' psuedo_expr
                {
                }
;

ndet_stmt: LNDET '{' delta_pair delta_list '}'
            {
            }
;

delta_pair: delta_item delta_item
;

delta_item: delta_label compound_stmt
;

delta_label: LNAME '.'
;

conc_stmt: LCONC '{' delta_pair delta_list '}'
            {
            }
;

pconc_stmt: LPCONC '{' delta_pair delta_list '}'
            {
            }
;

formula_expr: '?' '(' expr ')'
              {
              }
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
                {
                    $$ = node_symref_new($1);
                }
           | LNAME '(' opt_arg_list ')'
                {
                }
;

xdcl: common_dcl
    | xproc_dcl
        {
        }
    | term
        {
        }
;

common_dcl: action_dcl
            {
            }
          | fluent_dcl
            {
            }
;

action_dcl: LACTION LNAME '(' arg_list_r ')'
;

fluent_dcl: LFLUENT LNAME '(' arg_list_r ')'
;

xproc_dcl: proc_dcl compound_stmt
         {
            $$ = $1;
            node_proc_body($$, $2);
         }
;

proc_dcl: LPROCEDURE LNAME '(' opt_arg_list ')'
        {
            $$ = node_proc_new($2, $4);
        }
;

/* lists */
xdcl_list_r: xdcl
                {
                }
           | xdcl_list_r xdcl
                {
                }
;

stmt_list_r: stmt
                {
                    $$ = NULL;
                    if (NULL != $1) {
                        $$ = node_list_new();
                        node_list_add($$, $1);
                    }
                }
           | stmt_list_r stmt
                {
                    $$ = $1;
                    if (NULL != $2) {
                        node_list_add($$, $2);
                    }
                }
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
                 {
                 }
             | xdcl_list_r
                {
                }
;

opt_stmt_list:
                {
                    $$ = NULL;
                }
             | stmt_list_r
;

opt_arg_list:
                {
                    $$ = NULL;
                }
            | arg_list_r
                {
                    $$ = NULL;
                }
;

term: ';'
;

%%
