The Jing Language Reference
===========================

## Identifiers

### Keywords

```
action
citer
conc
else
false
fluent
if
interrupt
iter
ndet
pconc
pick
procedure
search
skip
true
while
```

### Identifiers

Identifiers must start with a lowercase letter, so they do not clash with
Prolog variables.

```
identifier = [a-z][a-zA-Z0-9_]* ;
number = [0-9]+ ;
name = identifier ;
```

## Grammar

```
program = opt_xdcl_list ;

stmt = common_dcl, term
     | simple_stmt
     | complex_stmt
     | compound_stmt
     | empty_stmt ;

empty_stmt = term ;

simple_stmt = psuedo_expr
            | formula_expr ;

complex_stmt = if_stmt
             | while_stmt
             | iter_stmt
             | citer_stmt
             | pick_stmt
             | search_stmt
             | interrupt_stmt
             | ndet_stmt
             | conc_stmt
             | pconc_stmt ;

compound_stmt = '{', opt_stmt_list, '}' ;

if_stmt = 'if', '(', expr, ')', compound_stmt, elseif_list, else ;

elseif = 'else', 'if', '(', expr, ')', compound_stmt ;

elseif_list = { elseif_list, elseif } ;

else = { 'else', compound_stmt } ;

while_stmt = 'while', '(', expr, ')', compound_stmt ;

iter_stmt = 'iter', compound_stmt ;

citer_stmt = 'citer', compound_stmt ;

pick_stmt = 'pick', '<', arg_list_r, '>', compound_stmt ;

search_stmt = 'search', compound_stmt ;

interrupt_stmt = 'interrupt', '(', expr, ')', compound_stmt
               | 'interrupt', '(', expr, ')', psuedo_expr ;

ndet_stmt = 'ndet', '{', delta_pair, delta_list, '}' ;

conc_stmt = 'conc', '{', delta_pair, delta_list, '}' ;

pconc_stmt = 'pconc', '{', delta_pair, delta_list, '}' ;

delta_pair = delta_item, delta_item ;

delta_item = delta_label, compound_stmt ;

delta_label = name, '.' ;

formula_expr = '?', '(', expr, ')' ;

expr = '~', expr
     | expr, '||', expr
     | expr, '&&', expr
     | '(', expr, ')'
     | psuedo_expr
     | "true"
     | "false" ;

psuedo_expr = name
            | name, '(', opt_arg_list, ')' ;

xdcl = common_dcl
     | xproc_dcl
     | term ;

common_dcl = action_dcl
           | fluent_dcl ;

action_dcl = 'action', name, '(', arg_list_r, ')' ;

fluent_dcl = 'fluent', name, '(', arg_list_r, ')' ;

xproc_dcl = proc_dcl, compound_stmt ;

proc_dcl = 'procedure', name, '(', opt_arg_list, ')' ;

xdcl_list_r = xdcl
            | xdcl_list_r, xdcl ;

stmt_list_r = stmt
            | stmt_list_r, stmt ;

arg_list_r = arg
           | arg_list_r, ',', arg ;

arg = name ;

delta_list = { delta_list, delta_item } ;

opt_xdcl_list = { xdcl_list_r } ;

opt_stmt_list = { stmt_list_r } ;

opt_arg_list = { arg_list_r } ;

term = ';' ;
```
