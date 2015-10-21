Jing
====

Introduction
------------

Jing is an agent oriented programming language that provides a Java like syntax
for writing [IndiGolog][indigolog] programs.

[indigolog]: http://www.cs.toronto.edu/cogrobo/main/systems/

IndiGolog is an AI high-level agent oriented programming language developed at
the University of Toronto used to program robots or agent software.

The IndiGolog language is built upon Prolog and programs must be written in a
Prolog style. Prolog is a declarative language and is less popular than the
well known C-family of programming languages where the imperative and object
oriented paradigms are more widely accepted. This is one of the motivating
factors for introducing the Jing programming language.

The goal of Jing is to reduce the barrier to entry for writing IndiGolog
programs (which requires good foundational knowledge of Prolog) and, to make it
easier to write IndiGolog programs whether that be for new users or, existing
users of Prolog and IndiGolog.

Jing is made up of two components: the Jing language and jing2indigo, a
translation tool for converting Jing programs into IndiGolog programs.


jing2indigo
-----------

jing2indigo is a source-to-source translator that takes a program written in
the Jing language and translates it into an IndiGolog program.


### Build Instructions

To build jing2indigo run `make all` or `make` ('all' is the default target).


### Install Instructions

There is no installation method at this time. To install copy the jing2indigo
binary somewhere in your `PATH`:

    cp jing2indigo /usr/local/bin


### Usage

To translate the Jing program hello.jing to an IndiGolog program world.pl:

    jing2indigo -o world.pl hello.jing

When an output file is not specified the default is to print the output to
`stdout`:

    jing2indigo hello.jing

jing2indigo will only accept one input file per invocation, so the following
will only result in translating the file 1.jing:

    jing2indigo 1.jing 2.jing 3.jing


Jing
----

Here we give a short introduction to the Jing language and in the next section
we examine more of the language and its features in detail.

Let's look at a simple example to get a feel for the syntax and some of the
basic features of Jing:

    action forward: 0;
    fun fluent goal: 0;

    procedure control() {
        while (~goal) {
            forward;
        }
    }

The above program is not a very interesting one and is a rather contrived
example, but will be adequate for our purposes for the moment.

This simple program controls an agent that starts at one end of a hallway and
walks to the other end and stops there.

This program continues to execute the action `forward` until it has reached its
`goal`. The procedure `control` is similar to a main function in other
languages and the name 'control' is more of a convention rather than a
requirement.

Each iteration of the loop checks the functional fluent to see if the goal is
reached. A functional fluent can be either true or false. The `~` operator is
the logical not operator that performs negation just like in any other
programming language. Within the loop we make a call to an `action` named
forward, which is declared at the top of the file along with the functional
fluent `goal`. Both the action and the functional fluent are declared as taking
0 arguments. In Jing everything must be declared before it can be used.

Here is the corresponding IndiGolog program:

    proc(control,
        while(neg(goal),
            forward
        )
    ).

The IndiGolog program appears to be smaller in this example as it doesn't
require the action or fluent declarations. However, this is not true. Both Jing
and IndiGolog programs require additional IndiGolog code that is left out here
for simplicity. The additional code defines the action theory that details the
state of the world (or more formally an *axiomatisation of the dynamics of the
world*) for the agent software and is based upon the [Situation
calculus][situation_calculus]. We won't be covering any of that in this
documentation but you should be aware that it exists. More information on this
can be found in the [IndiGolog documentation][indg_docs].

[situation_calculus]: https://en.wikipedia.org/wiki/Situation_calculus
[indg_docs]: https://bitbucket.org/ssardina/indigolog


### Going further: vacuum world example

Let's look at some more of the syntax and language features by looking at a
more interesting agent program.

The following program models the vacuum world problem. In this particular model
of the vacuum world there are two rooms that can be either dirty or clean. A
vacuum cleaner robot starts in room 1 (the left room) and room 1 is already
clean and room 2 is dirty. The agent can perform 4 simple actions: move left,
move right, suck and power down. The agent's goals are to clean dirty rooms
and, if there are no more rooms left to clean it will power itself down to save
its battery.

