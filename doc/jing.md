The Jing Language Reference
===========================

Introduction
------------

Jing is an agent oriented programming language that provides a Java like syntax
for writing [IndiGolog][indigolog] programs.

[indigolog]: http://www.cs.toronto.edu/cogrobo/main/systems/

IndiGolog Lexical Constructs
----------------------------

The Jing language is derived directly from IndiGolog. The following lists the
IndiGolog constructs available (incomplete):

```prolog
prim_action(action).
exog_action(action).
poss(action, cond).
```

A formal specification of the IndiGolog language is provided in the [IndiGolog
paper][indigolog_paper]: *A High-Level Programming Language for Embedded
Reasoning Agents*.

[indigolog_paper]: http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.146.2722

Jing Specification
------------------

### Primitive action

A basic action that can be performed by an agent.

IndiGolog:

```prolog
prim_action(x).
```

Jing:

```
action x;
```
