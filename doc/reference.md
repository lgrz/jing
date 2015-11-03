# Developer Guide

* [Overview](#overview)
    * [Scanning and parsing](#scanning-and-parsing)
    * [Intermediate representation](#intermediate-representation)
    * [Semantic checker](#semantic-checker)
    * [Code generation and output](#code-generation-and-output)
* [Limitations](#limitations)
* [Testing](#testing)
* [The Jing Language Reference](#the-jing-language-reference)
* [Future work](#future-work)


## Overview

Jing is a relatively simple language, and one of the design decisions was that
all steps of parsing, semantic analysis and code generation are performed as a
single pass (for the most part) over the input. Most of the calls to perform
these tasks are embedded in the parser itself. However, each of the steps
involved is more or less self contained in its own module(s).


### Scanning and parsing

The project uses [Flex][flex] and [Bison][bison] to handle the scanning and
parsing of input files. A reference of the grammar used can be found below in
the [grammar](#grammar) section.

[flex]: http://flex.sourceforge.net
[bison]: https://www.gnu.org/software/bison


### Intermediate representation

The in memory representation that is constructed during parsing more or less
resembles a tree like data structure. Each type and language construct is
represented as a node in the tree. Semantic checking and code generation
utilise this data structure to perform their various tasks.


### Semantic checker

Some basic semantic analysis is performed on the intermediate representation
at various stages during parsing. The semantic checker's primary goals are to
validate the use of types in various contexts and to use this contextual
information to report informative errors when they arise.


### Code generation and output

As each `procedure` is seen by the parser, code generation is performed before
continuing on through the rest of the input (which is a list of procedures).

Code generation traverses the intermediate representation of each `procedure`
and this could be considered a second pass over the input. However, it is a
single pass in the sense that code generation is performed at the same time as
parsing and semantic analysis. The decision to perform code generation in this
manner was for readability and maintainability, there is just one call to the
code generation module from within the parser.

The generated code is written out to an internal buffer and once the parser is
done and all code has been generated, and providing there are no errors, the
buffer is written to the output destination.


## Limitations

**Validate procedure arguments.** Arguments to procedures are not checked for
their correct use within the procedure (in a similar way to pick variables). It
may make sense to check or not check them in future. One case for not checking
them is where a local Prolog variable may be used inside a procedure that is
used to store a return value from a predicate that is called within the
procedure, this would make checking for the valid use of procedure arguments
difficult.

**Argument count for prolog/fluent is not checked.**

**The expressions `?(~u > 1)` and `?(~(u > 1))` have the same semantics.** Even
though `~` has a higher precedence than `>` in this case the first case `~u >
1` is considered equivalent to `~(u > 1)`. This is unintuitive as the negation
operator has higher precedence. A further implication is that `~` results in a
true/false value and the currently available Prolog operators in Jing are all
arithmetic operators which the Jing/IndiGolog logic operators cannot be used
with.

**An expression can't be passed as as argument to anything.** This is a pretty
big limitation, some cases of where this would be useful are shown below.

* A procedure that has a base case

        procedure fib(N) { /* ... */ }
        procedure fib(1) { /* ... */ } // error

* Passing the result of an expression to other types that take arguments

        rel fluent a: 1;
        fun fluent b: 1;
        procedure myproc(S) {}
        procedure foo() {
            if (b(~a)) {} // error
            myproc(1 > 2); // error
        }

**Error reporting for expressions.** Errors reported from the result of using
an action or procedure inside an expression don't display to the user the name
of the item that caused the error.

The problem is that offending item is reported as 'unknown'. The limitation in
this case is that expression data structure needs to be traversed to find the
offending item. More work is needed to provide a custom semcheck error data
structure that can collect all of the relevant information at the time the
error occurs instead of traversing data structures at the time of displaying the
error. Going further the custom errors could be stored in an array or queue and
then dumped out after parsing is done or, at any other time that is convenient.

**The semantic checker performs both semantic analysis and error reporting.** A
possible improvement could be to move the error reporting functionality to a
separate error module.

**Prolog anonymous variable.** The `_` anonymous variable can't be used in
Jing.


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


## Future work

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

**Improve error reporting.** More informative errors that output the line of
code along with an indicator of where the error occured on that line.

    action_undefined.jing:11:5 11:7 error: unresolved action `bar`
    action_undefined.jing:11    bar;
                                ^
