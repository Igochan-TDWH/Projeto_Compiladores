%{
// HEADERS
#include <stdlib.h>
#include<string.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+      {  }
#.*\n       { yyline++; }
\n          { yyline++; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"+"         { return PLUS; }
"*"         { return MULT; }
"/"         { return DIV; }
"-"         { return SUB; }
"%"         { return REST; }
"not"       { return NOT; }
"or"        { return OR; }
"and"       { return AND; }
"<"         { return LESS; }
">"         { return MORE; }
"="         { return EQUAL; }
"<="        { return LEQUAL; }
">="        { return MEQUAL; }
"!="        { return DIFF; }
":="        { return DECLARE; }
":"         { return ELIP; }
";"         { return SEMI; }
"true"      { return TRUE; }
"false"     { return FALSE; }
"if"        { return IF; }
"else"      { return ELSE; }
"then"      { return THEN; }
"while"     { return WHILE; }
"do"        { return DO; }
"begin"     { return BEG; }
"end."      { return END; }
"read"      { return READ; }
"write"     { return WRITE; }
"integer"   {  return INTEGER; }
"real"      {  return REAL; }
"boolean"   {  return BOOL; }
"char"      {  return CHAR; }
"string"    {  return STRING; }
\-?[0-9]+ { 
   yylval.intValue = atoi(yytext);
   return INT; 
}
\-?([0-9]+([.][0-9]*)?|[.][0-9]+) { 
   yylval.floatValue = atof(yytext);
   return FLOAT; 
}
[a-zA-Z_][a-zA-Z0-9_]* {
   yylval.strValue = strdup(yytext);
   return ID;
}
.        { yyerror("unexpected character"); }
%%

