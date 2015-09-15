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

%error-verbose

%union {
    struct node *node;
    struct symbol *sym;
    int num;
}

%token LACTION
       LFLUENT
       LPROLOG
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
       LREL
       LFUN

%token LNUMBER
       LNAME
       LVARIABLE

%type <node> opt_xdcl_list xdcl_list_r xdcl
%type <node> opt_stmt_list stmt_list_r
%type <node> opt_arg_list arg_list_r arg
%type <node> opt_var_list
%type <node> xproc_dcl proc_dcl
%type <node> action_dcl
%type <node> if_stmt elseif_list elseif else
%type <node> while_stmt iter_stmt citer_stmt
%type <node> pick_stmt search_stmt interrupt_stmt
%type <node> ndet_stmt conc_stmt pconc_stmt
%type <node> expr
%type <node> psuedo_expr formula_expr
%type <node> stmt common_dcl simple_stmt complex_stmt compound_stmt empty_stmt

%type <sym> LNAME LVARIABLE
%type <num> LNUMBER

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
            $$ = node_if_new($3, $5, $6, $7);
        }
;

elseif: LELSE LIF '(' expr ')' compound_stmt
        {
            $$ = node_if_new($4, $6, NULL, NULL);
        }
;

elseif_list:
            {
                $$ = node_list_new();
            }
           | elseif_list elseif
            {
                $$ = $1;
                node_list_add($$, $2);
            }

else:
    {
        $$ = NULL;
    }
    | LELSE compound_stmt
    {
        $$ = $2;
    }
;

while_stmt: LWHILE '(' expr ')' compound_stmt
            {
                $$ = node_while_new($3, $5);
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

pick_stmt: LPICK '<' var_list_r '>' compound_stmt
            {
            }
;

search_stmt: LSEARCH compound_stmt
            {
                $$ = node_search_new($2);
            }
;

interrupt_stmt: LINTERRUPT '(' expr ')' compound_stmt
                {
                    $$ = node_interrupt_new($3, $5);
                }
              | LINTERRUPT '(' expr ')' psuedo_expr
                {
                    struct node *list = node_list_new();
                    node_list_add(list, $5);
                    $$ = node_interrupt_new($3, list);
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
    {
        yyerror("`~` not implemented");
    }
    | expr LOROR expr
    {
        yyerror("`||` not implemented");
    }
    | expr LANDAND expr
    {
        yyerror("`&&` not implemented");
    }
    | '(' expr ')'
    {
        yyerror("`()` not implemented");
    }
    | psuedo_expr
    {
        yyerror("`action/proc call` not implemented");
    }
    | LTRUE
    {
        $$ = node_get_true();
    }
    | LFALSE
    {
        $$ = node_get_false();
    }
;

psuedo_expr: LNAME
                {
                    $$ = node_symref_new($1, node_list_new());
                }
           | LNAME '(' opt_arg_list ')'
                {
                    $$ = node_symref_new($1, $3);
                }
;

xdcl: common_dcl
    | xproc_dcl
        {
            $$ = $1;
        }
    | term
        {
            $$ = NULL;
        }
;

common_dcl: action_dcl
            {
                $$ = $1;
            }
          | rel_fluent_dcl
            {
            }
          | fun_fluent_dcl
            {
            }
          | prolog_dcl
            {
            }
;

action_dcl: LACTION LNAME ':' LNUMBER
            {
                $$ = node_comdcl_new(TACTION, $2, $4);
            }
;

rel_fluent_dcl: LREL LFLUENT LNAME ':' LNUMBER
;

fun_fluent_dcl: LFUN LFLUENT LNAME ':' LNUMBER
;

prolog_dcl: LPROLOG LNAME ':' LNUMBER
;

xproc_dcl: proc_dcl compound_stmt
         {
            $$ = $1;
            node_proc_body($$, $2);
         }
;

proc_dcl: LPROCEDURE LNAME '(' opt_var_list ')'
        {
            $$ = node_proc_new($2, $4);
        }
;

/* lists */
xdcl_list_r: xdcl
                {
                    $$ = node_list_new();
                    if (NULL != $1) {
                        node_list_add($$, $1);
                    }
                }
           | xdcl_list_r xdcl
                {
                    $$ = $1;
                    if (NULL != $2) {
                        node_list_add($$, $2);
                    }
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

var_list_r: var
      | var_list_r ',' var
;

var: LVARIABLE
;

arg_list_r: arg
              {
                $$ = node_list_new();
                node_list_add($$, $1);
              }
          | arg_list_r ',' arg
              {
                $$ = $1;
                node_list_add($$, $3);
              }
;

arg: LNAME
    {
        $$ = node_symref_new($1, node_list_new());
    }
;

delta_list:
         | delta_list delta_item
;

/* optional */
opt_xdcl_list:
                 {
                    $$ = NULL;
                 }
             | xdcl_list_r
;

opt_stmt_list:
                {
                    $$ = NULL;
                }
             | stmt_list_r
;

opt_var_list:
        {
            $$ = NULL;
        }
        | var_list_r
        {
            $$ = NULL;
        }
;

opt_arg_list:
                {
                    $$ = node_list_new();
                }
            | arg_list_r
                {
                    $$ = $1;
                }
;

term: ';'
;

%%
