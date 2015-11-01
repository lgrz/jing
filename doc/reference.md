# Developer Guide


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


## The Jing Language Reference

The notation used in this document is specified with Extended Backus-Naur Form
(EBNF).


### Identifiers

Identifiers name program entites such as procedures and types. They must start
with a lowercase letter, otherwise they would clash with Prolog variables.

    upper = "A"..."Z" ;
    lower = "a"..."z" ;
    digit = "0"..."9" ;
    identifier = lower { lower | upper | digit | '_' } ;


### Variables

Variables are used for arguments in procedure definitions and are equivalent to
Prolog variables. Note that Prolog's `_` variable is currently not a part of
the Jing specification. Variables for `pick` are identified with a leading `#`.

    variable = upper { lower | upper | digit | '_' } ;
    pick_variable = '#' lower { lower | upper | digit | '_' } ;


### Literals

    integer = digit { digit } ;


### Keywords

The following keywords are reserved and cannot be used.

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
or
pconc
pick
procedure
prolog
rel
search
then
true
while
```


### Grammar

```
program = opt_xdcl_list ;

stmt = common_dcl, term
     | simple_stmt
     | complex_stmt
     | compound_stmt
     | empty_stmt ;

empty_stmt = term ;

simple_stmt = predicate
            | formula_stmt ;

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

if_stmt = 'if', '(', jing_expr, ')', compound_stmt, elseif_list, else ;

elseif = 'else', 'if', '(', jing_expr, ')', compound_stmt ;

elseif_list = { elseif_list, elseif } ;

else = { 'else', compound_stmt } ;

while_stmt = 'while', '(', jing_expr, ')', compound_stmt ;

iter_stmt = 'iter', compound_stmt ;

citer_stmt = 'citer', compound_stmt ;

pick_stmt = 'pick', pick_arg_list_r, compound_stmt ;

search_stmt = 'search', compound_stmt ;

interrupt_stmt = 'interrupt', '(', jing_expr, ')', compound_stmt
               | 'interrupt', '(', jing_expr, ')', predicate ;

ndet_stmt = 'ndet', compound_stmt, or_list ;

conc_stmt = 'conc', compound_stmt, or_list ;

pconc_stmt = 'pconc', compound_stmt, then_list ;

or_list = or_item
        | or_list, or_item ;

or_item = 'or', compound_stmt ;

then_list = then_item
          | then_list, then_item ;

then_item = 'then', compound_stmt ;

formula_stmt = '?', '(', jing_expr, ')' ;

jing_expr = '~', jing_expr
          | jing_expr, '||', jing_expr
          | jing_expr, '&&', jing_expr
          | '(', jing_expr, ')'
          | expr ;

expr = expr, '<', expr
     | expr, '>', expr
     | expr, '<=', expr
     | expr, '>=', expr
     | pseudo_expr ;

pseudo_expr = dcl_expr
            | var
            | identifier
            | integer
            | 'true'
            | 'false'
            | pick_variable ;

dcl_expr = identifier, '(', opt_arg_list, ')' ;

predicate = identifier
          | identifier, '(', opt_arg_list, ')' ;

xdcl = common_dcl
     | xproc_dcl
     | term ;

common_dcl = action_dcl
           | rel_fluent_dcl
           | fun_fluent_dcl
           | prolog_dcl ;

action_dcl = 'action', identifier, ':', integer ;

rel_fluent_dcl = 'rel fluent', identifier, ':', integer ;

fun_fluent_dcl = 'fun fluent', identifier, ':', integer ;

prolog_dcl = 'prolog', identifier, ':', integer ;

xproc_dcl = proc_dcl, compound_stmt ;

proc_dcl = 'procedure', identifier, '(', opt_var_list, ')' ;

xdcl_list_r = xdcl
            | xdcl_list_r, xdcl ;

stmt_list_r = stmt
            | stmt_list_r, stmt ;

var_list_r = var
           | var_list_r, ',', var ;

var = variable ;

arg_list_r = arg
           | arg_list_r, ',', arg ;

arg = pseudo_expr ;

pick_arg_list_r = pick_arg
                | pick_arg_list_r, ',', pick_arg ;

pick_arg = pick_variable ;

opt_xdcl_list = { xdcl_list_r } ;

opt_arg_list = { arg_list_r } ;

opt_stmt_list = { stmt_list_r } ;

opt_var_list = { var_list_r } ;

term = ';' ;
```


### Future work

Possible ideas for future work and improvements.

**Support for additional Prolog operators.** For example: `=`, `is`, etc. There
are many more that could be added.

**Support floating point numbers.** Currently only integers are supported.

**Shorthand control flow statements.** Allow for the omission of `{}` for a
single statement following a control flow statement such as `if`, `while`, etc.

**Translate multiple files at the same time.** Enable jing2indigo to translate
multiple files in succession instead of having to run each file as a seperate
command.

**Declare multiple action/fluent/prolog on one line.** Add syntactic sugar to
allow declarations of the following form:

    action a: 0, b: 1, c: 0;

as an alternative to the existing syntax:

    action a: 0;
    action b: 1;
    action c: 0;

**Pretty print.** Add an option to output the generated Prolog code in a more
readable format.
