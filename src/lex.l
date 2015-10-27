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

%x COMMENT

%{
#include "jing.h"

#define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;

int yycolumn = 1;
%}

%%

":" |
"." |
"?" |
"," |
";" |
"{" |
"}" |
"(" |
")"         { return yytext[0]; }

"<"         { yylval.str = strdup(yytext); return LLT; }
">"         { yylval.str = strdup(yytext); return LGT; }
"<="        { yylval.str = strdup(yytext); return LLTE; }
">="        { yylval.str = strdup(yytext); return LGTE; }
"~"         { yylval.str = strdup(yytext); return LNOT; }
"||"        { yylval.str = strdup(yytext); return LOROR; }
"&&"        { yylval.str = strdup(yytext); return LANDAND; }

"if"        { return LIF; }
"else"      { return LELSE; }
"or"        { return LOR; }
"then"      { return LTHEN; }
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
[A-Z][a-zA-Z0-9_]* { yylval.sym = symtab_lookup(yytext); return LVARIABLE; }

[0-9]+  { yylval.num = atoi(yytext); return LNUMBER; }

<INITIAL>"/*"           { BEGIN(COMMENT); }
<COMMENT>"*/"           { BEGIN(INITIAL); }
<COMMENT>([^*]|\n)+|.
<COMMENT><<EOF>>        { yyerror("unterminated comment"); error_exit(); }

"//".*
[ \t]
\n      { yycolumn = 1; }
.       { yyerror("%c\n", *yytext); }

%%
