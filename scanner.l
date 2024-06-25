cpp
Copy code
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "fd.h"
#include "scanner.h"

/* To store tokens */
TOKEN *token;
FileDescriptor *fd;

/* Function to check if an identifier is a keyword */
int check_keyword(const char *str, TOKEN *token) {
    if (strcasecmp(str, "program") == 0) { token->type = kw_program; return 1; }
    if (strcasecmp(str, "var") == 0) { token->type = kw_var; return 1; }
    if (strcasecmp(str, "constant") == 0) { token->type = kw_constant; return 1; }
    if (strcasecmp(str, "integer") == 0) { token->type = kw_integer; return 1; }
    if (strcasecmp(str, "boolean") == 0) { token->type = kw_boolean; return 1; }
    if (strcasecmp(str, "string") == 0) { token->type = kw_string; return 1; }
    if (strcasecmp(str, "float") == 0) { token->type = kw_float; return 1; }
    if (strcasecmp(str, "true") == 0) { token->type = kw_true; return 1; }
    if (strcasecmp(str, "false") == 0) { token->type = kw_false; return 1; }
    if (strcasecmp(str, "if") == 0) { token->type = kw_if; return 1; }
    if (strcasecmp(str, "fi") == 0) { token->type = kw_fi; return 1; }
    if (strcasecmp(str, "then") == 0) { token->type = kw_then; return 1; }
    if (strcasecmp(str, "else") == 0) { token->type = kw_else; return 1; }
    if (strcasecmp(str, "while") == 0) { token->type = kw_while; return 1; }
    if (strcasecmp(str, "do") == 0) { token->type = kw_do; return 1; }
    if (strcasecmp(str, "od") == 0) { token->type = kw_od; return 1; }
    if (strcasecmp(str, "and") == 0) { token->type = kw_and; return 1; }
    if (strcasecmp(str, "or") == 0) { token->type = kw_or; return 1; }
    if (strcasecmp(str, "read") == 0) { token->type = kw_read; return 1; }
    if (strcasecmp(str, "write") == 0) { token->type = kw_write; return 1; }
    if (strcasecmp(str, "for") == 0) { token->type = kw_for; return 1; }
    if (strcasecmp(str, "from") == 0) { token->type = kw_from; return 1; }
    if (strcasecmp(str, "to") == 0) { token->type = kw_to; return 1; }
    if (strcasecmp(str, "by") == 0) { token->type = kw_by; return 1; }
    if (strcasecmp(str, "function") == 0) { token->type = kw_function; return 1; }
    if (strcasecmp(str, "procedure") == 0) { token->type = kw_procedure; return 1; }
    if (strcasecmp(str, "return") == 0) { token->type = kw_return; return 1; }
    if (strcasecmp(str, "not") == 0) { token->type = kw_not; return 1; }
    if (strcasecmp(str, "begin") == 0) { token->type = kw_begin; return 1; }
    if (strcasecmp(str, "end") == 0) { token->type = kw_end; return 1; }
    return 0; // Not a keyword
}

void yyerror(const char *s) {
    fd->ReportError((char *)s);
}
%}

DIGIT       [0-9]
ID          [a-zA-Z_][a-zA-Z0-9_]*
FLOAT       {DIGIT}+"."{DIGIT}*
STRING      \"[^\n\"]*\"
COMMENT     "##"[^\n]*

%%

{COMMENT}      { /* Skip comments */ }
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
                    token->str_ptr[strlen(token->str_ptr.c_str()) - 1] = '\0'; // Remove closing quote
                    printf("A string: %s\n", token->str_ptr.c_str());
                    return token->type;
                }

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
"{"            { return lx_lbrace; }
"}"            { return lx_rbrace; }
":"            { return lx_colon; }
"."            { return lx_dot; }
";"            { return lx_semicolon; }
","            { return lx_comma; }
":="           { return lx_colon_eq; }

[ \t\n]+       ; /* Skip whitespace */

.              { yyerror("Unrecognized character"); }

%%

int yywrap() {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        fd = new FileDescriptor(argv[1]);
    } else {
        fd = new FileDescriptor();
    }

    if (!fd->IsOpen()) {
        perror("Failed to open input");
        return 1;
    }

    yyin = fdopen(fileno(fd->getFilePointer()), "r");

    while (yylex() != 0) {
        // Continue lexing until EOF
    }

    delete fd;
    return 0;
}