/*
 *  This is a flex input file but should be edited in -*-C-*- mode
 *
 *  Modula2LaTeX 1.0:
 *  Produce prettyprinted LaTeX files from Modula-2 or Pascal sources.
 *
 *  Copyright (C) 1991 Joerg Heitkoetter
 *  Systems Analysis Group, University of Dortmund
 *  (heitk...@gorbi.informatik.uni-dortmund.de).
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

%x STRING COMMENT
%s MODULA2

%{
#define KEY             printf ("{\\%s %s}", keyword_font, yytext)
#define STDFUN          unput("("); printf ("{\\%s %s} (", stdfun_font, yytext)
#define SYM(x)          printf ("$\\%s$", x)
#define OUT(x)          printf ("%s", x)
#define CTAB            printf ("\\hspace*{%d\\indentation}", tabtocomment)
#define FONT(x)         printf ("{\\%s ",x);
#define SUB(x)          substitute(x)
#define IND             indent(yytext)
#define INIT            BEGIN (modula_mode ? MODULA2 : INITIAL);
#define REPARSE         yyless (0)

#include <stdio.h>

#define ON      (1)
#define OFF     (0)

int modula_mode         = ON;

int complete_file       = OFF;
int header              = OFF;
int tabtotab            = 8;
int piped               = OFF;

int aligntoright        = ON;           /* align comments to the right  -joke */
int tabtocomment        = 4;            /* distance between statement and corresponding comment  -joke */

char *indentation       = "0.5em";

char* font_size         = "10";
char* comment_font      = "it";
char* keyword_font      = "bf";
char* header_font       = "sl";
char* stdfun_font       = "bf";
char* string_font       = "tt";

#ifdef __STDC__
void substitute(const char*);
void indent(const char*);
void newpage(int);
void usage(const char*);
#else
void substitute();
void indent();
void newpage();
void usage();
#endif
%}

%%

                       INIT;

"CHR"[ \t]*"("              |
"DIV"[ \t]*"("              |
"DISPOSE"[ \t]*"("  |
"MOD"[ \t]*"("              |
"NEW"[ \t]*"("              |
"SUCC"[ \t]*"("             |
"ORD"[ \t]*"("              |
"ODD"[ \t]*"("              |
"PRED"[ \t]*"("             |
"ROUND"[ \t]*"("    |
"TRUNC"[ \t]*"("    |
<MODULA2>"ABS"[ \t]*"("       |
<MODULA2>"CAP"[ \t]*"("       |
<MODULA2>"EXCL"[ \t]*"("      |
<MODULA2>"FLOAT"[ \t]*"("     |
<MODULA2>"HIGH"[ \t]*"("      |
<MODULA2>"INCL"[ \t]*"("      |
<MODULA2>"MIN"[ \t]*"("       |
<MODULA2>"MAX"[ \t]*"("       |
<MODULA2>"REAL"[ \t]*"("      |
<MODULA2>"VAL"[ \t]*"("       |
<MODULA2>"DEC"[ \t]*"("       |
<MODULA2>"INC"[ \t]*"("       STDFUN;

"AND"                 |
"ARRAY"                       |
"BEGIN"                       |
"CASE"                        |
"CONST"                       |
"DO"                  |
"DOWNTO"              |
"ELSE"                        |
"END"                 |
"EXIT"                        |
"FOR"                 |
"FUNCTION"            |
"FROM"                        |
"IF"                  |
"HALT"                        |
"IN"                  |
"NOT"                 |
"OF"                  |
"OR"                  |
"PACKED"              |
"PROCEDURE"           |
"RECORD"              |
"REPEAT"              |
"SET"                 |
"STRING"              |
"THEN"                        |
"TO"                  |
"TYPE"                        |
"UNTIL"                       |
"VAR"                 |
"WHILE"                       |
"WITH"                        |
"INTEGER"             |
"CHAR"                        |
"REAL"                        |
"BOOLEAN"             |
"NIL"                 |
<MODULA2>"SHORTINT"     |
<MODULA2>"CARDINAL"     |
<MODULA2>"SHORTCARD"    |
<MODULA2>"BITSET"       |
<MODULA2>"PROC" |
<MODULA2>"BY"           |
<MODULA2>"DEFINITION"   |
<MODULA2>"ELSIF"        |
<MODULA2>"EXPORT"       |
<MODULA2>"FOREIGN"      |               /* Mocka special -joke */
<MODULA2>"IMPLEMENTATION"       |
<MODULA2>"IMPORT"       |
<MODULA2>"LOOP"         |
<MODULA2>"MODULE"       |
<MODULA2>"POINTER"      |
<MODULA2>"RETURN"       |
<MODULA2>"QUALIFIED"    KEY;

"<="                       SYM ("leq");
">="                       SYM ("geq");
"<>"                    SYM ("neq");
"*"                   SYM ("ast");
"^"                   SYM ("wedge");
"|"                   SYM ("mid");
<MODULA2>"#"            SYM ("neq");
<MODULA2>"~"            SYM ("sim");
"<"                        OUT ("$<$");
">"                        OUT ("$>$");
"{"                   OUT ("\\{");
"}"                   OUT ("\\}");
<MODULA2>"&"                OUT ("\\&");
":"                   |
"="                   |
","                   |
"."                   |
";"                   |
"-"                   |
"+"                   |
"/"                   |
":="                  |
"("                   |
")"                   |
"["                   |
"]"                   ECHO;

<MODULA2>[a-zA-Z_$][a-zA-Z_$0-9]* |
[a-zA-Z_][a-zA-Z_0-9]*                  SUB (yytext);

^[ \t]*"(*"                 { BEGIN (COMMENT);
                               FONT (comment_font);
                                REPARSE; }

"(*"                        { BEGIN (COMMENT);
                                if (aligntoright) {
                                       OUT ("\\hfill");
                               } else {
                                       CTAB;
                               }
                               FONT (comment_font);
                                OUT ("($\\ast$"); }

<COMMENT>"*)"                 { INIT; OUT ("$\\ast$)}"); }
<COMMENT>"\n"                   OUT ("\\mbox{}\\\\\n");
<COMMENT>[ \t]+                   IND;
<COMMENT>.                        SUB (yytext);

L?\"                             { BEGIN (STRING);
                               FONT (string_font); OUT ("\""); }
<STRING>"\""                     { INIT; OUT ("\"}"); }

L?'                           { BEGIN (STRING);
                               FONT (string_font); OUT ("'"); }
<STRING>"'"                   { INIT; OUT ("'}"); }
<STRING>"\n"                    OUT ("\\mbox{}\\\\\n");
<STRING>^[ \t]+                   IND;
<STRING>.                 SUB (yytext);

([0-9]*\.[0-9]+[fFlL]?)          |
([0-9]+\.[0-9]*[fFlL]?)          |
([0-9]*\.?[0-9]+[eE][+-]?[0-9]+) |
([0-9]+\.?[0-9]*[eE][+-]?[0-9]+) ECHO;

[0-9]+[uUlL]?                   ECHO;

L?'[ -~]'                       |
L?'\\[ntvbrfa\\?'"]'               |
L?'\\[0-7]{1,3}'                |
L?'\\x[0-9a-fA-F]{1,2}'         SUB (yytext);

0[0-7]+[uUlL]?                  ECHO;

0x[0-9a-fA-F]+[uUlL]?           ECHO;

"\\\n"                                OUT ("$\\backslash$\\\\\n");
^[ \t]+                         IND;
[ \t]+                          ECHO;
"\f"[\n]?                     OUT ("\\newpage\n");
"\n"                          OUT ("\\mbox{}\\\\\n");

%%

#include "main.c"
