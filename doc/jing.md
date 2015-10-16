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
