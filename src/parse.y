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
    long num;
    char *str;
}

%token LACTION
       LFLUENT
       LPROLOG
       LIF
       LELSE
       LOR
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
       LNOT
       LOROR
       LANDAND
       LREL
       LFUN
       LLT
       LGT
       LLTE
       LGTE

%token LNUMBER
       LNAME
       LVARIABLE

%type <node> opt_xdcl_list xdcl_list_r xdcl
%type <node> opt_stmt_list stmt_list_r
%type <node> opt_arg_list arg_list_r arg
%type <node> opt_var_list var_list_r var
%type <node> xproc_dcl proc_dcl
%type <node> action_dcl rel_fluent_dcl fun_fluent_dcl prolog_dcl
%type <node> if_stmt elseif_list elseif else
%type <node> while_stmt iter_stmt citer_stmt
%type <node> pick_stmt search_stmt interrupt_stmt
%type <node> ndet_stmt conc_stmt pconc_stmt
%type <node> or_list or_item
%type <node> jing_expr expr dcl_expr
%type <node> pseudo_expr formula_stmt
%type <node> predicate
%type <node> stmt common_dcl simple_stmt complex_stmt compound_stmt empty_stmt

%type <sym> LNAME LVARIABLE
%type <num> LNUMBER
%type <str> LLT LLTE LGT LGTE
%type <str> LNOT LANDAND LOROR

%left LOROR
%left LANDAND
%left LLT LLTE LGT LGTE
%right LNOT

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

simple_stmt: predicate
           | formula_stmt
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

if_stmt: LIF '(' jing_expr ')' compound_stmt elseif_list else
        {
            $$ = node_if_new($3, $5, $6, $7);
        }
;

elseif: LELSE LIF '(' jing_expr ')' compound_stmt
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

while_stmt: LWHILE '(' jing_expr ')' compound_stmt
            {
                $$ = node_while_new($3, $5);
            }
;

iter_stmt: LITER compound_stmt
            {
                $$ = node_comstmt_new(NODE_ITER, $2);
            }
;

citer_stmt: LCITER compound_stmt
            {
                $$ = node_comstmt_new(NODE_CITER, $2);
            }
;

pick_stmt: LPICK '<' var_list_r '>' compound_stmt
            {
            }
;

search_stmt: LSEARCH compound_stmt
            {
                $$ = node_comstmt_new(NODE_SEARCH, $2);
            }
;

interrupt_stmt: LINTERRUPT '(' jing_expr ')' compound_stmt
                {
                    $$ = node_interrupt_new($3, $5);
                }
              | LINTERRUPT '(' jing_expr ')' predicate
                {
                    struct node *list = node_list_new();
                    node_list_add(list, $5);
                    $$ = node_interrupt_new($3, list);
                }
;

ndet_stmt: LNDET '{' compound_stmt or_list '}'
            {
                struct node *list = node_list_new();
                $$ = node_comstmt_new(NODE_OR, $3);
                node_list_add(list, $$);
                node_list_append(list, $4);
                $$ = node_comstmt_new(NODE_NDET, list);
            }
;

conc_stmt: LCONC '{' compound_stmt or_list '}'
            {
                struct node *list = node_list_new();
                $$ = node_comstmt_new(NODE_OR, $3);
                node_list_add(list, $$);
                node_list_append(list, $4);
                $$ = node_comstmt_new(NODE_CONC, list);
            }
;

pconc_stmt: LPCONC '{' compound_stmt or_list '}'
            {
                struct node *list = node_list_new();
                $$ = node_comstmt_new(NODE_OR, $3);
                node_list_add(list, $$);
                node_list_append(list, $4);
                $$ = node_comstmt_new(NODE_PCONC, list);
            }
;

or_list: or_item
            {
                $$ = NULL;
                if (NULL != $1) {
                    $$ = node_list_new();
                    node_list_add($$, $1);
                }
            }
           | or_list or_item
            {
                $$ = $1;
                if (NULL != $2) {
                    node_list_add($$, $2);
                }
            }
;

or_item: LOR compound_stmt
            {
                $$ = node_comstmt_new(NODE_OR, $2);
            }
;

formula_stmt: '?' '(' jing_expr ')'
              {
                $$ = node_formula_new($3);
              }
;

