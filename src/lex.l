/*
 * This file is a part of Jing.
 *
 * Copyright (c) 2015 The Jing Authors.
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

%option noyywrap
%option yylineno
%option nodefault

%{
#include "jing.h"
#include "y.tab.h"
%}

%%

":" |
"~" |
"." |
"?" |
"," |
";" |
"<" |
">" |
"{" |
"}" |
"(" |
")"         { return yytext[0]; }

"||"        { return LOROR; }
"&&"        { return LANDAND; }

"if"        { return LIF; }
"else"      { return LELSE; }
"pick"      { return LPICK; }
"iter"      { return LITER; }
"ndet"      { return LNDET; }
"conc"      { return LCONC; }
"pconc"     { return LPCONC; }
"true"      { return LTRUE; }
"false"     { return LFALSE; }
"citer"     { return LCITER; }
"while"     { return LWHILE; }
"search"    { return LSEARCH; }
"prolog"    { return LPROLOG; }
"action"    { return LACTION; }
"fun"       { return LFUN; }
"rel"       { return LREL; }
"fluent"    { return LFLUENT; }
"interrupt" { return LINTERRUPT; }
"procedure" { return LPROCEDURE; }

[a-z][a-zA-Z0-9_]* { yylval.sym = symtab_lookup(yytext); return LNAME; }
[a-zA-Z0-9_]*	   { yylval.sym = symtab_lookup(yytext); return LVARIABLE; }

[0-9]+  { return LNUMBER; }

"//".*
[ \t\n]
.       { yyerror("%c\n", *yytext); }

%%
