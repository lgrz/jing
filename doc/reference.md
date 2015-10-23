The Jing Language Reference
===========================

## Testing

The project is configured with both unit tests and integration tests.
[CPPUTEST][cpputest] is used for the unit tests and a series of custom Python
scripts are used to run the integration tests.


### Setup CPPUTEST

To be able to run the unit tests CPPUTEST must be installed and some
configuration of the build system is in order. The following steps should be
enough to get up and running with CPPUTEST:

1. C++

    CPPUTEST requires a C++ compiler to run so make sure you have one
    installed before proceeding further.

2. Install CPPUTEST

    You may be able to install CPPUTEST via your package manager or, you can
    follow the instructions in the [CPPUTEST documentation][cpputest] to build
    it from source.

3. Run `make local.mk`

    This will create a file 'local.mk' in the root of the project that is used
    to customise the path to CPPUTEST as the location CPPUTEST is installed may
    not be same across different systems.

4. Set the `CPPUTEST_PATH` in the file 'local.mk'.

    Within the 'local.mk' file update the make variable `CPPUTEST_PATH` with
    the path to where CPPUTEST was installed. For example
    `CPPUTEST_PATH=/usr/local`.

5. Run `make check-jing`

    This will build and run the unit tests and you should see some test output
    on the console. If you got this far great! CPPUTEST is setup and
    configured.

[cpputest]: https://cpputest.github.io


### Running the tests

All of the tests can be run via various make targets which are explained below.

The available test targets are:


1. Run the unit tests:

        make check-jing

2. Run the passing tests located in 'test/trans-pass':

        make check-tpass

3. Run the failing tests located in 'test/trans-fail':

        make check-tfail

4. Runs all passing tests in 'test/trans-pass' through valgrind to check for
   memory leaks:

        make check-tvalgrind

5. Run all tests except the valgrind tests:

        make check

6. Run all tests including the valgrind tests:

        make check-full


## Identifiers

### Keywords

```
action
citer
conc
else
false
fluent
fun
if
interrupt
iter
ndet
pconc
pick
procedure
prolog
rel
search
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
variable = [A-Z][a-zA-Z0-9_]* ;
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

pick_stmt = 'pick', '<', var_list_r, '>', compound_stmt ;

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
           | rel_fluent_dcl
           | fun_fluent_dcl
           | prolog_dcl ;

action_dcl = 'action', name, ':', number ;

rel_fluent_dcl = 'rel', 'fluent', name, ':', number ;

fun_fluent_dcl = 'fun' 'fluent', name, ':', number ;

prolog_dcl = 'prolog', name, ':', number ;

xproc_dcl = proc_dcl, compound_stmt ;

proc_dcl = 'procedure', name, '(', opt_var_list, ')' ;

xdcl_list_r = xdcl
            | xdcl_list_r, xdcl ;

stmt_list_r = stmt
            | stmt_list_r, stmt ;

var_list_r = var
           | var_list_r, ',', var ;

var: variable ;

arg_list_r = arg
           | arg_list_r, ',', arg ;

arg = name ;

delta_list = { delta_list, delta_item } ;

opt_xdcl_list = { xdcl_list_r } ;

opt_stmt_list = { stmt_list_r } ;

opt_var_list = { var_list_r } ;

opt_arg_list = { arg_list_r } ;

term = ';' ;
```