jing_expr: LNOT jing_expr
    {
        $$ = node_op_new(ONOT, $1, $2, NULL);
    }
    | jing_expr LOROR jing_expr
    {
        $$ = node_op_new(OOROR, $2, $1, $3);
    }
    | jing_expr LANDAND jing_expr
    {
        $$ = node_op_new(OANDAND, $2, $1, $3);
    }
    | '(' jing_expr ')'
    {
        $$ = node_op_new(ONONE, NULL, $2, NULL);
    }
    | expr
    {
        $$ = $1;

        enum error_code err_code = ENONE;
        err_code = semcheck_expr($$);
        if (semcheck_is_error(err_code)) {
            semcheck_errorl(@1, err_code, $$);
        }
    }
;

expr: expr LLT expr
    {
        $$ = node_op_new(OPROLOG, $2, $1, $3);
    }
    | expr LGT expr
    {
        $$ = node_op_new(OPROLOG, $2, $1, $3);
    }
    | expr LLTE expr
    {
        $$ = node_op_new(OPROLOG, $2, $1, $3);
    }
    | expr LGTE expr
    {
        $$ = node_op_new(OPROLOG, $2, $1, $3);
    }
    | pseudo_expr
;

pseudo_expr: dcl_expr
           | var
           | LNAME
            {
                $$ = node_symref_new($1, node_list_new());
            }
           | LNUMBER
            {
                $$ = node_int_new($1);
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

dcl_expr: LNAME '(' opt_arg_list ')'
        {
            $$ = node_symref_new($1, $3);
        }
;

predicate: LNAME
                {
                    $$ = node_symref_new($1, node_list_new());

                    enum error_code err_code = ENONE;
                    err_code = semcheck_action_defined($1);
                    if (semcheck_is_error(err_code)) {
                        semcheck_errorl(@1, err_code, $$);
                    }

                    /* check argument count */
                    err_code = semcheck_sym_args($$);
                    if (semcheck_is_error(err_code)) {
                        semcheck_errorl(@1, err_code, $$);
                    }
                }
           | LNAME '(' opt_arg_list ')'
                {
                    $$ = node_symref_new($1, $3);

                    enum error_code err_code = ENONE;

                    /* check symbol is defined */
                    err_code = semcheck_usercall_defined($1);
                    if (semcheck_is_error(err_code)) {
                        semcheck_errorl(@1, err_code, $$);
                    }

                    /* check argument count */
                    err_code = semcheck_sym_args($$);
                    if (semcheck_is_error(err_code)) {
                        semcheck_errorl(@3, err_code, $$);
                    }
                }
;

xdcl: common_dcl
    | xproc_dcl
        {
            $$ = $1;
            emitter_gen_node($$);
        }
    | term
        {
            $$ = NULL;
        }
;

common_dcl: action_dcl
          | rel_fluent_dcl
          | fun_fluent_dcl
          | prolog_dcl
;

action_dcl: LACTION LNAME ':' LNUMBER
            {
                $$ = node_comdcl_new(TACTION, $2, $4);
            }
;

rel_fluent_dcl: LREL LFLUENT LNAME ':' LNUMBER
            {
                $$ = node_comdcl_new(TFLUENTREL, $3, $5);
            }
;

fun_fluent_dcl: LFUN LFLUENT LNAME ':' LNUMBER
            {
                $$ = node_comdcl_new(TFLUENTFUN, $3, $5);
            }
;

prolog_dcl: LPROLOG LNAME ':' LNUMBER
            {
                $$ = node_comdcl_new(TPROLOG, $2, $4);
            }
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
              {
                $$ = node_list_new();
                node_list_add($$, $1);
              }
          | var_list_r ',' var
              {
                $$ = $1;
                node_list_add($$, $3);
              }
;

var: LVARIABLE
    {
        /* Dummy list for args, not used. */
        $$ = node_symref_new($1, node_list_new());
    }
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

arg: pseudo_expr
;

/* optional */
opt_xdcl_list:
                 {
                    $$ = NULL;
                 }
             | xdcl_list_r
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

opt_stmt_list:
                {
                    $$ = NULL;
                }
             | stmt_list_r
;

opt_var_list:
        {
            $$ = node_list_new();
        }
        | var_list_r
        {
            $$ = $1;
        }
;


term: ';'
;

%%
