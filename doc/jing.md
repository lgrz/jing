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
