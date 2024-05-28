%{
#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

/* To store tokens */
TOKEN *token;
%}

DIGIT       [0-9]
ID          [a-zA-Z_][a-zA-Z0-9_]*
FLOAT       {DIGIT}+"."{DIGIT}*
STRING      \"[^\n\"]*\"
COMMENT     "##"[^\n]*

%%

{COMMENT}      ; /* Skip comments */
{ID}           {
                    token = new TOKEN();
                    if (check_keyword(yytext, token)) {
                        printf("A keyword: %s\n", yytext);
                    } else {
                        token->type = lx_identifier;
                        token->str_ptr = strdup(yytext);
                        printf("An identifier: %s\n", yytext);
                    }
                    return token->type;
                }
{FLOAT}        {
                    token = new TOKEN();
                    token->type = lx_float;
                    token->float_value = atof(yytext);
                    printf("A float: %s (%g)\n", yytext, token->float_value);
                    return token->type;
                }
{DIGIT}+       {
                    token = new TOKEN();
                    token->type = lx_integer;
                    token->value = atoi(yytext);
                    printf("An integer: %s (%d)\n", yytext, token->value);
                    return token->type;
                }
{STRING}       {
                    token = new TOKEN();
                    token->type = lx_string;
                    token->str_ptr = strdup(yytext + 1); // Remove quotes
                    token->str_ptr[strlen(token->str_ptr) - 1] = '\0'; // Remove closing quote
                    printf("A string: %s\n", token->str_ptr);
                    return token->type;
                }
"program"      { return kw_program; }
"var"          { return kw_var; }
"constant"     { return kw_constant; }
"integer"      { return kw_integer; }
"boolean"      { return kw_boolean; }
"string"       { return kw_string; }
"float"        { return kw_float; }
"true"         { return kw_true; }
"false"        { return kw_false; }
"if"           { return kw_if; }
"fi"           { return kw_fi; }
"then"         { return kw_then; }
"else"         { return kw_else; }
"while"        { return kw_while; }
"do"           { return kw_do; }
"od"           { return kw_od; }
"and"          { return kw_and; }
"or"           { return kw_or; }
"read"         { return kw_read; }
"write"        { return kw_write; }
"for"          { return kw_for; }
"from"         { return kw_from; }
"to"           { return kw_to; }
"by"           { return kw_by; }
"function"     { return kw_function; }
"procedure"    { return kw_procedure; }
"return"       { return kw_return; }
"not"          { return kw_not; }
"begin"        { return kw_begin; }
"end"          { return kw_end; }

"+"            { return lx_plus; }
"-"            { return lx_minus; }
"*"            { return lx_star; }
"/"            { return lx_slash; }
"="            { return lx_eq; }
"!="           { return lx_neq; }
"<"            { return lx_lt; }
"<="           { return lx_le; }
">"            { return lx_gt; }
">="           { return lx_ge; }
"("            { return lx_lparen; }
")"            { return lx_rparen; }
"["            { return lx_lbracket; }
"]"            { return lx_rbracket; }
"{"            { return lx_lbracket; }
"}"            { return lx_rbracket; }
":"            { return lx_colon; }
"."            { return lx_dot; }
";"            { return lx_semicolon; }
","            { return lx_comma; }
":="           { return lx_colon_eq; }

[ \t\n]+       ; /* Skip whitespace */

.              { printf("Unrecognized character: %s\n", yytext); }

%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror(argv[1]);
            return 1;
        }
    }
    yylex();
    return 0;
}