```
action left: 0;
action right: 0;
action suck: 1;
action power_down: 0;

rel fluent location: 0;
rel fluent room: 1;

prolog some: 2;
prolog is_dirty: 1;
prolog in_location: 1;

procedure go_room(N) {
    while (~in_location(N)) {
        if (location < N) {
            right;
        } else {
            left;
        }
    }
}

procedure clean_a_room() {
    pick #n {
        ?(is_dirty(#n));
        go_room(#n);
        suck(#n);
    }
}

procedure control() {
    while (some(n, is_dirty(n))) {
        clean_a_room;
    }
    power_down;
}
```

At the top of the file there are some delcarations. There are four kinds of
declarations:

* action
* relational fluent
* functional fluent
* prolog

An `action` is something the agent can perform in its environment. Here our
agent can perform the actions `left`, `right`, `suck` and `power_down`. For the
action `suck` we've specified that it takes 1 argument, the room in which the
suck action is performed.

A relational fluent can be considered a variable that changes depending on the
current state of the world, for example as the vacuum agent moves between rooms
the `location` fluent will change to reflect the agent's current location.
Relational fluents can be seen as taking on a range of values (i.e. they are
continuous).

Functional fluents are similar to relational fluents, however they can only be
either true or false.

The `prolog` declaration provides a way to 'import' Prolog predicates that can
be used within a program. The term import is used very loosely here. There is
no importing performed, we just tell Jing to assume that the declared predicate
exists in Prolog or IndiGolog elsewhere.

We can see that even for declarations that do not take any arguments we still
must specify the number of arguments (perhaps a syntactical limitation at this
stage). We only need to know the arity in these declarations because Prolog is
dynamically typed and the definitions are located elsewhere (in addtional
IndiGolog code that is required when running IndiGolog programs, however this
is beyond the scope of this guide).

In the `clean_a_room` procedure there are some things we haven't touched on
yet. The `pick` construct is used to nondeterministrically bind a variable to
the statements contained within the `{}` and, if successful execute those
statements. The variable to be bound by the `pick` statement is declared with a
`#` symbol. This example shows a variable `n` is declared and if the room `n`
is dirty then the agent will go to the room `n` and clean it.

Also within this `pick` block of code we can see the `?(...)` syntax. This is
the formula statement and it is used to test if a condition is true or false,
similar to an `if` statement, however there is no branching in the formula
statement and in this case if the `is_dirty(#n)` evaluates to false the
statements in the `pick` block that come after the formula statement will not
be executed.


### Expressions

The expression syntax in Jing is complete in some areas and still needs more
work in others. We'll highlight where this is the case in the details below.


#### Logical operators

Jing provides expression support for logical operations that are defined in the
IndiGolog language as `and/2`, `or/2` and `neg/1`.

In Jing the logical operators appear much the same way as they do in a Java
like syntax:

    if (~a && b || c) {
        // ...
    }


#### Prolog expressions

It is common for IndiGolog programs to make use of Prolog expressions and
operators to perform various tasks. Jing provides limited support for such
expressions at this time. Current support extends to the following operators:
`<`, `<=`, `>`, `>=`.

If you find you need support for an operator not in the above list, for example
unification `=`, a work around is to wrap the operator into a predicate and
import it via the `prolog` declaration described earlier.


### Semantic checks

There are a number of semantic checks performed during translation of a Jing
program, here we highlight some of the key semantic checks that are performed.

* Use of an action limited to only as a [sequence statement][structured_prog].
* Use of a `rel fluent` is limited to only as a [selection
  statement][structured_prog] with the added restriction that it must be used
  with a relational operator i.e. an operator that takes two arguments. For
  example the `>` operator takes two arguments. What this also means is that we
  can't use a `rel fluent` as an unary expression.
* Use of a `fun fluent` is limited to a [selection statement][structured_prog]
  with the added restriction that it must be as a unary expression.
* Attemtping to use undeclared `pick` variables will result in an error.
* The number of arguments passed to an `action` or `procedure` must match with
  the definition.

[structured_prog]: https://en.wikipedia.org/wiki/Structured_programming#Control_structures


### Examples

More examples of Jing programs can be found in the 'examples' directory.
